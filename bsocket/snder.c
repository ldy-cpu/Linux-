#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include"proto.h"

#define  IPSTRSIZE  40


int main(){
    int sfd;
    struct sockaddr_in laddr,raddr;
    socklen_t llen,rlen;
    struct msg_st lbuf;

    sfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sfd<0){
        perror("socket()");
        exit(1);
    }
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SNDPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);

    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,"255.255.255.255",&raddr.sin_addr);


    llen = sizeof(laddr);
    if(bind(sfd,(void*)&laddr,llen)<0){
        perror("bind()");
        exit(1);
    }
    rlen = sizeof(raddr);
    uint32_t a;
    a = 0;
    uint32_t b;
    b = 99;
    int optval = 1;
    socklen_t optlen = sizeof(optval);
    setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&optlen,optlen);

    while(1){
        sleep(1);
        printf("send");
        fflush(stdout);
        strcpy(lbuf.name,"holly shit");
        lbuf.math = htonl(a);
        lbuf.chinese = htonl(b);
        sendto(sfd,(void*)&lbuf,sizeof(lbuf) + strlen(lbuf.name)+1,0,(void*)&raddr,rlen);
        a = a + 1;
        b = b -1;
    }

    close(sfd);
    exit(0);
    


}