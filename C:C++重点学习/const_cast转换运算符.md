##const_cast 转换运算符学习

>[C++标准转换运算符const_cast](http://www.cnblogs.com/ider/archive/2011/07/22/cpp_cast_operator_part2.html)

###C++错误写法
```C++
const int constant = 21;
int* modifier = &constant;   // <1> invalid conversion from 'const int*' to 'int*'

int& modifier = constant;   // <2> invalid initialization of reference of type 'int&' from expression of type 'const int'
```
上述写法<1> 在C语言中是可以通过的。。。。。

###正确写法
```C++
const int* const_p = &constant;
int* modifier = const_cast<int*>(const_p);
*modifier = 7;
```

###传统写法
```C++
const int constant = 21;
const int* const_p = &constant;
int* modifier = (int*)(const_p);
```

###总结：
   上述给modifier赋值的写法在C++ 中被称为
"未定义行为(Undefined Behavior)".所谓未定义，是说这个语句在标准C++中没有明确的规定，由编译器来决定如何处理。
   对于未定义行为，我们所能做的所要做的就是避免出现这样的语句。对于const数据我们更要这样保证：
绝对不对const数据进行重新赋值。


###重点：
如果我们不想修改const变量的值，那我们又为什么要去const呢？

**原因1**: 我们可能调用了一个参数不是const的函数，而我们要传进去的实际参数确实const的，但是我们知道这个函数是不会对参数做修改的。于是我们就需要使用const_cast去除const限定，以便函数能够接受这个实际参数。

**原因2**: const对象想调用自身的非const方法的时候，因为在类定义中，const也可以作为函数重载的一个标示符。

**原因3**: 我们定义了一个非const的变量，但用带const限定的指针去指向它，在某一处我们突然又想修改了，可是我们手上只有指针，这时候我们可以去const来修改了。上边的代码结果也证实我们修改成功了。
```C++
#include <iostream>
using namespace std;

void f(int* p) {
  cout << *p << endl;
}

int main(void) {
  const int a = 10;
  const int* b = &a;

  // Function f() expects int*, not const int*
  //   f(b);
  int* c = const_cast<int*>(b);
  f(c);

  // Lvalue is const
  //  *b = 20;

  // Undefined behavior
  //  *c = 30;

  int a1 = 40;
  const int* b1 = &a1;
  int* c1 = const_cast<int*>(b1);

  // Integer a1, the object referred to by c1, has
  // not been declared const
  *c1 = 50;

  return 0;
}
```

