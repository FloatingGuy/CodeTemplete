/*
* @Subject: 
* @Model:
* @Compile-cmd: gcc -Wall 
* @Reference: 
* 准备工作：
* 	1. 拉取手机 或 out 目录下的的 libmedia.so 文件
*/

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <SoundPool.h>
//#include <utils/RefBase.h>
#include <StrongPointer.h>    //class sp  


#define TAG "cve-2016-0810"
//#define FILENAME "libmedia.so"
#define FILENAME "./libSoundPool.so"
//#ifdef __cplusplus
//extern "C" {
//#endif

using namespace android;		//note:sp is in 'namespace android'

//int argc, char const **argv
int main(int argc, char const **argv)
{
	void* handle;
	typedef android::sp<Sample> (*findSample_l_p)(int);   //add
	typedef void (*doLoad_p)(android::sp<Sample>);   //del
	const char* error;


	handle = dlopen(FILENAME, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	
	dlerror();

	findSample_l_p	findSample_l_f = (findSample_l_p)dlsym(handle, "findSample");
	if (findSample_l_f == NULL)
	{
		if ((error = dlerror()) != NULL)
		{
			fprintf(stderr, "%s\n", error);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("%s\n", "找不到符号【没有 patch】");
			exit(0);
		}
	}
	printf("%s\n", "找到符号 findSample【成功 patch】");
	exit(EXIT_SUCCESS);
}


//#ifdef __cplusplus

//#endif
