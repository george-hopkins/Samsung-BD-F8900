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
	{ 0x2adeec3c, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x77798c4a, "hid_add_device" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xea6c4968, "hid_parse_report" },
	{ 0x5ae7aa06, "device_destroy" },
	{ 0xea83babe, "__register_chrdev" },
	{ 0x9f4305f7, "hid_input_report" },
	{ 0xfc08a9ed, "skb_queue_purge" },
	{ 0x50eedeb8, "printk" },
	{ 0x3328d4ac, "class_unregister" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0xb4390f9a, "mcount" },
	{ 0x175c8888, "hid_disconnect" },
	{ 0xef73a4bd, "device_create" },
	{ 0x19384503, "__alloc_skb" },
	{ 0x534e3a48, "hid_connect" },
	{ 0xb0b7b5d6, "kfree_skb" },
	{ 0xa40600f, "hid_destroy_device" },
	{ 0xb8e5fc, "hid_allocate_device" },
	{ 0x37a0cba, "kfree" },
	{ 0xa0501269, "hidinput_disconnect" },
	{ 0x66b68a3d, "__hid_register_driver" },
	{ 0x1a575dbb, "class_destroy" },
	{ 0x8c543462, "hid_unregister_driver" },
	{ 0x7431f419, "skb_put" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xd618bbde, "__class_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=hid";


MODULE_INFO(srcversion, "750660AF4E31DB5F9E6F0E4");
