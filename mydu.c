#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/////////////有待改善


int recurtion(const char *root)
{
    long sum = 0;
    int err;
    struct stat now;
    err = lstat(root,&now);
    if(err == -1){
        // puts(root);
        perror("stat");
        exit(1);
    }
    
    
    //下层目录递归

    char* root_next =  (char*)malloc(1024*sizeof(char));
    // memset(root_next, 0, 1024);
    // memcpy(root_next, root, strlen(root));
    strcpy(root_next, root);
    glob_t cur;
    if(S_ISDIR(now.st_mode)){
        err = glob(strcat(root_next,"/*"),0,NULL,&cur);

        if(err!=0){
            printf("globerr1 %d\n",err);
            exit(1);
        }
        strcpy(root_next, root);
        // memset(root_next, 0, 1024);
        // memcpy(root_next, root, strlen(root));
        err = glob(strcat(root_next,"/.*"),GLOB_APPEND,NULL,&cur);
        if(err!=0){
            printf("globerr2 %d\n",err);
            exit(1);
        }
        free(root_next);
        for(int i = 0;i<cur.gl_pathc;i++){
            if(cur.gl_pathv[i][strlen(cur.gl_pathv[i])-1] == '.'){
                long u = now.st_blocks/2;
                printf("隐藏   %ld      %s\n",u,cur.gl_pathv[i]);
                fflush(stdout);
                continue;
            }
            sum += recurtion(cur.gl_pathv[i]);
        }
        globfree(&cur);
    }
    
    

     //本目录的信息
    
    sum += now.st_blocks/2;
    printf("%ld      %s\n",sum,root);
    fflush(stdout);
    
    

    return sum;




    
}

int main(int argc,char** argv){
    if(argc != 1 && argc!=2){
        printf("should be ./mydu *** or ./mydu");
        exit(1);
    }
    if(argc == 1){
        char cur_path[1024];
        getcwd(cur_path,1024);
        recurtion(cur_path);
        // recurtion(".");
    }
    recurtion(argv[1]);

    exit(0);


}