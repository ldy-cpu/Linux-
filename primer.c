#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#define LEFT 30000000
#define RIGHT 30200000


int main(){
    double u;
    int tag = 0;
    for(int i = LEFT;i<=RIGHT;i++){
        u = (double)i;
        tag = 0;
        for(int j = 2;j<=(int)sqrt(u);j++){
            if(i%j == 0){
                tag = 1;
                break;
            }
        }
        if(tag == 1)
            continue;
        printf("%d  ",i);
    }
    exit(0);

}