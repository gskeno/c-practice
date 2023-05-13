#include<stdio.h>

// 指针未初始化
// 正常编译，无运行结果
void test1(){
    int *p;
    *p = 10;

    printf("p=%p", p);
}

int main(){
    test1();
    return 0;
}