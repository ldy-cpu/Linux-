#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#include"mypipe.h"

#define BEGIN  30000000
#define END    30000200
#define THR_NUM  4
#define  ADD_NUM  16

pthread_t tid[THR_NUM];
pthread_cond_t conds,condx;
pthread_mutex_t mutex;
struct pipe *p;
int endtag;

void *thread_do(void*){
    int nownum;
    int tag;
    while(1){
        pthread_cond_wait(&condx,&mutex);
        if(p->head == p->tail && p->tail->tag == 0){
            if(endtag == 1)
                break;
            
            pthread_cond_signal(&conds);
            pthread_mutex_unlock(&mutex);
            continue;
        }
        nownum = removee(p);
        pthread_cond_broadcast(&condx);
        pthread_mutex_unlock(&mutex);

        tag = 0;
        for(int j = 2;j<=(int)sqrt((double)(nownum));j++){
                for(int q = j;q<nownum;q++){
                    if(j*q == nownum){
                        tag = 1;
                        break;
                    }
                }
                if(tag == 1)
                    break;
        }
        if(tag == 0){
            printf("%d  ",acp);
            fflush(stdout);
        }
    }
        


}

void at_exit(void){
    for(int i = 0;i<THR_NUM;i++){
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conds);
    pthread_cond_destroy(&condx);

}

int main(){
    
    atexit(at_exit);
    
    init(p);
    endtag = 0;
    pthread_cond_init(&conds,NULL);
    pthread_cond_init(&condx,NULL);

    for(int i = 0;i<THR_NUM;i++){
        pthread_create(tid + i,NULL,thread_do,NULL);
    }
    int nownum;
    nownum = BEGIN;

    pthread_cond_signal(&conds);
    pthread_mutex_unlock(&mutex);
    while(nownum<=END){
        pthread_cond_wait(&conds,&mutex);
        p->head->tag = 1;
        p->head->data = nownum;
        nownum++;
        for(int i = 0;i<ADD_NUM-1;i++){
            if(nownum>END)
                break;
            add(nownum,p);
            nownum++;
        }
        pthread_cond_broadcast(&condx);
        pthread_mutex_unlock(&mutex);
    }
    endtag = 1;
    exit(0);
}