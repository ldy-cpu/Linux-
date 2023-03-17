#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(){
    int fd,pid;
    fd = open("/tmp/out",O_RDWR|O_TRUNC,0600);
    
    char* a;
    a = mmap(NULL,1024,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,fd,0);
    pid = fork();
    if(pid == 0){
        
        strcpy(a,"HELLOworld");
        
        
        munmap(a,1024);
        
        exit(0);
    }
    else{
        
        wait(NULL);
        fflush(stdout);
        puts(a);
        munmap(a,1024);
        exit(0);
    }
}