//tcp_client.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define SERVER_PORT 60000          //-->1024 ~ 65535
#define SERVER_IP "192.168.124.32"  //-->服务器的IP
 
/*
    TCP的客户端的代码实现步骤
    1、建立套接字
    2、绑定（IP和端口号）（可有可无）
    3、发送连接请求（connect）
    4、发送数据（send，write）
    5、关闭
*/
 
int main(int argc,char** argv)
{
    //手动输入端口号和IP地址
    // if(argc != 3)
    // {
    //     perror("./a.out ip port");
    //     return -1;
    // }
 
    int ret = 0;
    char buf[1024] = { 0 };
 
    //1、建立套接字
    //参数：              地址族 流式套接字 默认协议
    int socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd == -1)
    {
        perror("socket fail");
        return -1;
    }
 
    //2、填充服务端结构体的端口和IP
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //server_addr.sin_port = htons(atoi(argv[2]));              //传参方式
    server_addr.sin_port = htons(SERVER_PORT);        //宏定义方式
    //server_addr.sin_addr.s_addr = inet_addr(argv[1]);   //传参方式
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);   //宏定义方式
 
    //3、连接服务器
    //参数   套接字文件描述符  IP和端口号（旧结构体指针强转取地址）新结构体的大小
    ret = connect(socketfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("connect fail");
        return -1;
    }
    printf("连接服务器成功[%s][%d]\n",SERVER_IP,SERVER_PORT);
    //printf("连接服务器IP:%s 端口号PORT:%hu\n",argv[1],atoi(argv[2]));
 
    //4、与服务器之间发送数据
    while(1)
    {
        //缓存区清零    
        bzero(buf,sizeof(buf));
 
        //发送数据，计算返回值（buf真实数据大小）
        scanf("%s",buf);
 
        //参数  套接字文件描述符 缓存区 缓存区真实大小 默认属性
        //ret = send(socketfd,buf,strlen(buf),0);
        ret = write(socketfd,buf,strlen(buf));
        if(ret  == -1)
        {
            printf("服务端掉线，客户端发送数据失败。。。\n");
            return -1;
        }
 
        printf("发送成功 ret:%d\n",ret);
 
        //做主动退出的判断条件
        if(!strcmp(buf,"exit"))
            break;
    }
 
    //关闭套接字
    close(socketfd);
 
    return 0;
}