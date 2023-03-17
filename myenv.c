#include<stdlib.h>
#include<stdio.h>
#include<dlfcn.h>
#include<gnu/lib-names-64.h>
extern char ** environ;
int main(){
    for(int i = 0;environ[i]!=NULL;i++){
        puts(environ[i]);

    void* handle = dlopen(LIBM_SO,RTLD_LAZY);
    if(!handle){
        fprintf(stderr,"%s\n",dlerror);
        exit(1);
    }
    dlerror;
    
    exit(0);


}