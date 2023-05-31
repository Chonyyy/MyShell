#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include "exec.h"
#include "readline.h"
#include "controlC.c"

#define MAXARGS 256
#define MAXLINE 256
#define MAG "\x1B[35m"
#define RESET "\x1B[0m"
#define CYN "\x1B[36m"

char* history_file_path;
void printPrompt();
int main()
{
    char *commandInput;
    char **args;
    int status;
    actual_foreground_pid = -1; /*Grounds and VaraibleList initialization*/
    vlist.cant = 0; //foreground.cant = background.cant = 0;
    history_file_path = history_file_path_creation();
    CatchingControlC();
   do{
        printPrompt();
        commandInput = read_LineCommand();
        args = splitLineCommand(commandInput);
        int argc = len(args);
       

        //utilizacion del valor de status devuelto por Execute para determinar cuando se termina el ciclo.
        status = all_Excecute(args, argc);
        //limpiando.....
        free(commandInput);
        free(args);
    }while(status!=-1);

    return EXIT_SUCCESS;
}
void printPrompt(){
    printf(MAG "my_prompt" RESET CYN " $ " RESET);
}
