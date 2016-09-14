/*
 * @Author:    liukun
 * @Date:      2016-09-14 09:19:49
 * @Email:     liukun@antiy.cn
 * @File Path: /Users/LK_mac/Study/codeTemplete/Linux/C:C++重点学习/符号重载/operators_overload.cpp
 * @Create Time: 2016-09-14 09:19:49
 * @Last Modified by:   liukun
 * @Last Modified time: 2016-09-14 09:24:49
*/
#include <iostream>

using namespace std;

class CVector {
public:
	int x,y;
	CVector() {};
	CVector(int a, int b) : x(a), y(b) {}
	//声明 +号的符号重载函数
	CVector operator + (const CVector&);
}

CVector CVector::operator+ (const CVector& param) {
	CVector temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	return temp;
}

int main(int argc, char const *argv[])
{
	CVector foo(3,1);
	CVector bar(1,2);
	CVector result;
	result = foo + bar;
	cout << result.x << ',' << result.y << '\n';
	return 0;
	return 0;
}