#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/wait.h>
#include<sys/types.h>
#define LEFT 30000000
#define RIGHT 30200000
#define PRO_NUM 6


int main(){
    int tag,pid,wstatus;
    for(int p = 0;p <= PRO_NUM - 1;p++){  //创建几个子进程
        pid = fork();
        if(pid < 0){
            perror("fork:");
            wait(&wstatus);
            exit(1);
        }
        if(pid != 0)
            continue;
        else{
            for(int i = LEFT + p;i<=RIGHT;i += PRO_NUM){
                tag = 0;
                for(int j = 2;j<=(int)sqrt(i);j++){
                    if(i%j == 0){
                        tag = 1;
                        break;
                    }
                }
                if(tag == 0){
                    printf("%d  ",i);
                }
            }
            exit(0);
        }
    }
    for(int i = 0;i<=PRO_NUM - 1;i++){
        wait(&wstatus);
    }
    
    printf("\n");
    exit(0);

}