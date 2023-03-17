#include<stdio.h>
#include<stdlib.h>

struct person{
    int num;
    int size;
};

struct person* person_num(){
    struct person a;
    a.num = 1;
    a.size = 2;
    return &a;
}
struct person*  person_size(){
    struct person *b;
    b->num = 1;
    b->size = 2;
    return b;
}

int main(void){
    // person_num;
    person_size();
    exit(0);
}