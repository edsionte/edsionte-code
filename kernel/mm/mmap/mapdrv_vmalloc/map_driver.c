#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <linux/mman.h>
#include "map_driver.h"
#define MAPLEN (PAGE_SIZE*10)

extern struct mm_struct init_mm;
/* device open */
int mapdrv_open(struct inode *inode, struct file *file);
/* device close */
int mapdrv_release(struct inode *inode, struct file *file);
/*device mmap */
int mapdrv_mmap(struct file *file, struct vm_area_struct *vma);
/* vm area open */
void map_vopen(struct vm_area_struct *vma);
/* vm area close */
void map_vclose(struct vm_area_struct *vma);
/* vm area nopage */
int map_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

static struct file_operations mapdrv_fops = {
	.owner = THIS_MODULE,
	.mmap = mapdrv_mmap,
	.open = mapdrv_open,
	.release = mapdrv_release,
};

static struct vm_operations_struct map_vm_ops = {
	.open = map_vopen,
	.close = map_vclose,
	.fault = map_fault,
};

static unsigned long *vmalloc_area = NULL;
static int major;		/*major number of device */
struct mapdrv* md;
MODULE_LICENSE("GPL");

static int __init mapdrv_init(void)
{
	struct page *page = NULL;
	int result, err;
	dev_t dev = 0;
	
	printk("mapdrv_init is running..\n");
	md = kmalloc(sizeof(struct mapdrv), GFP_KERNEL);
	if (!md)
		goto fail1;
	result = alloc_chrdev_region(&dev, 0, 1, "mapdrv");
	major = MAJOR(dev);
	if (result < 0) {
		printk(KERN_WARNING "mapdrv: can't get major %d\n", major);
		goto fail2;
	}
	cdev_init(&md->mapdev, &mapdrv_fops);
	md->mapdev.owner = THIS_MODULE;
	md->mapdev.ops = &mapdrv_fops;
	err = cdev_add (&md->mapdev, dev, 1);
	if (err) {
		printk(KERN_NOTICE "Error %d adding mapdrv", err);
		goto fail3;
	}
	atomic_set(&md->usage, 0);
	/* get a memory area that is only virtual contigous. */
	vmalloc_area = vmalloc(MAPLEN);
	if (!vmalloc_area)
		goto fail4;

	for (page = vmalloc_to_page(vmalloc_area); 
			page < vmalloc_to_page(vmalloc_area + MAPLEN); page++) {
		if (!page)
			printk("page is null..\n");
		SetPageReserved(page);
	}
	printk("set page reserved ok..\n");

	strcpy((char *)vmalloc_area, "hello world from kernel space !");
	printk("strcpy ok\n");

	printk("vmalloc_area at 0x%p (phys 0x)\n", vmalloc_area);
	return 0;
fail4:
	cdev_del(&md->mapdev);	
fail3:
	unregister_chrdev_region(dev, 1);
fail2:
	kfree(md);
fail1:
	return -1;
}

static void __exit mapdrv_exit(void)
{
	struct page *page;
	dev_t devno = MKDEV(major, 0);
	/* unreserve all pages */
	for (page = vmalloc_to_page(vmalloc_area); 
			page < vmalloc_to_page(vmalloc_area + MAPLEN); page++) {
		if (!page)
			printk("page is null..\n");
		SetPageReserved(page);
	}

	/* and free the two areas */
	if (vmalloc_area)
		vfree(vmalloc_area);

	cdev_del(&md->mapdev);
	/* unregister the device */
	unregister_chrdev_region(devno, 1);
	kfree(md);
	printk("mapdrv module is leaving..\n");
}

/* device open method */
int mapdrv_open(struct inode *inode, struct file *file)
{
	struct mapdrv *md;
	md = container_of(inode->i_cdev, struct mapdrv, mapdev);
	atomic_inc(&md->usage);
	printk("device is opened..\n");

	return 0;
}

/* device close method */
int mapdrv_release(struct inode *inode, struct file *file)
{
	struct mapdrv* md;
	md = container_of(inode->i_cdev, struct mapdrv, mapdev);
	atomic_dec(&md->usage);

	printk("device is closed..\n");
	return 0;
}

int mapdrv_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;

	unsigned long size = vma->vm_end - vma->vm_start;
	if (offset & ~PAGE_MASK) {
		printk("offset not aligned: %ld\n", offset);
		return -ENXIO;
	}
	if (size > MAPLEN) {
		printk("size too big\n");
		return -ENXIO;
	}
	/*  only support shared mappings. */
	if ((vma->vm_flags & VM_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		printk("writeable mappings must be shared, rejecting\n");
		return -EINVAL;
	}
	/* do not want to have this area swapped out, lock it */
	vma->vm_flags |= VM_LOCKED;
	if (offset == 0) {
		vma->vm_ops = &map_vm_ops;
		/* call the open routine to increment the usage count */
		map_vopen(vma);
	} else {
		printk("offset out of range\n");
		return -ENXIO;
	}

	printk("mmaping..\n");
	return 0;
}

/* open handler for vm area */
void map_vopen(struct vm_area_struct *vma)
{
	/* needed to prevent the unloading of the module while
	   somebody still has memory mapped */
	printk("vma is opened..\n");
}

/* close handler form vm area */
void map_vclose(struct vm_area_struct *vma)
{
	printk("vma is closed..\n");
}

/* page fault handler */
int map_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	/*
	unsigned long offset;
	unsigned long virt_addr;
	unsigned long address = *((unsigned long *)vmf->virtual_address);

	//determine the offset within the vmalloc'd area  
	offset = address - vma->vm_start + (vma->vm_pgoff << PAGE_SHIFT);
	//translate the vmalloc address to kmalloc address  
	virt_addr =
	    (unsigned long)vaddr_to_kaddr(&vmalloc_area[offset / sizeof(int)]);
	if (virt_addr == 0UL) {
		return ((struct page *)0);
	}
	//increment the usage count of the page
	get_page(virt_to_page(virt_addr));
	printk("map_drv: page fault for offset 0x%lx (kseg x%lx)\n", offset,
	       virt_addr);
	return (virt_to_page(virt_addr));
	*/

	struct page *page = vmalloc_to_page(vmalloc_area);

	if (!page) {
		printk("page is NULL\n");
		return -1;
	}
	get_page(page);
	vmf->page = page;

	printk("the requiring page is returned..\n");
	return 0;
}

module_init(mapdrv_init);
module_exit(mapdrv_exit);

