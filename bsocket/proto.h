#ifndef PROTO_H__
#define PROTO_H__

#define  RCVPORT   "1998"
#define  SNDPORT   "1999"

#define NAMESIZE  11
#define NAMEMAX  1024

struct msg_st{
    
    uint32_t math;
    uint32_t chinese;
    uint8_t name[];
}__attribute__((packed));

#endif