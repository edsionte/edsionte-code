#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7d09bae2, "module_layout" },
	{ 0xbe6a2279, "cdev_del" },
	{ 0x27a3b3fa, "kmalloc_caches" },
	{ 0x4f0b454f, "cdev_init" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xb72397d5, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x6087741d, "cdev_add" },
	{ 0x9dd454a7, "kmem_cache_alloc" },
	{ 0x37a0cba, "kfree" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "457F5D682786E5F785006C3");
