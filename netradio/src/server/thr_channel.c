#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<sys/socket.h>
#include<glob.h>
#include<unistd.h>

#include"thr_channel.h"
#include"medialib.h"
#include"../include/proto.h"
#include"server_conf.h"



struct channel_media_st{
    FILE *fp;
    chnid_t chnid;
};





void* channel_thread(void* arg){
    struct channel_media_st* argg = (struct channel_media_st*)arg;

    struct msg_channel_st *nchn = malloc(MSG_CHANNEL_MAX);
    // if(nchn == NULL)
    //     return -1;
    nchn->chnid = argg->chnid;  //因为是单字节的大小，所以不用转化字节序
    int pos = 0;
    int len;
    int epoch;
    int sndlen;
    while(1){
        len = 1;
        sndlen = 0;
        pthread_rwlock_rdlock(&rwlock);
        if(token[nchn->chnid] == 0){
            pthread_rwlock_unlock(&rwlock);
            continue;
        }
        token[nchn->chnid]--;
        pthread_rwlock_unlock(&rwlock);
        while(len>0){
            len = fread(nchn->data + pos,1,MAX_DATA-sndlen,argg->fp);
            pos += len;
            sndlen += len;

        }
        
        // sleep(1);
        fprintf(stderr,"\nsndlen:%d\n",sndlen);
        fprintf(stderr,"\nchnid:%d\n",nchn->chnid);
        len = sendto(sfd,nchn,sizeof(struct msg_channel_st) + sndlen,0,(void*)&raddr,addlen);
        if(len<0)
            perror("sendto()");
        fprintf(stderr,"\nacctual sndlen:%d\n",len);
        // sendto(sfd, nlist, sizeof(struct msg_list_st) - 4 + allsize,0,(void*)&raddr,sizeof(raddr))
        
        if(sndlen == MAX_DATA){//读够长度一次，但文件还没结束
            pos = 0;
        }
        else{//长度没读够，但文件结束了
            fseek(argg->fp,(long)0,SEEK_SET);
        }
    }

    
}




int thr_channel_create(struct mlib_listentry_st *chn){
    token[chn->chnid] = 0;

    pthread_t tid;
    char* patt = malloc(1024);
    strcpy(patt,server_conf.media_dir);
    strcat(patt,"ch*");
    glob_t glb;
    glob(patt,0,NULL,&glb);
    strcpy(patt,glb.gl_pathv[(int)chn->chnid-1]);
    strcat(patt,"/*.mp3");
    glob(patt,0,NULL,&glb);

    fprintf(stderr,"\nmp3path:%s\n",glb.gl_pathv[0]);
    FILE* fp;
    fp = fopen(glb.gl_pathv[0],"r");
    if(fp == NULL)
        return -1;

    struct channel_media_st *arg = malloc(sizeof(struct channel_media_st));
    arg->chnid = chn->chnid;
    arg->fp = fp;
    
    if(pthread_create(&tid,NULL,channel_thread,(void*)arg) != 0)
        return -1;
    return 0;
}
// int thr_channel_destroy(struct );
// int thr_channel_destroyall(void);