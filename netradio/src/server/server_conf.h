#ifndef SERVER_CONF_H__
#define SERVER_CONF_H__

#define DEFAULT_MEDIADIR    "/var/media/"
#define DEFAULT_IF    "ens33"
#define  DEFAULT_RUNMODE    RUN_DAEMON

#define BURST  2

#include"../include/proto.h"
enum{
    RUN_DAEMON = 1,
    RUN_FOREGROUND
};

struct server_conf_st{
    char* rcvport;
    char* mgroup;
    char *media_dir;
    char runmode;
    char* ifname;
};

extern struct server_conf_st server_conf;

extern int sfd;
extern struct sockaddr_in raddr;
extern socklen_t addlen;

extern int token[MAXCHNID];
extern pthread_rwlock_t rwlock;
extern pthread_rwlockattr_t attr;

#endif