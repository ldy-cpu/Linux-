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
void clean(void*a){
    free(a);
}

void *primer(void*arg){
    int tag;

    for(int j = BEGIN+((struct arg*)arg)->n;j<=END;j+=THR_NUM){
        tag = 0;
        for(int p = 2;p<(int)sqrt((double)j);p++){
            for(int q = 2;q<j;q++){
                if(p*q == j){
                    tag = 1;
                    break;
                }
            }
            if(tag == 1)
                break;
        }
        if(tag == 0){
            printf("%d  ",j);
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
    for(int i = 0;i<THR_NUM;i ++){
        struct arg *now = malloc(sizeof(struct arg));
        now->n = i;
        err = pthread_create(thr+i,NULL,primer,now);
        free(now);
        // printf("\n%d\n",i);
        
        if(err){
            perror("pthread_create()");
            exit(1);
        }
    }
    atexit(at_exit);
    // printf("done!");
    exit(0);

}