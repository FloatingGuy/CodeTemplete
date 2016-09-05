/*
* @Author: LK_mac
* @Date:   2016-08-17 17:29:30
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-17 20:39:43
*/

#define LOG_TAG "IFregService"
#include <utils/Log.h>
#include "IFregService.h"

using namespace android;

enum 
{	
	GET_VAL = IBinder::FIRST_CALL_TRANSACTION,
	SET_VAL
};

class  BpFregService: public BpInterface<IFregService>
{
public:
	 BpFregService(const sp<IBinder>& impl)
	 	:BpInterface<IFregService>(impl)
	 	{
	 	}
public:
	int32_t getVal()
	{
		Parcel data;
		data.writeInterfaceToken(IFregService::getInterfaceDescriptor());

		Parcel reply;
		//调用BpRefBase的成员remote获取BpBinder代理对象
		remote()->transact(GET_VAL, data, &reply);   //data引用，reply指针
		int32_t val = reply.readInt32();
		return val;
	}

	void setVal(int32_t val)
	{
		Parcel data;
		data.writeInterfaceToken(IFregService.getInterfaceDescriptor());
		data.writeInt32(val);

		Parcel reply;
		remote->transact(SET_VAL, data, &reply);
	}
};

IMPLEMENT_META_INTERFACE(FregService, "liukun.FregService");

status_t BnFregService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
	switch(code)
	{
		case GET_VAL:
		{
			CHECK_INTERFACE(IFregService, data, reply);
			int32_t val = getVal();		//sub class method
			reply->writeInt32(val);
			return NO_ERROR;
		}
		case SET_VAL:
		{
			//检查通信的合法性。检查请求来自 FregService代理对象，检查data Parcel对象的第一个数据是否是描述符"liukun.FregService"
			CHECK_INTERFACE(IFregService, data, reply);
			setVal(data.readInt32());
			return NO_ERROR;
		}
		default:
		{
			return BBinder::onTransact(code, data, reply, flags);
		}
	}
}




