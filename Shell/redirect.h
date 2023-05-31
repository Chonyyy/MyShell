#ifndef _REDIRECT_H
#define _REDIRECT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/*operator <*/
void redirect_in(char *file_name){
    
    int fileIn = open(file_name, O_RDONLY);
    if(fileIn ==-1){
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    
    int err=dup2(fileIn,STDIN_FILENO);
    
    if(err==-1){
        printf("error en <");
    }
    close(fileIn);  
}
/*operator >*/
void redirect_out(char *file_name){
    int fileOut=open(file_name,O_WRONLY);
    if(fileOut==-1){
        perror("Open failed");
        exit(EXIT_FAILURE);
    }
    int err=dup2(fileOut,STDOUT_FILENO);
    if(err==-1){
        printf("error en >");

    }
    close(fileOut);

}
/*operator >>*/
void append_out(char *fileName){
    int fileERR=open(fileName,O_WRONLY|O_APPEND);
    if(fileERR==-1){
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    int err=dup2(fileERR,STDOUT_FILENO);
    if(err==-1){
        printf("error en >>");

    }
    close(fileERR);
}
      
#endif
