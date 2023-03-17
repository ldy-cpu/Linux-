#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<string.h>
#define MAXSIZE  1024

// int main(int argc,char** argv){
//     time_t tim;
//     struct tm *ts;
//     char prt[MAXSIZE];
//     time(&tim);
//     ts = localtime(&tim);
//     strftime(prt,MAXSIZE,"%Y  %m  \%d   %T",ts);
//     printf("%s",prt);
//     exit(0);
// }

int main(int argc,char** argv){
    int c;
    time_t tim;
    struct tm *ts;
    char prt[MAXSIZE];
    time(&tim);
    ts = localtime(&tim);

    char hasRead[MAXSIZE];
    hasRead[0] = '\0';
    FILE *fp = NULL;
    while(1){
        c = getopt(argc,argv,"-H:MSymd");
        if(c == -1)
            break;
        
        switch (c)
        {
            case 1:
                
                fp = fopen(argv[optind-1],"a");
                printf("filename:%s\n",argv[optind-1]);
                if(fp == NULL){
                    perror("open");
                }
            case 'H':
                if(strcmp(optarg,"12") == 0){
                    strncat(hasRead,"hour:%I(\%p)  ",MAXSIZE-1);
                }
                else
                    strncat(hasRead,"hour:%H  ",MAXSIZE-1);
                break;
            case 'M':
                strncat(hasRead,"minute:%M  ",MAXSIZE-1);
                break;
            case 'S':
                strncat(hasRead,"second:\%S  ",MAXSIZE-1);
                break;
            case 'y':
                strncat(hasRead,"year without  century:%y  ",MAXSIZE-1);
                break;
            case 'm':
                strncat(hasRead,"month:%m  ",MAXSIZE-1);
                break;
            case 'd':
                strncat(hasRead,"date:\%d  ",MAXSIZE-1);
                break;
            
            default:
                printf("Wrong command!");
                break;
        }
    }
    
    strftime(prt,MAXSIZE,hasRead,ts);
    strcat(prt,"\n");
    if(fp == NULL)
        puts(prt);
    else{
        fputs(prt,fp);
        printf("put in file\n");
        fclose(fp);
    }



    exit(0);
}