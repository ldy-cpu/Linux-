#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc,char**argv){
    FILE *fp;
    if(argc != 2){
        perror("should be ./file_size ***");
        exit(1);
    }
    fp = fopen(argv[1],"r");
    if(fp == NULL){
        perror("fopen");
        exit(1);
    }
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    printf("file length is: %ld",size);
    exit(0);
}