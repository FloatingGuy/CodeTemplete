/*
 * Android Hacker's Handbook
 * Chapter 10
 *
 * This module simply prints Hello World to the kernel ring buffer.
 *
 * dev:kernel $ make ARCH=arm CONFIG_AHH_HELLOWORLD=m M=drivers/ahh_helloworld
 */
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
    printk(KERN_INFO "%s: HELLO WORLD!@#!@#\n", __this_module.name);
    return 0;
}


void cleanup_module(void)
{
    /* nothing here */
}


MODULE_LICENSE("GPL v2"); /* don't taint the kernel! */
MODULE_AUTHOR("Android Hackers");
MODULE_DESCRIPTION("hello world!");
MODULE_VERSION("1.0");

