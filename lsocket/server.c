#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include<string.h>
#include<signal.h>
#include <unistd.h>


#include"proto.h"

int main(){
    int sfd;
    int newsfd;
    struct sockaddr_in laddr;
    struct sockaddr_in raddr;
    socklen_t len = sizeof(raddr);
    struct msg_t rbuf;
    struct msg_t sbuf;


    sfd = socket(AF_INET,SOCK_STREAM,0);
    laddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    laddr.sin_port = htons(atoi(SPORT));
    bind(sfd,(void*)&laddr,sizeof(laddr));
    if(listen(sfd,10) != 0){
        perror("listen");
        exit(1);
    }
    while(1){
        newsfd = accept(sfd,(void*)&raddr,&len);
        printf("has accept！");
        int pid = fork();
        if(pid == 0){
            int i,lenn;
            lenn = recv(newsfd,&rbuf.num,sizeof(rbuf.num),0);
            while(lenn<sizeof(rbuf.num)){
                lenn += recv(sfd,&rbuf.num+lenn,sizeof(rbuf.num) - lenn,0);
            }
            rbuf.num = ntohl(rbuf.num);

            lenn = recv(newsfd,rbuf.data,MAXDATA,0);
            while(rbuf.data[lenn-1]!='\0'){
                lenn += recv(newsfd,rbuf.data + lenn,MAXDATA,0);
            }
            printf("\n%s\n",rbuf.data);
            fflush(stdout);
            i = rbuf.num;
            while(1){
                // recv(newsfd,&rbuf,sizeof(rbuf) + MAXDATA,0);
                // i = ntohl(rbuf.num);
                sleep(1);
                for(int j = 1;j<=i;j++){
                    for(int p = 1;p<= j;p++){
                        sbuf.num = htonl(i+1);
                        sbuf.data[p-1] = '*';
                        sbuf.data[p] = '\0';   
                    }
                    
                }
                send(newsfd,&sbuf,sizeof(sbuf) + strlen(sbuf.data) + 1,0);
                printf("%d",strlen(sbuf.data));
                fflush(stdout);
                // printf("\none receive!\n");
                // fflush(stdout);
                // strcpy(sbuf.data,"EOT");
                // send(newsfd,&sbuf,sizeof(sbuf) + strlen(sbuf.data) + 1,0);
                // printf("\nsend %s\n",sbuf.data);
                // fflush(stdout);
                i++;
            }
            printf("close connect");
            fflush(stdout);
            close(newsfd);
            close(sfd);
            exit(0);
        }
        else
            close(newsfd);
        
    }
    
    close(sfd);
    exit(0);



}