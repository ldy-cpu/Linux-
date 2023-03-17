#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<pthread.h>


typedef void (*sighandler_t)(int);


void *newthr(){
    for(int i = 0;i<10;i++){
        write(1,"!",1);
        sleep(2);
    }
    // return NULL;
    pthread_exit(NULL);
}
int tag;
sigset_t set;
sighandler_t handler(int s){
    pthread_t tid;
    if(tag == 1)
        return NULL;
    tag = 1;
    int p;
    p = pthread_create(&tid,NULL,newthr,NULL);
    if(p != 0){
        perror("pthread_create()");
    }

}



int main(){
    
    signal(SIGINT,handler);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    tag = 0;


    while(1){
        for(int i = 0;i<5;i++){
            write(1,"*",1);
            sleep(1);
        }
        write(1,"\n",1);
    }
    exit(0);
}