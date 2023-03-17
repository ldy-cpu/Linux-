#ifndef THR_LIST_H__
#define THR_LIST_H__

#include"medialib.h"

struct argg{
    struct mlib_listentry_st** list;
    int list_size;
};

void *send_list(void*);

int thr_list_create(struct mlib_listentry_st**, int );

int thr_list_destroy(void);




#endif