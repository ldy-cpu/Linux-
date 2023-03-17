#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#define TTY4 "/dev/tty11"
#define TTY6 "/dev/tty12"
#define BUFSIZE   1024



enum{
    STATE_R = 1,
    STATE_W,
    STATE_T
};

struct fsm
{
    int state;
    int sfd;
    int dfd;
    int len;
    char buf[BUFSIZE];
};

void driver(struct fsm* this){

    if(this->state == STATE_R){
        this->len = read(this->sfd,this->buf,BUFSIZE);
        // printf("读了：%d",this->len);
        fflush(stdout);
        if(strcmp(this->buf,"QUIT") == 0){
            this->state = STATE_T;
            write(this->sfd,"this TTY has closed!",20);
            return;
        }
        if(this->len > 0)  {
            this->state = STATE_W;
            write(1,this->buf,this->len);
        }
            
    }
    if(this->state == STATE_W){
        write(this->dfd,this->buf,this->len);
        
        this->state = STATE_R;
    }
}

int max(int a,int b){
    if(a>b)
        return a;
    return b;
}

int main(){
    int fd1,fd2;
    fd1 = open(TTY4,O_RDWR);
    fd2 = open(TTY6,O_RDWR);

    struct fsm fsm12,fsm21;

    fsm12.state = STATE_R;
    fsm21.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;
    int fd1_save,fd2_save;
    fd1_save = fcntl(fd1,F_GETFL);
    fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
    fd2_save = fcntl(fd2,F_GETFL);
    fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);
    fd_set rset,wset;
    while(fsm12.state != STATE_T || fsm21.state != STATE_T){
        FD_SET(fd1,&rset);
        FD_SET(fd2,&rset);
        FD_SET(fd1,&wset);
        FD_SET(fd2,&wset);
        if(select(max(fd1,fd1)+1,&rset,&wset,NULL,NULL)<0){
            if(errno == EINTR){
                continue;
            }
            
        }
        driver(&fsm12);
        driver(&fsm21);
    }
    close(fd1);
    close(fd2);
    printf("%d",fsm12.state);

    exit(0);

}