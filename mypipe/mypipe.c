#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include <unistd.h>


//  #include"mypipe.h"

#define BEGIN  30000000
#define END    30000200
#define THR_NUM  4
#define  ADD_NUM  16

struct node
{
    int tag;
    int data;
    struct node *next;
};


struct pipe{
    struct node *head;
    struct node *tail;
};


void init(struct pipe *p){
    // p = malloc(sizeof(struct pipe));
    struct node *head;
    head = malloc(sizeof(struct node));
    p->head = head;
    p->tail = head;
    head->next = NULL;
    head->tag = 0;
}   


void add(int num,struct pipe *p){
    struct node *now = malloc(sizeof(struct node));
    now->data = num;
    now->next = NULL;
    now->tag = 1;
    p->tail->next = now;
    p->tail = now;
}

int removee(struct pipe *p){
    struct node *del = p->head;
    int num = del->data;
    if(del->next==NULL){
        if(del->tag == 1){
            del->tag = 0;
            return num;
        }
        else{
            return -1;
        }
    }
    
    p->head = del->next;
    
    free(del);
    return num;
}

pthread_t tid[THR_NUM];
pthread_cond_t conds,condx;
// pthread_cond_init(&conds,NULL);
// pthread_cond_init(&condx,NULL);
// conds = PTHREAD_COND_INITIALIZER;
// condx = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
struct pipe *p;
int endtag;

void *thread_do(void*){
    int nownum;
    int tag;
    while(1){

        pthread_cond_wait(&condx,&mutex);
        // pthread_mutex_lock(&mutex);
        if(p->head == p->tail && p->tail->tag == 0){
            if(endtag == 1){
                pthread_cond_signal(&condx);
                pthread_mutex_unlock(&mutex);
                break;
            }
            else{
                pthread_cond_signal(&conds);
                pthread_mutex_unlock(&mutex);
    
                continue;
            }
            
            
        }
        nownum = removee(p);
        pthread_cond_signal(&condx);
        pthread_mutex_unlock(&mutex);

        tag = 0;
        for(int j = 2;j<nownum;j++){
                for(int q = 2;q<nownum;q++){
                    if(j*q > nownum)
                        break;
                    if(j*q == nownum){
                        tag = 1;
                        break;
                    }
                }
                if(tag == 1)
                    break;
        }
        if(tag == 0){
            printf("%d  ",nownum);
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
    p = malloc(sizeof(struct pipe));
    init(p);
    endtag = 0;
    
    pthread_cond_init(&conds,NULL);
    pthread_cond_init(&condx,NULL);
    
    for(int i = 0;i<THR_NUM;i++){
        printf("\ncreate %d\n",i+1);
        fflush(stdout);
        pthread_create(tid + i,NULL,thread_do,NULL);
    }
    sleep(1);
    int nownum;
    nownum = BEGIN;
    // pthread_cond_signal(&conds);
    // pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
    int uuu = 0;
    while(nownum<=END){

        if(uuu == 1){
            pthread_cond_wait(&conds,&mutex);
            // pthread_mutex_lock(&mutex);
        }
        else{
            uuu = 1;
            pthread_mutex_lock(&mutex);
        }
        if(p->head->tag != 0){
            pthread_mutex_unlock(&mutex);
            continue;
        }
   
        p->head->tag = 1;
        
        p->head->data = nownum;
        
        nownum++;
        for(int i = 0;i<ADD_NUM-1;i++){
            if(nownum>END){
                endtag = 1;
                break;
            }
                
            add(nownum,p);
            nownum++;
        }
        
        pthread_cond_signal(&condx);
        pthread_mutex_unlock(&mutex);
    }
    
    exit(0);
}