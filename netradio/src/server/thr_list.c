#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>


#include"medialib.h"
#include"../include/proto.h"
#include"server_conf.h"
#include"thr_list.h"



struct msg_list_st *nlist;


void *send_list(void* arg){
    // fprintf(stderr,"send_list()");
    int allsize = 0;
    

    struct argg *arggg;
    arggg = (struct argg*)arg;
    struct msg_listentry_st *all[arggg->list_size];

    //
    //创建了多个msg_listentry_st,把mlib_listentry_st内容导入msg_listentry_st。
    
    for(int i = 0;i<arggg->list_size;i++){
        struct msg_listentry_st* listentry = malloc(sizeof(struct msg_listentry_st) + strlen(arggg->list[i]->desc));
        // *listentry = *(arggg.list[i]);
        listentry->chnid = i+1;
        listentry->len = strlen(arggg->list[i]->desc) + 4;
        // fprintf(stderr,"\nlen:%d\n",listentry->len);
        strcpy(listentry->desc,arggg->list[i]->desc);

        all[i] = listentry;
        allsize += sizeof(struct msg_listentry_st) + strlen(arggg->list[i]->desc);
    }
    
    //mlib_listentry_st数组可以都释放了  在server.c
    // for(int i - 0;i<arggg.list_size;i++){
    //     mlib_freechnlist(arggg.list[i]);
    // }
    
    

    //创建要发送的msg_list_st，把msg_listentry_st拷贝到msg_list_st，msg_list_st可以释放了。
    nlist = (struct msg_list_st*)malloc(sizeof(struct msg_list_st) - 4 + allsize);
    
    if(nlist == NULL){
        perror("nlist malloc()");
        exit(1);
    }


    struct msg_listentry_st* pos = nlist->entry;
    for(int i = 0;i<arggg->list_size;i++){
        *(pos) = *all[i];
        strcpy(pos->desc,all[i]->desc);
        pos = (struct msg_listentry_st*)((char*)pos + sizeof(struct msg_listentry_st) + strlen(pos->desc));
        free(all[i]);
    }


    free(arggg);


    nlist->chnid = LISTCHNID;

    
    int len;
    while(1){
    //     fprintf(stderr,"\nsending list\n");
        len = sendto(sfd, nlist, sizeof(struct msg_list_st) - 4 + allsize,0,(void*)&raddr,sizeof(raddr));
        // fprintf(stderr,"\nsend size = %d\n",len);
        sleep(1);
    }
    
    pthread_exit(NULL);                                                                                                                                                                                                                                                                                                                                                                                                                      
}

int thr_list_create(struct mlib_listentry_st** list, int list_size){
    // fprintf(stderr,"thr_list_create()");
    pthread_t tid;
    struct argg *arg = malloc(sizeof(struct mlib_listentry_st**) + sizeof(int));
    arg->list = list;
    arg->list_size = list_size;
    if(pthread_create(&tid,NULL,send_list,(void*)arg)!=0)
        return -1;
    
    return 0;
}

int thr_list_destroy(void){
    free(nlist);
}