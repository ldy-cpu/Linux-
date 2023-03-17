#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <sys/wait.h>

typedef void (*sighandler_t)(int);


sighandler_t handler(int){
    printf("new");
    fflush(stdout);
}


int main(){
    int pipefd[2];

    pipe(pipefd);

    int pid;
    pid = fork();
    if(pid == 0){
        int fp;
        fp = open("/tmp/out",O_WRONLY|O_TRUNC,0600);
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set,SIGALRM);
        sigprocmask(SIG_UNBLOCK,&set,NULL);

        signal(SIGALRM,handler);
        char* r = malloc(sizeof(char)*1024);
        int len;
        while(1){
            pause();
            // sigwait(&set,NULL);这个不行
            len = read(pipefd[0],r,1024);
            write(fp,r,len);
            if(strncmp(r,"END",3) == 0){
                break;
            }
        }
        close(fp);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }
    else{
        signal(SIGALRM,SIG_IGN);
        int a = 1024;
        char* r = malloc(sizeof(char)*1024);
        int len;
        while(1){
            
            len = getline(&r,&a,stdin);
            write(pipefd[1],r,len);
            // printf("\n1 line\n");
            // fflush(stdout);
            // alarm(1);这个不行
            
            kill(pid, SIGALRM);

            // printf("%d",strncmp(r,"END",3));
            // fflush(stdout);
            if(strncmp(r,"END",3) == 0){
                break;
            }

        }
        close(pipefd[0]);
        close(pipefd[1]);

        wait(NULL);
        exit(0);
    }
}