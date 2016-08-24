/*
* @Author: LK_mac
* @Date:   2016-08-18 11:29:18
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-18 15:14:40
* @command: 
*		gcc [-rdynamic] -o foo foo.c -ldl
*		gcc -shared -nostartfiles -o bar bar.c
*	
* 共享库:
	-shared -fPIC/fpic
* ELF linker 参数： -rdynamic
	这个参数 会导出给ELF linker，命令linker 添加所有的符号（而不是只有被使用了的）到动态符号表。

*/

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#define FILE_NAME  "share.so"

int main() {
	long addr = 0;
	void (*func)();   //定义函数指针

	void* handle = dlopen(FILE_NAME, RTLD_NOW);
	if (handle == NULL)
	{
		fprintf(stderr, "Failed to open library %s error:%s\n",FILE_NAME, dlerror());
		return -1;
	}
	addr = (long) dlsym(handle, "printAB");
	printf("%ld\n", addr);
	addr = (long) dlsym(handle, "printab");
	printf("%ld\n", addr);
	addr = (long) dlsym(handle, "printIt");
	printf("%ld\n", addr);

	func = addr;
	func(1);
	dlclose(handle);
	sleep(-1);

    return 0;
}