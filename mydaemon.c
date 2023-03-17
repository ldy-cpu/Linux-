#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    pid_t pid  = fork();
    if(pid != 0)
        exit(0);
    FILE* fp;
    int fd = open("/dev/null",O_RDWR);
    if(fd<0){
        fprintf(stderr,"open()");
        fclose(fp);
    }
    fp = fopen("/tmp/out","w");
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd>2)
        close(fd);
    setsid();
    char* a = malloc(sizeof(char)*100);
    for(int i = 0;i<100;i++){
        sleep(3);
        sprintf(a,"line %d\n",i);
        fflush(fp);
        fputs(a,fp);
    }
    fclose(fp);
    exit(0);
    
}