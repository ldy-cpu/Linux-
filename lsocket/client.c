#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>

#include"proto.h"

typedef void (*sighandler_t)(int);

int sfd;

sighandler_t handler(int){
    printf("stop connecting!");
    fflush(stdout);
    close(sfd);
    exit(0);
}


int main(){
    
    struct sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&raddr.sin_addr);
    raddr.sin_port = htons(atoi(SPORT));
    struct msg_t sbuf;
    struct msg_t rbuf;

    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(connect(sfd,(void*)&raddr,(socklen_t)sizeof(raddr)) < 0){
        perror("connect()");
        exit(1);
    }
    int i = 1;

    signal(SIGINT,handler);

    // while(1){
    //     sbuf.num = htonl(i);
    //     i++;
    //     // strcpy(sbuf.data,"hello!!");
    //     // send(sfd,&sbuf,sizeof(sbuf) + strlen(sbuf.data) + 1,0);
    //     if(recv(sfd,&rbuf,sizeof(rbuf) + MAXDATA,0)<0){
    //         perror("recv");
    //         exit(1);
    //     }
    //     while(strcmp(rbuf.data,"EOT") != 0){
    //         printf("%s\n",rbuf.data);
    //         fflush(stdout);
    //         if(recv(sfd,&rbuf,sizeof(rbuf) + MAXDATA,0)<0){
    //             perror("recv");
    //             exit(1);
    //         }
    //         // printf("rasdfasdfas");
    //         // fflush(stdout);
    //     }
    //     printf("\nEOT\n");
    //     fflush(stdout);
    // }

    int len;
    sbuf.num = 4;
    sbuf.num = htonl(sbuf.num);
    strcpy(&sbuf.data,"hello!");
    send(sfd,&sbuf,sizeof(sbuf) + strlen(sbuf.data) + 1,0);
    while(1){
        // sleep(2);
        len = recv(sfd,&rbuf.num,sizeof(rbuf.num),0);
        while(len<sizeof(rbuf.num)){
            len += recv(sfd,&rbuf.num+len,sizeof(rbuf.num) - len,0);
        }
        rbuf.num = ntohl(rbuf.num);
        len = recv(sfd,rbuf.data,rbuf.num,0);
        while(len<rbuf.num){
            len += recv(sfd,rbuf.data + len,rbuf.num - len,0);
        }
        // if(strcmp(rbuf.data,"EOT"== 0))
        //     continue;
        
        printf("%s\n",rbuf.data);
        fflush(stdout);
    }


    close(sfd);
    exit(0);


}