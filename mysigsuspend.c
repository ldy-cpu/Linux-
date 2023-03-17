#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <signal.h>
#define  MYRTSIG    (SIGRTMIN+1)

typedef void (*sighandler_t)(int);


sighandler_t inthandler(int){

    write(1,"! ",1);
}

int main(){

    sigset_t new,old;
    sigemptyset(&new);

    signal(SIGALRM,SIG_IGN);
    signal(SIGINT,inthandler);

    sigaddset(&new,MYRTSIG);



    while(1){
        sigprocmask(SIG_UNBLOCK,&new,NULL);
        sigprocmask(SIG_BLOCK,&new,&old);
        for(int i = 0;i<5; i++){
            write(1,"* ",1);
            sleep(1);
        }

        // sigprocmask(SIG_SETMASK,&old,NULL);
        // pause();

        sigsuspend(&old);

        write(1,"\n",1);

    }   
    exit(0);

}