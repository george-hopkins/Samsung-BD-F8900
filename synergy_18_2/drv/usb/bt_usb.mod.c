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
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x5dce6b75, "complete_and_exit" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x4792c572, "down_interruptible" },
	{ 0xb5e9f692, "usb_kill_urb" },
	{ 0x56dfaad2, "remove_proc_entry" },
	{ 0xea83babe, "__register_chrdev" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x48eb0c0d, "__init_waitqueue_head" },
	{ 0x3fa58ef8, "wait_for_completion" },
	{ 0x2bc95bd4, "memset" },
	{ 0xad7e4f1a, "usb_string" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0x90e90126, "current_task" },
	{ 0x770d96cb, "usb_deregister" },
	{ 0x50eedeb8, "printk" },
	{ 0xd8322212, "usb_set_interface" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0xdbcc5895, "usb_control_msg" },
	{ 0x960ddfe5, "usb_driver_claim_interface" },
	{ 0xdd1a2871, "down" },
	{ 0xc60e1346, "usb_match_id" },
	{ 0x4f9f9032, "usb_submit_urb" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x1269c79d, "usb_reset_device" },
	{ 0x4292364c, "schedule" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x64078393, "create_proc_entry" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x37ff4c06, "copy_from_user_overflow" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0x622fa02a, "prepare_to_wait" },
	{ 0xc4554217, "up" },
	{ 0xce55595f, "usb_register_driver" },
	{ 0x75bb675a, "finish_wait" },
	{ 0x7e9ebb05, "kernel_thread" },
	{ 0xf5cd713, "usb_ifnum_to_if" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x357bd51c, "usb_free_urb" },
	{ 0xdc43a9c8, "daemonize" },
	{ 0x5560b28e, "usb_alloc_urb" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A7E1C663C5B21227C71C3CC");
