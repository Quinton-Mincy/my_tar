#include "my_tar.h"

int main(int ac, char* av[]){
    int index = 1;
    int block_count = 0;
    int flags = getFlags(ac,av,&index);
    char* tar_filename = av[index];
    index++;
    struct stat* sb = malloc(sizeof(struct stat));
    ph_info* curr_files = NULL;
    int fd_tar = 0;
    if( !(flags&EXTRACT) ){
        if( ((lstat(tar_filename,sb)) == 0 && (flags&APPEND)) || (((lstat(tar_filename,sb)) == 0) && ((flags&APPEND_IF) || (flags&LIST)))  ){
            fd_tar = open(tar_filename,O_RDWR,0644);//read existing tar file
            curr_files = read_tar(&fd_tar,&block_count,&flags);
            block_count/=512;
        }else if(flags&CREATE){
            fd_tar = open(tar_filename,O_CREAT | O_RDWR | O_TRUNC, 0644);//create new tar file
        }
        if(tar_loop(ac,av,fd_tar,&block_count,&index,&flags,curr_files)){
            char end[10240] = {'\0'};
            int end_bytes = 0;
            int block_mod = 20 - (block_count%20);
            if(block_mod >= 2){//tar files must be in 20 block sections
                end_bytes = 512* block_mod;
            }
            else if(block_mod == 1){//must end in at least 2 blocks of null bytes (end-of-file marker)
                write(fd_tar,&end,512);
                end_bytes = 10240;
            }
            write(fd_tar,&end,end_bytes);   
        }
    }else{
        fd_tar = open(tar_filename,O_RDWR,0644);
        read_tar(&fd_tar,&block_count,&flags);
    }
    close(fd_tar);
    free(sb);
    return 0;
}
