#include <stdio.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include<pthread.h>
typedef struct
{
    char type;      //消息类型 L C Q
    char id[32];    //用户id
    char text[128]; //消息内容
} msg_t;
 
//链表节点
typedef struct node_t
{
    struct sockaddr_in caddr;
    // 指针的基类型是node_t, *用来表示变量next是指针
    struct node_t *next; 
} list;
 
 struct sockaddr_in saddr, caddr;
 
//创建头节点 
list *list_create(void)
{
    // L=(SqList *)malloc(sizeof(SqList));
    // L指的是SqList *的指针，指向名为SqList的结构体
    // malloc(sizeof(SqList))指的是向系统内存申请大小为sizeof(SqList)的内存地址
    // (SqList *)指的是把这个地址强制转化为SqlList *的指针
    list *p = (list *)malloc(sizeof(list));
    if (p == NULL)
    {
        perror("malloc err.\n");
        return NULL;
    }
    p->next = NULL;
    return p;
}
 
void login(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr);
void chat(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr);
void quit(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr);
 
 void * handler(void *arg)
{
    int socketfd = *(int *)arg;
    msg_t msg_s;
    strcpy(msg_s.id,"server");
    while (1)
    {
        scanf("%[^\n]s",msg_s);
        getchar();
        sendto(socketfd,msg_s.text,sizeof(msg_s),0,  (struct sockaddr *)&caddr, sizeof(caddr));
    }
    
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("usage:./a.out <port>\n");
        return -1;
    }
 
    int sockfd;
    struct sockaddr_in saddr, caddr;
    socklen_t len = sizeof(caddr);
    msg_t msg;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("sock err.\n");
        return -1;
    }
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    saddr.sin_port = htons(atoi(argv[1]));
    bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    printf("bind ok!\n");
 
    list *p = list_create();
    int recvbyte;
    pthread_t tid;
    pthread_create(&tid,NULL,handler,&sockfd);
    pthread_detach(tid);
 
    while (1)
    {
        struct timeval val={2000,0};
        setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&val,sizeof(val));
        recvbyte = recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&caddr, &len);
        if (recvbyte < 0)
        {
            perror("recvfrom err.\n");
            return -1;
        }
 
        if (msg.type == 'L') //登入
        {
            login(sockfd, msg, p, caddr);
        }
        else if (msg.type == 'C') //聊天
        {
            chat(sockfd, msg, p, caddr);
        }
        else if (msg.type == 'Q') //退出
        {
            printf("type :%c\n", msg.type);
            printf("ip:%s pord:%d id:%s\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), msg.id);
            printf("text:%s\n", msg.text);
            quit(sockfd, msg, p, caddr);
        }
    }
    close(sockfd);
    return 0;
}
 
void login(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr)
{
    list *new = NULL;
    sprintf(msg.text, "login");
 
    while (p->next != NULL)
    {
        p = p->next; //发送给的其他用户 登入消息
        sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&(p->caddr), sizeof(p->caddr));
        printf("%s\n", msg.text);
    }
    //将新用户信息保存 尾插到链表
    new = (list *)malloc(sizeof(list));
    new->caddr = caddr;
    new->next = NULL;
    p->next = new;
    return;
}
 
void chat(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr)
{
    int n;
    while (p->next != NULL)
    {
        p = p->next;
        n = memcmp(&(p->caddr), &caddr, sizeof(caddr));
        if (n != 0)
        {
            sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&(p->caddr), sizeof(p->caddr));
        }
    }
    return;
}
 
void quit(int sockfd, msg_t msg, list *p, struct sockaddr_in caddr)
{
    list *dele;
    sprintf(msg.text, "logout");
    while (p->next != NULL)
    {
        if ((memcmp(&(p->next->caddr), &caddr, sizeof(caddr))) == 0) //找要释放的前一个节点
        {
            dele = p->next;
            p->next = dele->next;
            free(dele);
            dele = NULL;
        }
        else
        {
            p = p->next;
            sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&(p->caddr), sizeof(p->caddr));
        }
    }
    return;
}
