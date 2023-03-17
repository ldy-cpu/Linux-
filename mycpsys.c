#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>



#define BUFSIZE 1024
int main(int argc,char**argv){
    int sfd,dfd;
    if(argc!=3){
        perror("should be ./mycpsys *** ***");
        exit(1);
    }
    sfd = open(argv[1],O_RDONLY);
    if(sfd<0){
        perror("open");
        exit(1);
    }
    dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(dfd < 0){
        perror("open");
        close(sfd);
        exit(1);
    }
    char buf[BUFSIZE];
    int len;
    while((len = read(sfd,buf,BUFSIZE))>0){
        int now = write(dfd,buf,len);
        if(now<0){
            perror("write");
            break;
        }
        len = len - now;
        while(len>0){
            now = write(dfd,buf,len);
            len -= now;
        }
    }
    if(len < 0){
        perror("read");
    }

    close(dfd);
    close(sfd);

    exit(0);    
}