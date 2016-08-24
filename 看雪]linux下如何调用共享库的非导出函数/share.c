/*
* @Author: LK_mac
* @Date:   2016-08-18 11:24:27
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-18 11:28:02
*/

#include <stdio.h>

void printAB()
{
	printf("ABCD\n");
	sleep(-1);
}

__attribute__((visibility("hidden"))) void printab()
{
	printf("abcd\n");
	sleep(-1);
}

void printIt(int flag)
{
	if (flag)
	{
		printAB();
	}
	else
	{
		printab();
	}
	printf("end!!\n");
}
