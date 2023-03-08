#include "my_tar.h"

ph_info* create_node(char* name, char* mtime){
    ph_info* new_node = malloc(sizeof(ph_info));
    new_node->mtime = strdup(mtime);
    new_node->name = strdup(name);
    new_node->next = NULL;
    return new_node;
}

ph_info* read_tar(int* fd_tar,int* block_count,int* flags){
    ph_info* head = NULL;
    ph_info* head_cpy = NULL;
    while(1){
        struct posix_header ph;
        char burn[12] = {'\0'};
        char pad[512] = {'\0'};
        *block_count += read(*fd_tar,&ph,sizeof(struct posix_header));
        if(ph.name[0] == '\0'){
            lseek(*fd_tar,-(sizeof(struct posix_header)),SEEK_CUR);
            *block_count -= sizeof(struct posix_header);
            return head;
        }
        if( (*flags&LIST) ){
            printf("%s\n",ph.name);
        }
        else if( (*flags&APPEND_IF) ){
            if(head == NULL){
                head = create_node(ph.name, ph.mtime);
                head_cpy = head;
            }else{
                head_cpy->next = create_node(ph.name, ph.mtime);
                head_cpy = head_cpy->next;
            }
        }
        int file_size = atoi(ph.size);
        char* file_contents = malloc(file_size);
        *block_count += read(*fd_tar,&burn,sizeof(burn)); 
        *block_count += read(*fd_tar,file_contents,file_size);
        if( (*flags&EXTRACT) ){
            int fd = open(ph.name,O_CREAT | O_RDWR | O_TRUNC,0644);
            write(fd,file_contents,file_size);
            close(fd);
        }
        // printf("name: %s...mode: %s...uid: %s...gid: %s...size: %s...mtime: %s...filetype: %s...magic: %s...version: %s...devmajor: %s...devminor: %s...uname: %s...gname: %s...checksum: %s\n"
        //     ,ph.name, ph.mode, ph.uid, ph.gid, ph.size, ph.mtime, ph.typeflag, ph.magic, ph.version, ph.devmajor,ph.devminor,ph.uname, ph.gname,ph.chksum);
        // printf("%s\n\n",file_contents);
        int padding = 512 - (file_size % 512);
        *block_count += read(*fd_tar,pad,padding);
    }
    return head;
}
