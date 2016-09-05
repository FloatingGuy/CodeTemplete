/*
* @Author: liukun
* @Date:   2016-08-31 18:57:44
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-09-01 14:44:58
*
* 目标-- 指针和数组的区别：
* 	1. 数组名不能当做变量来处理，不能取地址（无意义）
* 	2. 指针和数组的分配  大小有变化
* 	3. 空间的分配 ？？？ 没看懂
*
* %lx 只能显示 低4个字节
* 64位机器上  %lx和 %p 显示的结果是一样的
*
* URL：
* 		http://www.cnblogs.com/csyisong/archive/2010/03/18/1688877.html
*/

#include <stdio.h>
#define ARRAY_OR_POINTER 0


static void test1()
{
	char* p1, *p2;
	// char ch[] = {"1","2","3","4","5"};
	char ch[] = {'1','2','3'};
	char** pp;
	p1 = ch;	
	printf("&ch=%lx\n", &(&ch));    //栈上的地址 
	printf("地址: pch[0]=0x%lx, ch=%lx, \n数据: ch[0]=%c, ch[1]=%c\n\n",&ch[0], ch, ch[0], ch[1]);   //%x 只能显示 低4个字节
	

	// pp = &ch;   //bug   test.cpp:19:5: error: cannot convert 'char (*)[12]' to 'char**' in assignment
//ch的内容就是栈上的地址，没办法再获取栈地址的地址 （上面的例子就是 证明）
	// p2 = *pp; 
	// 
	// ======================Begain Output ==================
	// &ch=7fff71434230
	// 地址: pch[0]=0x7fff71434230, ch=7fff71434230,
	// 数据: ch[0]=1, ch[1]=2
	// ======================End Output ==================
	// 
}

static void test2()
{
	int a = 1;
	int* pa = &a;
	int** ppa = &pa;
	int*** pppa = &ppa;
	printf("a=%d, pa = %d, ppa=%d pppa=%d\n", a, pa, ppa, pppa);
	printf("a=%d, pa = %d, ppa=%d pppa=%d\n", a, *pa, *ppa, *pppa);

	// ======================Begain Output ================== 
	// a=1, pa = 1900233252, ppa=1900233256 pppa=1900233264
	// a=1, pa = 1, ppa=1900233252 pppa=1900233256
	// ======================End Output ==================
	// 从输出结果知道： 先定义的变量 先入栈（在栈底，低地址），后定义的变量在高地址（栈顶方向），变化顺序--地址依次增高
}

static int right()
{
	char* p1;
	char* p2;
	char** pp;

	#if ARRAY_OR_POINTER
		char ch[] = "hello, world!\n";
		printf("%d, %d, %d, %d\n", sizeof(p1), sizeof(p2), sizeof(pp), sizeof(ch));
	#else
		char* ch = "hello, world!\n";
		printf("%d, %d, %d, %d\n", sizeof(p1), sizeof(p2), sizeof(pp), sizeof(ch));
	#endif

	p1 = ch;

    #if ARRAY_OR_POINTER
            pp = &p1;
    #else
            pp = &ch;
    #endif

    p2 = *pp;
    if (p1 == p2) {
            printf("p1 equals to p2\n");
    } else {
            printf("p1 doesn't equal to p2\n");
    }

    return 0;
}

int main(){

	test1();

	test2();
	unsigned int a = 0xffffffff;
	unsigned int b = 0x10000004;
	printf("%d, %lx\n", sizeof(int), b);
	printf("%lx\n", (int)(a+b));
    return 0;
}