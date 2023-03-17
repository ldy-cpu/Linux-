#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc,char** argv){
    if(argc != 3){
        perror("should be ./delete_row file row_num");
        exit(1);
    }
    int row_num = 0;
    int i = 0;
    while(argv[2][i]!='\0'){
        if(argv[2][i] - '0' <0 || argv[2][i] - '0' > 9){
            perror("wrong row_num");
            exit(1);
        }
        row_num = row_num*10 + argv[2][i] - '0';
        i++;
    }
    FILE* r = fopen(argv[1],"r");
    FILE* r_ = fopen(argv[1],"r+");
    int j = 1;
    int a;
    int all = 0;
    while(j<row_num){
        a = fgetc(r_);
        if(a == '\n'){
            j++;
        }
        all++;
    }
    j = 1;
    while(j<=row_num){
        a = fgetc(r);
        if(a == '\n'){
            j++;
        }
    }
    
    while((a = fgetc(r)) != EOF){
        fputc(a,r_);
        all++;
    }
    
    fclose(r);
    fclose(r_);
    truncate(argv[1],all);
    exit(0);
}