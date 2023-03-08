#include "my_tar.h"

bool check_files(char* filename,ph_info* curr_files,struct posix_header* ph){
    while(curr_files != NULL){
        if( (strcmp(curr_files->name, filename)) == 0 ){
            int mtime_curr_files = atoi(curr_files->mtime);
            int mtime_ph = atoi(ph->mtime);
            if( mtime_curr_files < mtime_ph){
                return true;
            }else{
                return false;
            }
        }
        curr_files = curr_files->next;
    }
    return true;
}

void push_to_tar(int fd_tar, int fd_file, struct posix_header* ph, struct stat* sb, int* block_count){
    int file_size = sb->st_size;
    int file_blocks = (file_size/512) ? ((file_size/512)+1) : 1;

    *block_count += (file_blocks +1);

    char burn[12] = {'\0'};
    char pad[512] = {'\0'};
    char* file_contents = malloc(file_size);
    write(fd_tar, ph,sizeof(struct posix_header));
    write(fd_tar,&burn,sizeof(burn));
    /*read contents of file -to- buffer
    */
    read(fd_file,file_contents,file_size);
    //  /*write contents of file -from- buffer
    // */
    write(fd_tar,file_contents,file_size);
    //padding between next header
    int padding = (file_size%512) ? (512 - (file_size % 512)) : 0;
    write(fd_tar,&pad,padding);
}

bool tar_loop(int ac,char* av[], int fd_tar, int* block_count,int* i, int* flags, ph_info* curr_files){
    while(*i < ac){
        struct posix_header ph;
        struct stat* sb = malloc(sizeof(struct stat));
        char* filename = av[*i];
        int fd_file = open(filename,O_RDONLY, 0644);
        if( (lstat(filename,sb)) == -1){
            perror("my_tar: i_don_t_exist: Cannot stat: No such file or directory");
            return 1;
        }
        if( (populate_header(&ph, sb, filename)) == 1){
            close(fd_file);
            free(sb);
            return false;
        }
        if(  (*flags&APPEND_IF) && (check_files(filename,curr_files,&ph)) ) {
            push_to_tar(fd_tar, fd_file, &ph, sb, block_count);
        }
        else if(( (*flags&CREATE) || (*flags&APPEND) )){
            push_to_tar(fd_tar, fd_file, &ph, sb, block_count);
        }
        close(fd_file);
        free(sb);
        *i+=1;
    }
    return true;
}
