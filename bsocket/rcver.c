#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include"proto.h"

#define  IPSTRSIZE  40

int main(){
    int sfd;
    struct sockaddr_in laddr,raddr;
    struct msg_st rbuf;
    char* ipstr;
    ipstr = malloc(IPSTRSIZE);

    socklen_t raddr_len;
    sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if(sfd<0){
        perror("socket()");
        exit(1);
    }


    laddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);/*能够匹配任何地址(当前地址)*/
    laddr.sin_port = htons(atoi(RCVPORT));
    if(bind(sfd,(void*)&laddr,sizeof(laddr))<0)/*因为bind要的是一个sock_addr,而ipv4规定的地址结构体 是sockaddr_in,所以用void转换一下  */
    {
        perror("bind()");
        exit(1);
    }

    // raddr.sin_family = AF_INET;
    // inet_pton(AF_INET,"0.0.0.0",&raddr.sin_addr);/*能够匹配任何地址(当前地址)*/
    // raddr.sin_port = htons(atoi(SNDPORT));

    // raddr_len = sizeof(raddr);
    // alarm(5);
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&optlen,optlen);
    while(1){
        recvfrom(sfd,&rbuf,sizeof(rbuf) + NAMEMAX,0,(void*)&raddr,&raddr_len);
        inet_ntop(AF_INET,&raddr.sin_addr,ipstr,IPSTRSIZE);
        printf("--MESSAGE FROM %s:%d---\n",ipstr,ntohs(raddr.sin_port));
        printf("NAME = %s\n",rbuf.name);
        printf("MATH = %d\n",ntohl(rbuf.math));
        printf("CHINESE = %d\n",ntohl(rbuf.chinese));
        fflush(stdout);
    }
    free(ipstr);
    close(sfd);
    exit(0);
}