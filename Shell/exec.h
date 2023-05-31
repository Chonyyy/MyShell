#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"
#include "redirect.h"
#include "controlC.c"



int all_Excecute(char **args,int argc);
//este es el que se ejecuta primero. Asegura el history, evitando que Execute llame al history recursivo

int Execute(char**args, int argc);
//se encarga de ejecutar todos los comandos

int Execute_background(char **args,int argc);
//si la entrada termina en & ejecuta el comando en el background

void Separate(char**args,int argc,int a,char*** parte);
//funcion que separa args en lo que esta antes y despues de la posicion a y guarda ambos pedazos en la ultima entrada

int caseIf(char** args, int argc);
//Este caso se analiza aparte
char* Reconstruct (char ** args, int argc);
//obtiene la entrada original
int  WhiteSpaces(char ** args);
int all_Excecute(char **args,int argc){
    
   //Ignora linas en blanco.
    if (WhiteSpaces(args)==1){
        printf("No introduzca espacios vacios\n");
        return 0;
    }
    
    if(strcmp(args[0],"again")!=0){
        history(args,argc);//llama al history
    }

    return Execute(args,argc);
}

int Execute(char**args,int argc){
    
    if(strcmp(args[argc - 1], "&")==0)//llama al background
        return Execute_background(args,argc);
    
    //&& || ;
    for(int i=0;i<argc;i++)
    {
        if(strcmp(args[i], ";")==0)
        {
            char** part[2];
            Separate(args,argc,i,part);
            Execute(part[0],i);
            return Execute(part[1],argc-i-1);
        }
        if(strcmp(args[i], "&&")==0)
        {
            char** part[2];
            Separate(args,argc,i,part);
            if(Execute(part[0],i))
                return Execute(part[1],argc-i-1);
            else return 0;
        }
        if(strcmp(args[i], "||")==0)
        {
            char** part[2];
            Separate(args,argc,i,part);
            if(!Execute(part[0],i))
                return Execute(part[1],argc-i-1);
            else return 1;
        }
    }
    //tuberia
    for(int i=argc-1;i>=0;i--){
        if(strcmp(args[i], "|")==0)
        {
            char** parte[2];
            Separate(args,argc,i,parte);
            int fd[2];
            int in = dup(0);
            int out = dup(1);
            pid_t pid;
            pipe(fd);
            pid=fork();
            if(pid==0)
            {
                close(fd[0]);
                dup2(fd[1],STDOUT_FILENO);
                Execute(parte[0],i);
                exit(1);
            }
            else
            {
                pid=fork();
                if(pid==0)
                {
                    close(fd[1]);
                    dup2(fd[0],STDIN_FILENO);
                    Execute(parte[1],argc-i-1);
                    exit(1);
                }
                else
                {
                    close(fd[0]);
                    close(fd[1]);
                    wait(NULL);
                    wait(NULL);
                }
            }
            return 0;
        }
    }
    //redireccion
    for(int i=argc-1;i>=0;i--){
        if(strcmp(args[i], "<")==0)
        {
            char *archivo=args[i+1];
            int comandLen=i;
            
            //last_pid=fork();
            int pid=fork();
            if(pid==0){
                redirect_in(archivo);
                Execute(args,comandLen);
                printf("\n aver \n");
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);       
                return 0;
            }
        }
    
        if(strcmp(args[i], ">")==0){
             
            char *archivo=args[i+1];
            int comandLen=i;
            
            //last_pid=fork();
            int pid=fork();
            if(pid==0){
                
                redirect_out(archivo);
                Execute(args,comandLen);
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);       
                return 0;
            }
        }    

            if(strcmp(args[i], ">>")==0){
              
            char *archivo=args[i+1];
            int comandLen=i;

            
            //last_pid=fork();
            int pid=fork();
            if(pid==0){
                
                append_out(archivo);
                Execute(args,comandLen);
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);       
                return 0;
            }

    }

    }
    //...
    for (int a = 0; a < lsh_num_builtins(); a++) {
        if (strcmp(args[0], builtin_str[a]) == 0) {
            return (*builtin_func[a])(args,argc);
        }
    }
    
    
    //esto es para garantizar que execute no coja como argumento mas de lo q debe y que haya un null al fondo
    char * aux[argc+1];
    int b=0;
    while(b!=argc){
        aux[b]=args[b];
        b=b+1;
    }
    aux[b]=NULL;
    
    //int pi = fork();
    last_pid=fork();
    if (last_pid == 0) {
        if (execvp(aux[0], aux) == -1) {
            perror("lsh");
        }
        //getchar();
        exit(1);
    }
    else {
        wait(NULL);
        return 0;
    }


}

int Execute_background(char **args,int argc)
{
        if(argc==1)
            return 2;

        char *p = args[argc - 1];
        args[argc - 1] = NULL;

        pid_t pid = fork();//no deberian guardarlo para saber despues que es el
        if (pid==0)
        {
            backgroundAdd(getpid(),Reconstruct(args,argc-1));
            pid_t pidnieto=fork();
            if(pidnieto==0)
                return Execute( args, argc - 1);
            else
            {
                wait(NULL);
                BackgroundFinish(getpid());
            }
        }
        return 0;
}
//----------------------------------------------


//auxiliares
void Separate(char**args,int argc,int a,char*** parte)
{
     parte[0]=(char**)malloc((a)*sizeof(char*));
    parte[1]=(char**)malloc((argc-a-1)*sizeof(char*));
    int j=0;
    for(j=0;j<a;j++)
        parte[0][j]=args[j];
    for(j=a+1;j<argc;j++)
        parte[1][j-a-1]=args[j];
    //return parte;

}


char* Reconstruct (char ** args, int argc)
{
    int i=0,longTotal=0;
    for(i=0;i<argc;i++)
        longTotal+=strlen(args[i])+1;
    char* sentence=(char*)malloc(longTotal*sizeof(char));
    strcpy(sentence,"");
    for(i=0;i<argc;i++)
    {
        strcat(sentence,args[i]);
        strcat(sentence," ");
    }
    return sentence;
}

int  WhiteSpaces(char ** args){
       if (args[0] == NULL)
       {
        return 1;
       }
       else {
        return 2; 
        }
    }
