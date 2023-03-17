#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
int main(int argc,char**argv){
    if(argc != 2){
        perror("should be ./big ***");
        exit(1);
    }
    int fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC);
    if(fd<0){
        perror("open");
        exit(1);
    }
    lseek(fd,2L*1024L*1024L*1024L-1,SEEK_SET);

    write(fd,"",1);
    exit(0);

}