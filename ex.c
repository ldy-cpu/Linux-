#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){
    execl("/usr/bin/sleep","sleep","100",NULL);
    perror("exec");
    exit(1);

}
