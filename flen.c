#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>



long long flen(char* name){
    struct stat state;
    stat(name,&state);   
    return state.st_size;
}


int main(int argc,char** argv){
    if(argc != 2){
        perror("should be ./flen ***");
        exit(1);
    }
    int fd;
    fd = open(argv[1],O_RDONLY);
    if(fd<0){
        perror("open");
        exit(1);
    }
    printf("length of %s is %lld",argv[1],flen(argv[1]));
    exit(0);
}