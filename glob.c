#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include<sys/types.h>
#include<dirent.h>


// int main(int argc,char** argv){
//     glob_t globres;
//     int err = glob(PAT,0,NULL,&globres);
//     if(err){
//         printf("find name err:%d",err);
//         globfree(&globres);
//         exit(1);
//     }
//     for(int i = 0;i<globres.gl_pathc;i++){
//         puts(globres.gl_pathv[i]);
//     }
//     globfree(&globres);
//     exit(0);
// }

int main(int argc,char** argv){
    if(argc<2){
        printf("should be ./glob ***");
        exit(1);
    }
    DIR* dp = opendir(argv[1]);
    if(dp == NULL){
        perror("open");
        exit(1);
    }
    struct dirent *now = readdir(dp);
    while(now != NULL){
        printf("%s\n",now->d_name);
    }
    closedir(dp);
    exit(0);
}