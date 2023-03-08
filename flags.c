#include "my_tar.h"

int getFlags(int argc, char** argv, int *index){
    int flags = 0b0;
    int inner_index = 1;
    if(argv[1][0] != '-'){
            return flags;
    }
    while(*index < argc){
        if(argv[*index][0] != '-'){
            return flags;
        }
        if(argv[*index][inner_index] == 'c'){
            flags |= CREATE;
        }
        else if(argv[*index][inner_index] == 'f'){
            flags |= FILE;
        }
        else if(argv[*index][inner_index] == 'r'){
            flags |= APPEND;
        }
        else if(argv[*index][inner_index] == 't'){
            flags |= LIST;
        }
        else if(argv[*index][inner_index] == 'u'){
            flags |= APPEND_IF;
        }
        else if(argv[*index][inner_index] == 'x'){
            flags |= EXTRACT;
        }
        if(strlen(argv[*index]) > 2 && inner_index<(int)strlen(argv[*index])){
            inner_index++;
        }
        else{
            *index+=1;
            inner_index = 1;
        }
    }
    return flags;
}
