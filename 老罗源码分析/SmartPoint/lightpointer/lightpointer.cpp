/*
* @Author: liukun
* @Date:   2016-09-05 15:21:00
* @Last Modified by:   liukun
* @Last Modified time: 2016-09-05 16:15:01
* @
* @Reference:
* 	应用轻量级指针 实例
*   	引用计数器：class LightRefBase （framework/base/include/utils/RefBase.h）
*   	轻量级指针的实现类：class sp
* 知识点：
* 	目录结构：
* 		--external/lightpointer/--
*/

#include <stdio.h>
#include <utils/RefBase.h>

using namespace android;

class LightClass : public LightRefBase<LightClass>
{
public:
	LightClass()
	{
		printf("Construct LightClass Object.\n");
	}

	virtual ~LightClass()
	{
		printf("Destroy LightClass Object\n");
	}
};

int main(){
	LightClass* pLightClass = new LightClass();
	sp<LightClass> lpOut = pLightClass;

	printf("Light Ref Count: %d.\n", lpOut.get()->getStrongCount());

	{
		sp<LightClass> lpInner = lpOut;
		printf("Light Ref Count: %d.\n", pLightClass->getStrongCount());
	}
	printf("Light Ref Count: %d.\n", lpOut.get()->getStrongCount());

	printf("\nlpOut addr is %x\n", &lpOut);
	printf("pLightClass addr is %x\n", pLightClass);
	printf("lpOut.get() addr is %x\n", lpOut.get());
    return 0;

   /** printf

    Construct LightClass Object.
	Light Ref Count: 1.
	Light Ref Count: 2.
	Light Ref Count: 1.

	lpOut addr is beb90ac8
	pLightClass addr is b6c8a008
	lpOut.get() addr is b6c8a008
	Destroy LightClass Object
    */

}