#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int ftype(const char* name){

    struct stat mystate;
    if(stat(name,&mystate) < 0){
        fprintf(stderr,"get stat err");
        exit(1);
    }
    mode_t a = mystate.st_mode;
    
    switch (a & S_IFMT)
    {

        
        case S_IFSOCK:{

            return 1;
        };
        case S_IFLNK:{

            return 2;
        }

        case S_IFREG:{

            return 3;
        }

        case S_IFBLK:{

            return 4;
        }
        case S_IFDIR:{

            return 5;
        }
        case S_IFCHR:{

            return 6;
        }
        case S_IFIFO:{

            return 7;
        }
        default :{
            fprintf(stdout,"mode_t err");
            exit(1);
        }

    }

}


int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr,"should be ./ftype ***\n");
        exit(1);
    }


    switch(ftype(argv[1])){
        case 1:{
            fprintf(stdout,"socket\n");
            break;
        }
        case 2:{
            fprintf(stdout,"linke\n");
            break;
        }

        case 3:{

            fprintf(stdout,"regular\n");
            break;
        }

        case 4:{

            fprintf(stdout,"block\n");
            break;
        }
        case 5:{

            fprintf(stdout,"direct\n");
            break;
        }
        case 6:{

            fprintf(stdout,"character\n");
            break;
        }
        case 7:{

            fprintf(stdout,"pipe\n");
            break;
        }
        default: {
            fprintf(stdout,"mode_t err");
            exit(1);
        }
    }



    exit(0);
}