/*
* @Subject: 二维数组 和 指针
* @Model:
* @Compile-cmd: gcc -Wall
*
* @Author:
* @Date:      2016-09-04 18:43:46
* @Email:
* @Create Time: 2016-09-04 18:43:46
* @Last Modified by:   liukun
* @Last Modified time: 2016-09-14 14:20:55
* @Reference:
定义二维数组的原理

概念：
	C 语言只有一维数组
	数组指针
*/
#include <stdio.h>


// 二维数组的地址首地址 --> 数组指针 -->  数组指针的值（一维数组的地址==一维数组首元素的地址）

void initArray(calendar){
	// int calendar[12][31];

}
// 实验代码
int main(int argc, char const *argv[])
{
	int calendar[12][31];

	// initArray(calendar);
	int month;
	for (month=0; month < 12; month++)
	{
		int day;
		for (day = 0; day < 31; day++)
			calendar[month][day] = (month*100+day);
	}

	printf("[]#calendar[4][10]=%ld\n", calendar[4][10]);
	printf("point#calendar[4][10]=%ld\n", *(*(calendar+4)+10));
	printf("5月份 int[31]数组的首地址: \n");
	printf("*(calendar + 4) = %lx\n", *(calendar + 4));
	printf("&calendar[4][0] = %lx\n", &calendar[4][0]);
	printf("calendar[4] = %lx\n", calendar[4]);
	printf("5.1号 = %d\n", *(*(calendar+4)));
	printf("Over!!!\n");

	return 0;
}

/**  理论代码
int main(int argc, char const *argv[])
{

	int calendar[12][31];   // 二维数组
	int (*month)[31];		// 数组指针
	int *p;					// int 指针
	int i;					// int

	month = calendar + 4;   // 获取 calendar 数组中某个元素（数组类型）的地址（数组指针）
	p = calendar[4];		// 等同于 *(calendar + 4);  获取int[31] 数组首元素的地址 （int类型的指针）

	i = calendar[4][7];
	i = *(*(calendar + 4) + 7);

	1.1 号 = *(*(calendar));
	1.2 = *(*(calendar+0)+1);
	2.1 = *(*(calendar + 1));

	// int days[31]的首地址
	*(calendar + 4) == (*(calendar + 4)+0) == &calendar[4][0] == calendar[4] == p == &p[0];
	return 0;
}  */

