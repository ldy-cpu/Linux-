#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX      1024
typedef void mytbf_t;



struct mytbf_st *mytbf_init(int cps,int burst);

int mytbf_fetchtoken(struct mytbf_st *inited,int a,int b);

int mytbf_returntoken(struct mytbf_st *inited, int a);


int mytbf_destroy(struct mytbf_st *inited);

#endif