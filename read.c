#include<unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<stdlib.h>
#include<string.h>

int read_binary(char* file_name,char* buffer){
    int fd=open(file_name,O_RDONLY);

    struct stat st;
    stat(file_name, &st);
    int size = st.st_size;
    int n=read(fd,buffer,size);
    if(n<0){
        printf("read failed");
    }
    return n;

    

}