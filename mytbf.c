#include<stdlib.h>
#include<stdio.h>
#include"mytbf.h"

struct mytbf_st
{
    int cps;
    int burst;
    int token;
};

struct mytbf_st *mytbf_init(int cps,int burst){
    struct mytbf_st *inited;
    inited = malloc(sizeof(struct mytbf_st));
    inited->cps = cps;
    inited->burst = burst;
    inited->token = 0;
    return inited;
}

int mytbf_fetchtoken(struct mytbf_st *inited,int a,int b){
    inited->token += a;

    if(inited->token > b){
        inited->token = b;
    }
    return a;
}

int mytbf_returntoken(struct mytbf_st *inited, int a){
    inited->token -= a;
    if(inited->token < 0){
        inited->token = 0;
    }
    return a;
}


int mytbf_destroy(struct mytbf_st *inited){
    free(inited);
    return 1;
}