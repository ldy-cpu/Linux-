#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include"mytbf.h"
#define  CPS  10
#define  BURST  100
#define  BUFFMAX   1024
typedef void (*sighandler_t)(int);
struct mytbf_st *inited;

static int tag = 0;
static void handle(int s){
    if(tag != 0){
        return;
    }
    mytbf_fetchtoken(inited,CPS,BUFFMAX);
    alarm(1);
    printf("\n");
    fflush(stdout);
}
struct mytbf_st
{
    int cps;
    int burst;
    int token;
};

int main(){
    
    signal(SIGALRM,handle);
    FILE *fp = fopen("./myenv.c","r");
    inited = mytbf_init(CPS,BURST);
    if(fp == NULL){
        perror("open()");
        exit(1);
    }
    FILE *dst = fopen("/tmp/out","w");
    char* buffer = malloc(sizeof(char)*BUFFMAX);
    
    mytbf_fetchtoken(inited,CPS,BUFFMAX);
    int en;
    en = fread(buffer,1,CPS,fp);
    printf("%d",en);
    fflush(stdout);
    en = fwrite(buffer,1,en,dst);
    fflush(dst);
    printf("%d",en);
    fflush(stdout);
    alarm(1);
    mytbf_returntoken(inited,en);
    int b = 0;
    while(en > 0){
        for(int i = 0;i<2119999999;i++){
            b+= 1;
        }
        printf("%d\n",inited->token);
        while(inited->token>0){
            printf("\nshit");
            en = fread(buffer,1,CPS,fp);
            printf("%d",en);
            fflush(stdout);
            fwrite(buffer,1,en,dst);
            fflush(dst);
            mytbf_returntoken(inited,en);
            if(en == 0)
                break;
        }
        
        
    }
    tag = 1;

    fclose(dst);
    fclose(fp);
    free(buffer);
    mytbf_destroy(inited);
    exit(0);
}