#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
int main(int argc,char**argv){
    
    if(argc != 2){
        perror("should be ./line_num ***");
        exit(1);
    }
    FILE *fp;
    fp = fopen(argv[1],"r");
    if(fp == NULL){
        perror("fopen");
        exit(1);
    }
    int line_num = 0;
    char* p;
    long cnum;
    while(getline(&p,&cnum,fp) != -1){
        printf("char num = %d\n",(int)strlen(p));
        printf("buffer length = %ld\n",cnum);
        line_num++;
    }
    printf("line num = %d\n",line_num);

    fclose(fp);
}