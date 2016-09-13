/*
 * @Author:    liukun
 * @Date:      2016-09-13 09:51:17
 * @Email:     liukun@antiy.cn
 * @File Path: /Volumes/myshare/FloatG/code/android6.0.1/external/Vulnerbility/SmartPoint/weightpointer/weightpointer.cpp
 * @Create Time: 2016-09-13 09:51:17
 * @Last Modified by:   LK_mac
 * @Last Modified time: 2016-09-13 21:51:26
 *
 * 案例：使用强弱指针
 * 步骤：
 * 	1. 创建一个受 强弱引用计数器控制的指针
 *  2. 测试只受强引用计数器控制的弱指针的强弱计数器 变化 && 升级为强指针
 *  3. 测试同时受强、弱引用计数器控制的弱指针的强弱计数器 变化 && 升级为强指针
 *  2. 测试只受强引用计数器控制的弱指针的强弱计数器 变化 && 升级为强指针
*/
#include <utils/RefBase.h>
#include <stdio.h>

#define INITIAL_STRONG_VALUE (1<<28)

using namespace android;

class WeightClass : public RefBase
{
public:
	void printRefCount()
	{
		int32_t strong = getStrongCount();
		weakref_type* ref = getWeakRefs();

		printf("---------------\n");
		printf("Strong Ref Count: %d.\n", (strong == INITIAL_STRONG_VALUE ? 0 : strong));
		printf("Weak Ref Count: %d.\n", ref->getWeakCount());
		printf("---------------\n");

	}
};


class  StrongClass : public WeightClass
{
public:
	StrongClass() {
		printf("Construct StrongClass\n");
	}
	virtual ~StrongClass() {
		printf("Destroy StrongClass\n");
	}
};

class WeakClass : public WeightClass
{
public:
	WeakClass() {
		extendObjectLifetime(OBJECT_LIFETIME_WEAK);
		printf("Construct WeakClass\n");
	}
	virtual ~WeakClass() {
		printf("Destroy WeakClass\n");
	}
};

class ForeverClass : public WeightClass
{
public:
	ForeverClass() {
		// extendObjectLifetime(OBJECT_LIFETIME_MASK);
		printf("Construct ForeverClass\n");
	}
	virtual ~ForeverClass() {
		printf("Destroy ForeverClass\n");
	}
};

void TestStrongClass(StrongClass* pStrongClass)
{
	wp<StrongClass> wpOut = pStrongClass;
	pStrongClass->printRefCount();

	{
		sp<StrongClass> spInner = pStrongClass;
		pStrongClass->printRefCount();
	}
	sp<StrongClass> spOut = wpOut.promote();  //Fail --> 因为StongClass 对象已经被释放了 -->  因为StrongClass 只受强引用计数的影响，不受弱引用计数的影响，而强引用技术为0.
	printf("spOut: %p.\n", spOut.get());
}

void TestWeakClass(WeakClass* pWeakClass)
{
	wp<WeakClass> wpOut = pWeakClass;
	pWeakClass->printRefCount();
	{
		sp<WeakClass> spInner = pWeakClass;
		pWeakClass->printRefCount();
	}
	sp<WeakClass> spOut = wpOut.promote();   //success  受弱引用计数的影响
	printf("spOut: %p.\n", spOut.get());
}

void TestForeverClass(ForeverClass* pForeverClass)
{
	wp<ForeverClass> wpOut = pForeverClass;
	pForeverClass->printRefCount();
	{
		sp<ForeverClass> spInner = pForeverClass;
		pForeverClass->printRefCount();

		printf("\n\n");
		sp<ForeverClass> spInnerTmp = wpOut.promote();   //success  受强引用计数，并且强引用计数不为0
		pForeverClass->printRefCount();
		printf("spInnerTmp: %p.\n", spInnerTmp.get());
	}
}


int main(){
	printf("Test Strong Class\n");
	StrongClass* pStrongClass = new StrongClass();
	TestStrongClass(pStrongClass);

	printf("Test Weak Class\n");
	WeakClass* pWeakClass = new WeakClass();
	TestWeakClass(pWeakClass);

	printf("Test Forever Class\n");
	ForeverClass* pForeverClass = new ForeverClass();
	TestForeverClass(pForeverClass);
    return 0;
}