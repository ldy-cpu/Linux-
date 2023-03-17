#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
int main(int argc,char** argv){
    FILE *src;
    FILE *dest;
    src = fopen(argv[1],"r");
    if(src == NULL){
        perror("无原文件");
        exit(1);
    }
    dest = fopen(argv[2],"w");
    if(dest == NULL){
        perror("新建文件失败");
        exit(1);
    }
    int c;
    while((c = fgetc(src)) != EOF){
        fputc(c,dest);
    }

    fclose(dest);
    fclose(src);
    
    exit(0);
}