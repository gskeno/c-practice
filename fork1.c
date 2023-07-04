#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int x = 1;
    // 子进程逻辑
    if(fork() == 0){
        printf("p1: x=%d\n", ++x);
    }
    // 主子进程都会执行
    printf("p2: x=%d\n", --x);
    exit(0);
}