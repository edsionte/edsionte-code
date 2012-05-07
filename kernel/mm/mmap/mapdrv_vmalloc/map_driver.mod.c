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
	{ 0x5eadf54a, "module_layout" },
	{ 0x999e8297, "vfree" },
	{ 0x37a0cba, "kfree" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xb714e933, "cdev_del" },
	{ 0xe914e41e, "strcpy" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x9b604f4c, "cdev_add" },
	{ 0xe41b8ba6, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x83699014, "kmem_cache_alloc_trace" },
	{ 0x85e90336, "kmalloc_caches" },
	{ 0x3f5689df, "__get_page_tail" },
	{ 0x3b356931, "vmalloc_to_page" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BA3AAD427DE5DE924AED0E3");
