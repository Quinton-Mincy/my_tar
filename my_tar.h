#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/sysmacros.h>

#define CREATE    0b1
#define APPEND    0b10
#define LIST      0b100
#define APPEND_IF 0b1000
#define EXTRACT   0b10000
#define FILE      0b100000

struct posix_header
{                              /* byte offset */
  char name[100];               /*   0 -filepath*/ 
  char mode[8];                 /* 100 -file permissions*/
  char uid[8];                  /* 108 -numeric user id*/
  char gid[8];                  /* 116 -numeric group id*/
  char size[12];                /* 124 -size of file in bytes, linked files are size zero*/
  char mtime[12];               /* 136 -data modification*/
  char chksum[7];               /* 148 -simple sum of all bytes in the header block*/
  char typeflag[2];                /* 156 -type of file*/
  char linkname[100];           /* 157 -backup*/
  char magic[6];                /* 257 - P1003 archive format*/
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[155];             /* 345 */
                                /* 500 */
};

typedef struct s_ph_info{
    char* name;
    char* mtime;
    struct s_ph_info* next;
}ph_info;

/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

#define TMAGIC   "ustar"        /* ustar and a null */
#define OLDGNU_MAGIC "ustar  "
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2

//create_tar
bool check_files(char* filename,ph_info* curr_files,struct posix_header* ph);
void push_to_tar(int fd_tar, int fd_file, struct posix_header* ph, struct stat* sb, int* block_count);
bool tar_loop(int ac,char* av[], int fd_tar, int* block_count,int* i, int* flags, ph_info* curr_files);
//flags
int getFlags(int argc, char** argv, int *index);
//formatting
int add(char* word);
char* dec_to_oct(int dec);
char* itoa(int num);
char* reverse_string(char* str);
//pos_header
char* check_file_permissions(struct stat* sb);
char* check_file_type(struct stat* sb,char* file_type);
bool populate_header(struct posix_header* ph, struct stat* sb, char* filename);
//read_tar
ph_info* create_node(char* name, char* mtime);
ph_info* read_tar(int* fd_tar,int* block_count,int* flags);

#endif 
