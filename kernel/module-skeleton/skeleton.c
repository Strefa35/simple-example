#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("4Embedded.Systems");
MODULE_DESCRIPTION("A skeleton of Linux module.");
MODULE_VERSION("0.01");

static int __init my_init_module(void) {
    printk(KERN_INFO "[Skeleton] Init module\n");
    return 0;
}

static void __exit my_exit_module(void) {
    printk(KERN_INFO "[Skeleton] Exit module\n");
}

module_init(my_init_module);
module_exit(my_exit_module);
