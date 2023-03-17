#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#define FNAME "/tmp/out"
int main(){
    int fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
    dup2(fd,1);
    if(fd != 1){
        close(fd);
    }
    puts("Hello World !");
    fflush(stdout);
    close(1);
    exit(0);
}