#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// 父进程死亡，子进程被init进程收养
int main(){
    pid_t pid;
    int x = 1;

    pid = fork();

    // 子进程
    if(pid == 0){
        sleep(10);
        printf("child: x=%d\n", ++x);
        exit(0);
    }
    sleep(5);
    // 主进程
    printf("parent: x=%d\n", --x);
    exit(0);
}