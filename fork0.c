#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    pid_t pid;
    int x = 1;

    pid = fork();

    // 子进程
    if(pid == 0){
        sleep(5);
        printf("child: x=%d\n", ++x);
        exit(0);
    }
    // 主进程
    printf("parent: x=%d\n", --x);
    exit(0);
}