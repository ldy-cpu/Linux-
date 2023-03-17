#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>


#include "../include/proto.h"
#include "client.h"
/*
    -M指定多播组
    -P指定接收端口
    -p指定播放器
    -H显示帮助
*/

struct client_conf_st client_conf = {
    .rcvport = DEFAULT_RCVPORT,\
    .mgroup = DEFAULT_MGROUP,\
    .playercmd = DEFAULT_PLAYERCMD
};


void print_help(){
    printf("-P:change receive port.");
    printf("\n-M:change mgroup.");
    printf("\n-p:change player\n");
}


int main(int argc, char* argv[]){

    int c;
    while(1){
        c = getopt(argc,argv,"P:M:p:H");
        if(c<0)
            break;
        switch (c)
        {
        case 'P':
            client_conf.rcvport = optarg;
            break;
        case 'M':
            client_conf.mgroup = optarg;
            break;
        case 'p':
            client_conf.playercmd = optarg;
            break;
        case 'H':
            print_help();
            break;
        default:
            break;
        }
    }
    printf("rcvport = %s\nmgroup = %s\nplayercmd = %s\n",client_conf.rcvport,client_conf.mgroup,client_conf.playercmd);
    int sfd;
    sfd = socket(AF_INET,SOCK_DGRAM,0);

    struct ip_mreqn mreqn;
    inet_pton(AF_INET,client_conf.mgroup,&mreqn.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreqn.imr_address);
    mreqn.imr_ifindex = if_nametoindex("ens33");
    
    if(setsockopt(sfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreqn,sizeof(mreqn))<0){
        perror("setsocketopt()");
        exit(1);
    }

    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    laddr.sin_port = htons(atoi(client_conf.rcvport));

    if(bind(sfd,(void*)&laddr,sizeof(laddr))<0){
        perror("bind()");
        exit(1);
    }

    int  pfd[2];

    
    if(pipe(pfd)<0){
        perror("pipe()");
        exit(1);
    }

    int pid;

    pid = fork();

    if(pid == 0){
        close(sfd);
        close(pfd[1]);
        dup2(pfd[0],0);
        if(pfd[0]>0)
            close(pfd[0]);
        execl("/bin/sh","sh","-c",client_conf.playercmd,NULL);
        perror("execl()");
        close(pfd[0]);
        exit(1);
    }
    else{
      

        int chosen_id;
        close(pfd[0]);
        struct sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        socklen_t socklen;
        inet_pton(AF_INET,client_conf.mgroup,&serveraddr.sin_addr);
        serveraddr.sin_port = htonl(atoi(client_conf.rcvport));

        struct msg_list_st *msg_list;
        msg_list = malloc(MSG_CHANNEL_MAX);
        if(msg_list == NULL){
            perror("malloc()");
            exit(1);
        }
        int len;
        while(1){   
            fprintf(stderr,"\nreceiving list\n");
            len = recvfrom(sfd,msg_list,MSG_CHANNEL_MAX,0,(void*)&serveraddr,&socklen);
            fprintf(stderr,"\nreceive one\n");
            if(len<sizeof(struct msg_list_st)){
                fprintf(stderr,"message is too small.\n");
                continue;
            }
            if(msg_list->chnid !=LISTCHNID){
                fprintf(stderr,"chnid is not match.\n");
                continue;
            }
            break;
        }
        fprintf(stderr,"\nlen = %d\n",len);
        struct msg_listentry_st *pos;
        for(pos = msg_list->entry;(char*)pos < (char*)msg_list + len;pos = (struct msg_listentry_st*)((char*)pos + pos->len)){
            printf("channel %d:%s\n",pos->chnid,pos->desc);
            fflush(stdout);
            fprintf(stderr,"\nlen:%d\n",pos->len);
        }


     

    
        // for(int j = 0;j<3;j++){
        //     printf("channel %d:%s\n",msg_list->entry[j].chnid,msg_list->entry[j].desc);
        //     fflush(stdout);
        //     fprintf(stderr,"\nlen:%d\n",msg_list->entry[j].len);
        // }


        fprintf(stderr,"\nhas receive list!\n");
        free(msg_list);

  
        fprintf(stderr,"\nplease input channel.\n");
        scanf("%d",&chosen_id);

        struct msg_channel_st *msg_channel;
        msg_channel = malloc(MSG_CHANNEL_MAX);
        if(msg_channel == NULL){
            perror("malloc()");
            exit(1);
        }
        int snum = 0;
        while(1){
            len = recvfrom(sfd,msg_channel,MSG_CHANNEL_MAX,0,(void*)&serveraddr,&socklen);
            fprintf(stderr,"\nlen:%d\n",len);
            fprintf(stderr,"\nchnid:%d\n",msg_channel->chnid);
            if(len < sizeof(struct  msg_channel_st)){
                fprintf(stderr,"not msg_channel_st,\n");
                continue;
            }
            if(msg_channel->chnid == chosen_id){
                snum++;
                fprintf(stderr,"\nchannel_msg len:%d\n",len);
                int wlen;
                wlen = write(pfd[1],msg_channel->data,len-sizeof(chnid_t));
                while(wlen<len-sizeof(chnid_t)){
                    wlen += write(pfd[1],msg_channel->data + wlen,len-sizeof(chnid_t) - wlen);
                }
            }

            if(snum >2){
                // usleep(1300000);//1
                // usleep(550000);//2
                usleep(130000);//3
                snum = 0;
            }
            
        }

        free(msg_channel);
        close(sfd);
        close(pfd[1]);
        
        exit(0);
        
        
    }

    exit(0);
}