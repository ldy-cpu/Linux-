#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<error.h>

int main(void){
    
    fwrite("a",1,1,stdout);
    // putchar('a');
    fflush(stdout);
    write(1,"b",1);
    exit(0);

}