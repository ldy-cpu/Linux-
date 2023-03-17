#include<stdio.h>
#include<stdlib.h>
#include<crypt.h>
#include<unistd.h>
#include<shadow.h>
#include<string.h>


int main(int argc,char** argv){
    if(argc != 2){
        fprintf(stderr,"should be ./chkpass ***\n");
        exit(1);
    }

    char *ipw,*opw;
    struct spwd * shadowline;
    
    shadowline = getspnam(argv[1]);
    if(shadowline == NULL){
        fprintf(stderr,"No user named %s\n",argv[1]);
        exit(0);
    }
    for(int i = 0;i<3;i++){
        ipw = getpass("password:");
        opw = crypt(ipw,shadowline->sp_pwdp);
        if(strcmp(opw,shadowline->sp_pwdp) == 0){
            printf("Log in Success!\n");
            exit(0);
        }
        else{
            printf("Wrong password!\n");
            continue;
        }
    }
    printf("too many tries\n");
    exit(0);
    
}