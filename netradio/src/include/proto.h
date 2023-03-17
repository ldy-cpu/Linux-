#ifndef PROTO_H__
#define PROTO_H__

#define DEFAULT_MGROUP  "224.2.2.2"
#define DEFAULT_RCVPORT   "1998"
#define SERVER_PORT         "1999"

#define CHNNR  100          /*最大频道数*/
#define  LISTCHNID  0       /*节目单的号*/
#define  MINCHNID   1       /*从1到100 都是固定的广播或多播内容*/
#define  MAXCHNID   100

#include<stdint.h>

#define MSG_CHANNEL_MAX    10000
#define MAX_DATA  (MSG_CHANNEL_MAX-sizeof(chnid_t))

typedef uint8_t chnid_t;


struct msg_channel_st
{
    chnid_t chnid;
    uint8_t data[1];
}__attribute__((packed));


struct msg_listentry_st{
    chnid_t chnid;
    uint16_t len;
    uint8_t desc[1];
}__attribute__((packed));

struct msg_list_st{
    chnid_t chnid;  /*must be LISTCHNID*/
    struct msg_listentry_st entry[1];
}__attribute__((packed));

#endif