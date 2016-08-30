/*
* @Author: LK_mac
* @Date:   2016-08-18 13:49:42
* @Last Modified by:   FloatingGuy
* @Last Modified time: 2016-08-24 16:08:09
*/

#include <stdio.h>

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
	printf("%lld\n", addr);
	addr = (long) dlsym(hanle, "printab");
	printf("%lld\n", addr);
	addr = (long) dlsym(handle, "printIt");
	printf("%lld\n", addr);

	func = addr - nn;
	func(1);
	dlclose(handle);
	sleep(-1);
    return 0;
}