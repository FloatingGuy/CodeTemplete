/*
* @Author: LK_mac
* @Date:   2016-08-17 17:30:47
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-17 21:15:15
*/
#define LOG_TAG "FregClient"

#include <utils/Log.h>
#include <binder/IServiceManager.h>

#include "../common/IFregSerice.h"

int main(int argc, char const *argv[])
{
	/* code */
	sp<IBinder> binder = defaultServiceManager()->getService(String16(FREG_SERVICE));
	if (binde == NULL)
	{
		LOGE("Failed to get freg service : %s.\n", FREG_SERVICE);
		return -1;
	}
	sp<IFregSerice> service = IFregSerice::asInterface(binder);
	if (service == NULL)
	{
		LOGE("Failed to get freg service interface.\n");
	}
	printf("Read original value from FregService:\n");
	
	int32_t val = service->getVal();
	printf("%d\n", val );
	printf("Add value 1 to FregService\n");

	val += 1;
	service->setVal(val);

	printf("Read the vaule from FregService again:\n");

	val = service->getVal();
	printf("%d\n", val);
	return 0;
}

