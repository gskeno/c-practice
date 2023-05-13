#include<stdio.h>

// 指针未初始化
// 正常编译，无运行结果
// void test1(){
//     int *p;
//     *p = 10;

//     printf("p=%p", p);
// }


// 数组指针越界
void test2(){
    int arr[10] = {0};
    // p指针指向arr数组的首元素
    // 写法错误
    // warning: incompatible integer to pointer conversion initializing 'int *' 
    // with an expression of type 'int'; take the address with & [-Wint-conversion]
    // 不兼容的int转指针
    // int *p = arr[0];

    int *p = arr;
    for (int i = 0; i <= 10; i++)
    {
      *p = i;
      // 指针p++，由于p是指针，只能打印出p内存储的数组元素的地址
      // 由于数组元素是int类型，所以相邻两次打印p存储的地址值相差4，表示4个字节
      // 当i=10时，p指向了数组外，出现野指针
      p++; 
      printf("i=%d, p=%p\n", i, p);
    }
}

int main(){
    // test1();
    test2();
    return 0;
}