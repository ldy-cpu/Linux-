#ifndef MEDIA_H__
#define MEDIA_H__ 

#include"../include/proto.h"

struct mlib_listentry_st{
    chnid_t chnid;
    char* desc;
};

int mlib_getchnlist(struct mlib_listentry_st **,int *,char* );
int mlib_freechnlist(struct mlib_listentry_st * );

// ssize_t mlib_readchn(chnid_t,void*, size_t);


#endif