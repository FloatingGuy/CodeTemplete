/*
* @Author: LK_mac
* @Date:   2016-08-17 21:03:32
* @Last Modified by:   liukun
* @Last Modified time: 2016-09-02 14:16:20
*/
#define LOG_TAG "FregServer"

#include <stdlib.h>
#include <fcntl.h>
#include <utils/Log.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "../common/IFregStervice"

#define FREG_DEVICE_NAME "/dev/freg"

class FregService: public BnFrageService
{
public:
	FregService()
	{
		fd = open(FREG_DEVICE_NAME, O_RDWR);
		if(fd == -1)
		{
			LOGE("Failed to open device %s. \n", FREG_DEVICE_NAME);
		}
	}
	virtual ~FregService()
	{
		if (fd == -1)
		{
			close(fd);
		}
	}

	static void instantiate()
	{
		defaultServiceManager()->addService(String16(FREG_SERVICE), new FregService());
	}

	int32_t getVal()
	{
		int32_t val=0;
		if (fd != -1)
		{
			read(fd, &val, sizeof(val));
		}
		return val;
	}

	void setVal(int32_t val)
	{
		if (fd != -1)
		{
			write(fd, &val, sizeof(val));
		}
	}
private:
	int fd;	
};

int main(int argc, char const *argv[])
{
	FregService::instantiate();

	//调用进程的成员函数启动线程池
	ProcessState::self()->startThreadPool();
	//调用主线程IPCThreadState对象的成员函数 将主线程添加到 Binder线程池
	IPCThreadState::self()->joinThreadPool();
	return 0;
}