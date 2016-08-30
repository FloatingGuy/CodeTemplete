/* ReadMe
*
*1. 每个HAL 层（硬件抽象层）在内核中都对应有一个驱动程序。
*
*2. 该文件的内容都是『套路』，除了freg_device_t 中定义的两个函数接口，必须更具设备的功能进行定义。
			int (*set_val)(struct freg_device_t* dev, int val);
			int (*get_val)(struct freg_device_t* dev, int* val);
*/


#ifndef ANDROID_FREG_INTERFACE_H
#define ANDROID_FREG_INTERFACE_H

#include <hardware/hardware.h>  //定义三部分：硬件抽像模块结构体、硬件抽象设备结构体、硬件模块抽象层模块操作方法列表

__BEGIN_DECLS

/*定义模块ID*/
#define FREG_HARDWARE_MODULE_ID "freg"

/*定义设备ID*/
#define FREG_HARDWARE_DEVICE_ID "freg"

/*自定义 freg模块结构体*/
struct  freg_module_t
{
	struct hw_module_t common;  //hardware.h
};

/*自定义 freg设备结构体*/
struct freg_device_t
{
	struct hw_device_t common;
	int fd;
	int (*set_val)(struct freg_device_t* dev, int val);
	int (*get_val)(struct freg_device_t* dev, int* val);
};

__END_DECLS
#endif