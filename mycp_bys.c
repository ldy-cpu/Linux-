#define BUFSIZE 1024
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>


int main(int argc,char** argv){
    char buf[BUFSIZE];
    if(argc<3){
        perror("should be ./mycp_bys *** ***");
        exit(1);
    }
    FILE *src,*dest;
    src = fopen(argv[1],"r");
    if(src == NULL){
        perror("src");
    }
    dest = fopen(argv[2],"w");
    if(dest == NULL){
        perror("dest");
    }
    while(fgets(buf,BUFSIZE,src) != NULL){
        fputs(buf,dest);
    }
    fclose(dest);
    fclose(src);
    exit(0);
    
}
