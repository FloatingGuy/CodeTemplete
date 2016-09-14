/*
* @Subject:
* @Model:
* @Compile-cmd: gcc -Wall
*
* @Author:    liukun
* @Date:      2016-09-14 14:23:18
* @Email:     liukun@antiy.cn
* @File Path: /Users/LK_mac/Study/codeTemplete/Linux/C:C++重点学习/指针和数组的区别/demo3.c
* @Create Time: 2016-09-14 14:23:18
* @Last Modified by:   liukun
* @Last Modified time: 2016-09-14 14:26:10
* @Reference:
*
* 指针数组 和 数组指针的区别?
* 	指针数组：首先它是一个数组，数组的元素都是指针，数组占多少个字节由数组本身决定。它是“储存指针的数组”的简称。
*	数组指针：首先它是一个指针，它指向一个数组。在32 位系统下永远是占4 个字节，至于它指向的数组占多少字节，不知道。它是“指向数组的指针”的简称。
*
* 重点：
* 	'*' 和 '[]' 运算符 优先级比较。 '[]'优先级更高
*/
#include <stdio.h>

typedef struct
{
	int* a;
	int arr[5];
	int *parr[5];
} Arr;

// 数组指针
//int (*p)[10];
int arr_point() {
	char a[5] = {'a', 'b', 'c'};
	char (*p3)[5] = &a;
	char (*p5)[5] = (char(*)[])a;

	printf("&a=%x, a=%x\n", &a, (unsigned int)a);
	printf("%p\n", a);
	return 0;

}

// 指针数组
// int *p[10]
void point_arr() {

}

int main() {

	int a = 5;
	Arr* pArr = new Arr;
	int* t = &a;
	pArr->parr[0] = t;
	pArr->arr[0] = 2;
	printf("pArr->parr[0]=%d\n", *(pArr->parr[0]));
	printf("pArr->arr[0]=%d\n", pArr->arr[0]);

    return 0;
}