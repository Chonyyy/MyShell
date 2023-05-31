#include<stdlib.h>
#include<sys/types.h>//incluye el tipo pid_t
#include<sys/wait.h>//incluye el tipo pid_t
#include<unistd.h>//incluye los metodos para trabajar con procesos
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<fcntl.h>
#ifndef SignHandling_h
#define SignHandling_h

int last_pid;
void sign_handler(int sign_num)
{
     kill(last_pid,0);
     //printf("control c\n");
     signal(SIGINT,SIG_DFL);
}

void CatchingControlC(){
    signal(SIGINT,sign_handler); 
 }

#endif