#include<stdio.h>
union Data{
    // 1. 内存大小，应是最大基础类型占用字节的整数倍，即double 8 的整数倍
    // 2. 内存大小 >= 最大成员所需字节，这里是9
    // 3. 所以占用字节16
    int i;
    float f;
    char str[9];
    double d;
}data;

union Data1{
    // 1. 内存大小，应是最大基础类型占用字节的整数倍，即4的整数倍
    // 2. 内存大小 >= 最大成员所需字节，这里是20
    int i;
    float f;
    char  str[20];
}data1;

int main(){
    printf("unionData size = %lu\n", sizeof(union Data));
    printf("unionData1 size = %lu\n", sizeof(union Data1));

    return 0;
}