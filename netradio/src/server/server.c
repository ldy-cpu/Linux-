#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

#include"server_conf.h"
#include"../include/proto.h"
#include"medialib.h"
#include"thr_channel.h"
#include"thr_list.h"




/*  
    -M 指定多播组
    -P 指定接收端口
    -F 前台运行
    -D 指定媒体库位置
    -I 网卡名称
    -H help
    
*/



struct server_conf_st server_conf = {
    .rcvport = DEFAULT_RCVPORT,\
    .mgroup = DEFAULT_MGROUP,\
    .media_dir = DEFAULT_MEDIADIR,\
    .runmode = DEFAULT_RUNMODE,\
    .ifname = DEFAULT_IF
};

int sfd;

struct sockaddr_in raddr;

socklen_t addlen = sizeof(raddr);

int token[MAXCHNID];
pthread_rwlock_t rwlock;
pthread_rwlockattr_t attr;



int daemonize(){
    pid_t pid = fork();
    if(pid<0){
        return -1;
    }
    if(pid > 0)
        exit(0);

    pid_t did;
    did = setsid();
    if(did<0){
        perror("setsid()");
        return -1;
    }
    chdir("/");

    return 0;
}

void daemon_exit(int s){
    exit(0);
}


void token_ctrl(int list_size){
    while(1){ 
        pthread_rwlock_wrlock(&rwlock);
        for(int i = 1;i<=list_size;i++){
            token[i] += 2;
            if(token[i]>BURST)
                token[i] = BURST;
        }
        pthread_rwlock_unlock(&rwlock);
    
        // usleep(1200000);//1
        // usleep(500000);//2
        usleep(1250000);//3
    }
}



int main(int argc, char* argv[]){

    int c = 1;
    struct sigaction sa;
    sa.sa_handler = daemon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGTERM);
    sigaddset(&sa.sa_mask,SIGINT);
    sigaddset(&sa.sa_mask,SIGQUIT);

    sigaction(SIGTERM,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);

    while(c>0){
        c = getopt(argc,argv,"M:P:FD:I:H");
        switch (c)
        {
        case 'M':
            server_conf.mgroup = optarg;
            break;
        
        case 'P':
            server_conf.rcvport = optarg;
            break;

        case 'F':
            server_conf.runmode = RUN_FOREGROUND;
            printf("FOREGROUND\n");
            fflush(stdout);
            break;

        case 'D':
            server_conf.media_dir = optarg;
            break;
            
        case 'I':
            server_conf.ifname = optarg;
            break; 

        case 'H':
            printf("-M 指定多播组\n-P 指定接收端口\n-F 前台运行\n-D 指定媒体库位置-n-I 网卡名称\n-H help");
            break;
        
        default:
            break;
        }
    }

    


    if(server_conf.runmode == RUN_DAEMON)
        if(daemonize()<0){
            perror("daemonize()");
            exit(1);
        }
    // printf("FOREGROUND\n");
    // fflush(stdout);

    
    sfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in laddr;
    laddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    laddr.sin_port = htons(atoi(SERVER_PORT));

    inet_pton(AF_INET,server_conf.mgroup,&raddr.sin_addr);
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(server_conf.rcvport));


    struct ip_mreqn mreqn;
    inet_pton(AF_INET,server_conf.mgroup,&mreqn.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreqn.imr_address);
    mreqn.imr_ifindex = if_nametoindex(server_conf.ifname);

    
    if(setsockopt(sfd,IPPROTO_IP,IP_MULTICAST_IF,&mreqn,sizeof(mreqn))<0){
        perror("setsockopt()");
        exit(1);
    }



    if(bind(sfd,(void*)&laddr,sizeof(laddr))<0){
        // perror("bind()");
        exit(1);
    }  

    struct mlib_listentry_st  *list[CHNNR];  //这里改为了指针数组，这样就不怕内存泄漏，而且恰好规定了最大频道数。
    int list_size;
    int err;

    err = mlib_getchnlist(list,&list_size,server_conf.media_dir);
    if(err<0){
        perror("mlib_getchnlist()");
        exit(1);
    }


    err = thr_list_create(list,list_size);
    if(err <0){
        perror("thr_list_create()");
        exit(1);
    }
    

    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr,PTHREAD_RWLOCK_PREFER_WRITER_NP);//设置写锁优先
    pthread_rwlock_init(&rwlock,&attr);


    for(int i = 0;i<list_size;i++){
        if(thr_channel_create(list[i])<0){
            perror("thr_channel_create()");
            exit(1);
        }
    }

    token_ctrl(list_size);







    while(1){
        // fprintf(stderr,"??");
        pause();
        
    }
        
        
////////////////////////////////////////////////////////////////////
    // thr_list_destroy();

    // pthread_join();
    // for(int j = 0;j<list_size;j++){
    //     err = mlib_freechnlist(list[j]);
    // }
    exit(0);
    
}