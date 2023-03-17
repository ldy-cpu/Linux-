#include<stdlib.h>
#include<stdio.h>
#include<glob.h>
#include"medialib.h"
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// #include"../include/proto.h"


struct channel_context_st{
    chnid_t chnid;
    char* desc;
    glob_t mp3glob;
    int pos;
    int fd;
    off_t offset;
    // mytbf_t *tbf;
};


int mlib_getchnlist(struct mlib_listentry_st **list,int *list_size,char* lib_pth){
    char patt[strlen(lib_pth)+2];
    strcpy(patt,lib_pth);
    strcat(patt,"*");
    glob_t ch;
    
    glob(patt,0,NULL,&ch);
    *list_size = ch.gl_pathc;

    char des[1024];
    int n;
    int m;
    int i;
    FILE *fp;
    for(i = 0;i<ch.gl_pathc;i++){
        struct mlib_listentry_st *now = malloc(sizeof(struct mlib_listentry_st));
        if(now == NULL)
            return -1;
        now->chnid = i+1;
        strcpy(des,ch.gl_pathv[i]);
        strcat(des,"/desc.text");
        fp = fopen(des,"r");
        if(fp == NULL)
            return -2;
        now->desc = malloc(1024);
        if(now->desc == NULL)
            return -3;
        n = 0;
        m = 0;
        while(1){
            n = fread(now->desc+m,1,10,fp);
            m += n;
            if(n == 0)
                break;
        }
        *(now->desc+m) = '\0';

        list[i] = now;
        fclose(fp);
    }

    return 0;
}


int mlib_freechnlist(struct mlib_listentry_st *now){
    free(now->desc);
    free(now);
    now = NULL;
    return 0;
}

// ssize_t mlib_readchn(chnid_t,void*, size_t);