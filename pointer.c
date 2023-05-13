#include<stdio.h>

int main(){
    // p指针暂未初始化
    int *p;

    // 定义一个基本类型int变量i
    int i = 10;
    
    // p存储变量i的地址，即根据p的地址内容可以找到i
    p = &i;

    // 前两者打印地址，值相同
    printf("p=%p\n", p);
    printf("&i=%p\n", &i);

    // *p，打印指针p存储的地址 指向目的地(i变量) 的内容 10
    printf("*p=%d\n", *p);

    return 0;
}