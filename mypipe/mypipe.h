#ifndef MYPIPE_H__
#define MYPIPE_H__

#define PIPESIZE 1024


#endif 

struct node
{
    int tag;
    int data;
    struct node *next;
};


struct pipe{
    struct node *head;
    struct node *tail;
};

void init(struct pipe *p);


void add(int num,struct pipe *p);

int removee(struct pipe *p);