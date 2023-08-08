// https://blog.csdn.net/sakura0908/article/details/129618962
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERVER_PORT 60000          //-->1024 ~ 65535
#define SERVER_IP "192.168.124.32"  //-->虚拟机ip或主机ip
 
/*
    TCP的服务端的代码实现步骤
    1、建立套接字
    2、绑定（IP和端口号）
    3、监听（listen）
    4、阻塞连接（accept）
    5、接收数据（recv，read）
    6、关闭
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
 
    //设置端口复用
    int optval = 1;
    //参数  套接字文件描述符    网络层   端口复用    设置值   设置值的大小
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR,&optval, sizeof(optval));

    // 设置keep-alive
    int keepalive = 1; // 开启keepalive属性
    int idle_time = 120; // 空闲120秒开始检测
    int interval_time = 30; // 每次探测间隔时间为30秒
    int retry_count = 5; // 最多重试5次
    
    setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive , sizeof(int));
    // mac不支持
    //setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPIDLE, &idle_time , sizeof(int));
    //setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPINTVL, &interval_time , sizeof(int));
    //setsockopt(socketfd, IPPROTO_TCP, TCP_KEEPCNT, &retry_count , sizeof(int));
 
    
    //2、填充服务端结构体的端口和IP
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //server_addr.sin_port = htons(atoi(argv[2]));              //传参方式
    server_addr.sin_port = htons(SERVER_PORT);                  //宏定义方式
    //server_addr.sin_addr.s_addr = inet_addr(argv[1]);         //传参方式
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);         //宏定义方式
 
    //3、绑定
    //参数  套接字文件描述符 IP和端口号（旧结构体指针强转取地址）新结构体的大小
    ret = bind(socketfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
    if(ret == -1)
    {
        perror("bind fail");
        return -1;
    }
 
    //4、监听
    //参数 套接字文件描述符 支持的客户端连接最大数量
    ret = listen(socketfd,20);
    if(ret == -1)
    {
        perror("bind fail");
        return -1;
    }
 
    //5、阻塞等待客户端连接
    //printf("绑定服务器IP:%s 端口号PORT:%hu\n",argv[1],atoi(argv[2]));
    printf("绑定服务器IP:%s 端口号PORT:%hu\n",SERVER_IP,SERVER_PORT);
 
    struct sockaddr_in client_addr;
    int len = sizeof(struct sockaddr_in);
    //参数           套接字文件描述符 IP和端口号（旧结构体指针强转取地址）长度取址
    //返回新的套接字文件描述符
    int newClientfd = accept(socketfd,(struct sockaddr*)&client_addr,&len);
 
    //6、拿到客户端的地址和端口号
    char* ip = inet_ntoa(client_addr.sin_addr);
    int port = ntohs(client_addr.sin_port);
    printf("客户端的IP:%s 端口号：%d\n",ip,port);
 
    //7、接收数据
    while(1)
    {
        //缓存区清零
        bzero(buf,sizeof(buf));
 
        //接收数据，计算返回值（buf真实数据大小）
        //参数  套接字文件描述符 缓存区 缓存区大小 默认属性
        //使用返回的新套接字文件描述符
        //ret = recv(newClientfd,buf,sizeof(buf),0);
        ret = read(newClientfd,buf,sizeof(buf));
        // 客户端掉线，会进行4次挥手，发送的挥手报文数据长度为0
        if(ret == 0)
        {
            printf("客户端掉线，服务器退出。。。\n");
            return -1;
        }
 
        printf("收到数据:%s 大小:%d\n",buf,ret);
 
        //做主动退出的判断条件
        if(!strcmp(buf,"exit"))
            break;
 
    }
 
    //8、关闭套接字
    close(socketfd);
    close(newClientfd);
 
    return 0;
}
 