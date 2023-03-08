
#include "my_tar.h"

int add(char* word){
    int i = 0;
    int len = strlen(word);
    int count = 0;
    while(i < len){
        count+= word[i];
        i++;
    }
    return count;
}

// char* dec_to_oct(int dec){
//   char* oct_buff = malloc(sizeof(char)*12);
//   int i = 0;
//   while(dec != 0){
//     oct_buff[i]  = *itoa(dec%8); 
//     dec/=8;
//     i++;
//   }
//   while(i < 11){//fill the rest with zeros
//     oct_buff[i] = '0';
//     i++;
//   }
//   char* oct = reverse_string(oct_buff);
//   oct[12] = '\0';
//   return oct;
// }

char* itoa(int num){
    char str[1024] = {'\0'};
    int i = 0;
    if(num != 0){
        while(num != 0){
            str[i] = (num%10) + '0';
            num/=10;
            i++;
        }
    }else{
        str[0] = 48;
    }
    char* final = strdup(str);
    if(strlen(final)>1){
        return reverse_string(final);
    }
    return final;
}
char* reverse_string(char* str){
    int endex = strlen(str) -1;
    int index = 0;
    while(index < endex){
        char temp = str[index];
        str[index] = str[endex];
        str[endex] = temp;
        index++;
        endex--;
    }
    return str;
}
