#include<stdio.h>
#include<stdlib.h>
#include<setjmp.h>

static jmp_buf hah;
void c(void){
    printf("jump!\n");
    longjmp(hah,6);
    return;
}


void b(void){
    printf("c begin\n");
    c();
    printf("c end\n");
    return;
}

void a(void){
    printf("b begin\n");
    
    
    b();
    
    
    printf("b end\n");
    return;
}

int main(void){
    printf("a begin\n");
    int ret = setjmp(hah);
    if(ret != 0)
        printf("has jumped:%d\n",ret);
    else
        a();
    printf("a end\n");
    exit(0);
}