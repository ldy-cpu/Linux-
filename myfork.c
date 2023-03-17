#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    pid_t pid;
    printf("[%d]is begin\n",getpid());

    pid = fork();
    if(pid<0){
        perror("fork:");
        exit(1);
    }

    if(pid == 0){
        sleep(1);
        printf("child id:[%d]\n",getpid());

    }
    else{
        // sleep(1);
        printf("parents id:[%d]\n",getpid());
    }

    printf("[%d]is terminate\n",getpid());

    getchar();
    exit(0);


}