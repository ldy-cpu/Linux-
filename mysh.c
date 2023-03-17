#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include<crypt.h>
#include <shadow.h>


void prompt(void){
    printf("mysh-$ :");
}


char** parse(char *line){
    char* token;
    char**param = malloc(sizeof(char*)*100);
    int i = 0;
    while(1){
        token = strsep(&line," \t\n");
        if(token == NULL){
            break;
        }
        if(token[0] == '\0')
            continue;
        param[i] = token;
        i++;
    }
    param[i] = NULL;
    return param;
}


int main(){
  

    int wstatic;
    pid_t pid;

    size_t n = 0;

    while(1){
        int tag = 0;
        prompt();
        char* lp = NULL;
        if(getline(&lp,&n,stdin)<0){
            perror("getline()");
            free(lp);
            exit(1);
        }

        //////////////////////////////////////

        char** param = parse(lp);
        if(strcmp(param[0],":q") == 0){
            printf("quit!\n");
            break;
        }

        if(strcmp(param[0],"sudo") == 0){
            tag = 1;
            // setuid(0);
            printf("enter root passwd: ");
            char* pwd,* hpwd;
            struct spwd *info = getspnam("ldy");
            if(info == NULL){
                printf("\nno such user\n");
                continue;
            }
            int m = 0;
            pwd = getpass("please enter passwd:");

            hpwd = crypt(pwd,info->sp_pwdp);
    
            if(strcmp(info->sp_pwdp,hpwd) == 0){
                
                param = &param[1];
         
            }
            else{
                printf("wrong passwd!!\n");
                continue;
            }
            
        }
            
        pid = fork();
        if(pid < 0){
            perror("fork()");
            free(lp);
            exit(1);
        }
        if(pid == 0){
            if(tag == 0)
                setuid(1000);
            execvp(param[0],param);
            perror("exec()");
            exit(1);
        }
        if(pid > 0){
            wait(&wstatic);
            if(!WIFEXITED(wstatic)){
                perror("wait()");
                exit(1);
            }
        }
    }
    exit(0);

    


}