
#include <stdint.h>
#include "my_tar.h"

char* check_file_permissions(struct stat* sb){
    char *modeval = malloc(sizeof(char) * 9 + 1);
    mode_t perm = sb->st_mode;
    modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[9] = '\0';
    return modeval;
}
char* dec_to_oct(int dec){
  char oct_buff[12] = {'\0'};
  int i = 0;
  while(dec != 0){
    oct_buff[i]  = *itoa(dec%8); 
    dec/=8;
    i++;
  }
  while(i < 11){//fill the rest with zeros
    oct_buff[i] = '0';
    i++;
  }
  char* oct = reverse_string(&oct_buff[0]);
  oct[11] = '\0';
  char* rep = strdup(oct);
  return rep;
}
char* check_file_type(struct stat* sb,char* file_type){
    if (S_ISREG(sb->st_mode)) {
        /* Handle regular file */
        *file_type = REGTYPE;
    }
    else if(S_ISDIR(sb->st_mode)){
        /* Handle directory */
        *file_type = DIRTYPE;
    }
    else if(S_ISCHR(sb->st_mode)){
        /* Handle character device */
        *file_type = CHRTYPE;
    }
    else if(S_ISBLK(sb->st_mode)){
        /* Handle block device */
        *file_type = BLKTYPE;
    }
    else if(S_ISFIFO(sb->st_mode)){
        /* Handle FIFO */
        *file_type = FIFOTYPE;
    }
    else if(S_ISLNK(sb->st_mode)){
        /* Handle symbolic link */
        *file_type = LNKTYPE;
    }
    return file_type;
}


bool populate_header(struct posix_header* ph, struct stat* sb, char* filename){
    int count = 0;
    struct passwd *pwd = getpwuid(sb->st_uid);
    if(pwd == NULL){
        perror("getpwuid() Failed: ");
        return EXIT_FAILURE;
    }
    struct group *grp = getgrgid(sb->st_gid);
    if(grp == NULL){
        perror("getgrgid() Failed: ");
        return EXIT_FAILURE;
    }
    char name[100] = {'\0'};
    char md[] = "0000644";
    count+=add(md);
    char id[] = "0001750";
    char mode[8] = {'\0'};
    char id_[8] = {'\0'};

    // char chksum[7] = {'\0'};
    char* f = " 0";



    char linkname[100] = {'\0'};
    char guname[32] = {'\0'};
    char docode[] = "docode";
    count+=add(docode);
    count+=add(docode);
    char minmaj[8] = {'\0'};
    char prefix[155] = {'\0'};

    strncpy(name,filename,100);
    count+=add(name);
    strncpy(mode,md,7);
    
    strncpy(id_,id,8);
    count+=add(id_);
    count+=add(id_);
    // strncpy(size,sz,12);
    // strncpy(mtime,mtim,12);

    strncpy(guname,docode,7);
    strncpy(ph->size,dec_to_oct(sb->st_size),12);
    count+=add(ph->size);
        // // //mtime
    strncpy(ph->mtime, dec_to_oct(sb->st_mtim.tv_sec),12);
    count+=add(ph->mtime);
    strncpy(ph->name,name,100);
    strncpy(ph->mode,mode,8);
    strncpy(ph->uid,id_,8);
    strncpy(ph->gid,id_,8);
    // strncpy(ph->size,size,12);
    // strncpy(ph->mtime,mtime,12);
        // char file_type;
        // check_file_type(sb,&file_type);
    strncpy(ph->typeflag,f,2);
    strncpy(ph->linkname,linkname,100);
    strncpy(ph->magic,OLDGNU_MAGIC,8);
    count+=add(OLDGNU_MAGIC);
    strncpy(ph->uname,guname,32);
    strncpy(ph->gname,guname,32);
    strncpy(ph->devmajor,minmaj,8);
    strncpy(ph->devminor,minmaj,8);
    strncpy(ph->prefix,prefix,155);
    // printf("%d\n",count);
    count+=304;//8 spaces from empty checksum plus the filetype (0);
    char* chec = dec_to_oct(count);
    chec = &chec[5];
    strncpy(ph->chksum,chec,7);
    return EXIT_SUCCESS;
}
