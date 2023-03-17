#ifndef PROTO_H__
#define PROTO_H__

#define SPORT  "1999"
#define MAXDATA 1024
struct msg_t{
    uint32_t num;
    uint8_t data[];
}__attribute__((packed));


#endif