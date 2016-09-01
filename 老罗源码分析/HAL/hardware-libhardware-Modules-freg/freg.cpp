/*
* @Author: LK_mac
* @Date:   2016-08-26 18:36:47
* @Last Modified by:   liukun
* @Last Modified time: 2016-08-31 14:04:02
* 
* @Reference:
* 	实现 hardware.cpp
*  硬件抽象层模块编写规范：
*  		每一个硬件抽象层模块 必须导出一个名称为 HAL_MODULE_INFO_SYM的符号，它指向一个 （freg_module_t）自定义的硬件抽象层模块结构体
*
*/

#define LOG_TAG  "FregHALStub"

#include <string.h>  // strcmp
#include <stdlib.h>  // malloc   free
#include <hardware/freg.h>
#include <hardware/hardware.h>

#include <fcntl.h>
#include <errno.h>

#include <cutils/log.h>			//ALOGE/ALOGI/ALOGV ..... log/log.h
#include <cutils/atomic.h>
#define DEVICE_NAME "/dev/freg"
#define MODULE_NAME "Freg"
#define MODULE_AUTHOR "liukun@antiy.cn"

/**套路
* 设备的打开和关闭
*/
static int freg_device_open(const struct hw_module_t* module,const char * id, struct hw_device_t** device);
static int freg_device_close(struct hw_device_t* device);

/**功能接口
* 设备寄存器的 读写结构
*
* 凡是需要在函数 内部赋值的都用指针
*/
static int freg_get_val(struct freg_device_t* dev, int* val);
static int freg_set_val(struct freg_device_t* dev, int val);

/**套路
 * 定义模块操作方法结构体 变量 
 */
static struct hw_module_methods_t freg_module_methods = {
	open: freg_device_open     //初始化 (*open)函数指针
};

/**套路
 * 定义模块结构体 变量
 */
struct freg_module_t HAL_MODULE_INFO_SYM = {
	common : {
		tag: HARDWARE_MODULE_TAG,
		version_major: 1,
		version_minor: 0,
		id: FREG_HARDWARE_MODULE_ID,
		name: MODULE_NAME,
		author: MODULE_AUTHOR,
		methods: &freg_module_methods,
	}
};

static int freg_device_open(const struct hw_module_t* module, const char* id, struct hw_device_t** device)
{
	if (!strcmp(id, FREG_HARDWARE_DEVICE_ID)) {
		struct freg_device_t* dev;

		dev = (struct freg_device_t*) malloc(sizeof(struct freg_device_t));   //
		if (!dev)
		{
			ALOGE("Failed to alloc apace for freg_device_t");
			return -EFAULT;
		}
		memset(dev, 0, sizeof(struct freg_device_t));

		dev->common.tag = HARDWARE_MODULE_TAG;   //写死
		dev->common.version = 0;
		dev->common.module = (hw_module_t*)module;
		dev->common.close = freg_device_close;
		dev->set_val = freg_set_val;
		dev->get_val = freg_get_val;

		if ((dev->fd = open(DEVICE_NAME, O_RDWR)) == -1)
		{
			ALOGE("Failed to open device file /dev/freg -- %s", strerror(errno));
			free(dev);
			return -EFAULT;
		}

		*device = &(dev->common);
		ALOGI("Open device file /dev/freg successfully.");

		return 0;
	}
	return -EFAULT;
}


static int freg_device_close(struct hw_device_t* dev)
{
	struct freg_device_t* freg_device = (struct freg_device_t*)dev;  //????????  可能会引用错误，  除非调用代码 传递的就是freg_device_t
	if (freg_device)
	{
		close(freg_device->fd);
		free(freg_device);
	}
	return 0;
}

static int freg_get_val(struct freg_device_t* dev, int* val)
{
	if (!dev)
	{
		ALOGE("Null dev pointer");
		return -EFAULT;
	}

	if (!val)
	{
		ALOGE("Null val pointer");
		return -EFAULT;
	}

	read(dev->fd, val, sizeof(*val));
	ALOGI("Get value %d from device file /dev/freg.", *val);
	return 0;
}

static int freg_set_val(struct freg_device_t* dev, int val)
{
	if (!dev)
	{
		ALOGE("Null dev pointer");
		return -EFAULT;
	}
	ALOGI("get value %d to device file /dev/freg.", val);
	write(dev->fd, &val, sizeof(val));
	return 0;
}
