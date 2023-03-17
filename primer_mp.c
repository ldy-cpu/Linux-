#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<sys/types.h>
#define LEFT 30000000
#define RIGHT 30000200
#define PRO_NUM 6


int main(){
    double u;
    int tag = 0;
    int pid;
    int wstatus;

    for(int i = LEFT;i<=RIGHT;i++){
        u = (double)i;
        tag = 0;
        pid = fork();
        if(pid < 0){
            perror("fork:");
            exit(1);
        }
        if(pid == 0){
            for(int j = 2;j<=(int)sqrt(u);j++){
                if(i%j == 0){
                    tag = 1;
                    break;
                }
            }
            if(tag == 0)
                printf("%d  ",i);
            exit(0);
        }
        else{
            continue;
        }
    }
    for(int i = LEFT;i<=RIGHT;i++){
        wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("normally exit,code = %d\n",WEXITSTATUS(wstatus));
        }
        else{
            printf("child exit unormally!!\n");
        }
        
    }
    exit(0);

}