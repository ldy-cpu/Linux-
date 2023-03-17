#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>


int main(){
    alarm(2);

    while(1){
        printf("1\n");
        fflush(stdout);
        pause();
    }

    printf("2\n");
    fflush(stdout);

    exit(0);
}