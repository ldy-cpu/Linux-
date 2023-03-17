#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#define  BEGIN  300000
#define  END    300200
#define  THR_NUM  8

struct arg{
    int n;
};

pthread_t thr[THR_NUM];
pthread_mutex_t lck;
pthread_cond_t conds,condx;
int cur;
int has;

void clean(void*a){
    free(a);
}

void *primer(void*arg){
    int tag;
    int acp;
    while(1){
        // pthread_mutex_lock(&lck);
        pthread_cond_wait(&condx,&lck);
        printf("!!");
        if(has == 0){
            acp = cur;
            has = 1;
            printf("##");
            pthread_cond_broadcast(&conds);
            pthread_mutex_unlock(&lck);
            tag = 0;
            for(int p = 2;p<=(int)sqrt((double)(acp));p++){
                for(int q = p;q<acp;q++){
                    if(p*q == acp){
                        tag = 1;
                        break;
                    }
                }
                if(tag == 1)
                    break;
            }
            if(tag == 0){
                printf("%d  ",acp);
            }
            
            continue;
        }
        else{
            
            pthread_cond_broadcast(&conds);
            pthread_mutex_unlock(&lck);
            if(cur == END){
                
                
                break;
                
            }
            // else
            //     {
            //         // pthread_mutex_unlock(&lck);
                    
            //     }
                
        }
            
        
    }
    
    

    pthread_exit(NULL);
}

void at_exit(void){
    for(int i = 0;i<THR_NUM;i++){
        pthread_join(thr[i],NULL);
        // printf("\ncollect %d\n",i);
    }
}

int main(){
    
    int err;
    cur = BEGIN;
    has = 0;
    pthread_mutex_unlock(&lck);
    for(int i = 0;i<THR_NUM;i ++){
        err = pthread_create(thr+i,NULL,primer,NULL);
        // printf("\n%d\n",i);
        
        if(err){
            perror("pthread_create()");
            exit(1);
        }
    }
    pthread_cond_broadcast(&condx);
    

    while(1){
        // pthread_mutex_lock(&lck);
        pthread_cond_wait(&conds,&lck);
        if(cur == END){
            // pthread_mutex_unlock(&lck);
            break;
        }
        if(has == 1){
            cur += 1;
            has = 0;
            pthread_cond_broadcast(&condx);
        }
        
        // pthread_mutex_unlock(&lck);

    }
    atexit(at_exit);
    pthread_mutex_destroy(&lck);
    pthread_cond_destroy(&condx);
    pthread_cond_destroy(&conds);
    // printf("done!");
    exit(0);

}