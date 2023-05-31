#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SZ 256
#define SIZE_HELP 18

char *helps   [SIZE_HELP] = {"my_shell",
                             "cd",
                             "exit",
                             "true",
                             "false",
                             "background",
                             "fg",
                             "jobs",
                             "history",
                             "again",
                             "ctrl+c",
                             "chain",
                             "conditional",
                             "pipes",
                             "redir",
                             "set",
                             "get",
                             "unset",
};
char *commands_help[SIZE_HELP] = {"Primer proyecto de Sistemas Operativos\n\n Integrantes: \nSherlyn Ballestero Cruz Grupo: C-311\nMaria de Lourdes Choy Fernandez Grupo: C-312\nJavier Rodriguez Sanchez Grupo: C-311\n\n Funcionalidades implementadas:\n\n cd: Se utiliza para cambiar el directorio actual\nexit: Se utiliza para cerrar el shell \n true:Comando que siempre tiene exito\n false: Comando que nunca tiene exito\nbackground: Permite enviar procesos al background\nfg: Permite enviar procesos del background al foreground\njobs: Lista los procesos que ocurren en el background\nhistory: Lista los ultimos 10 comandos ejecutados\n again: Permite volver a ejecutar comandos del history\ncntrl+c:Permite terminar el proceso actual\n chain: Permite crear instrucciones de varios comandos \n conditional: Permite simular instrucciones condicionales en una linea\npipes: Permite redirigir la salida de un comando a la entrada de otro\nredir: Permite redirigir la salida de un comando a un archivo o usar el texto de un archivo como entrada de un comando\nset permite asignarle valores a variables\nget: Permite ver el valor de una variable\n unset: Permite eliminar variables\n\n Para mas informacion acerca de un comando en especifico escribir help <nombre del comando>\n "
                                   ,
"El comando cd permite cambiar de directorio actual\ncd<dir> cambia el directorio actual por el directorio especificado\ncd folder se mueve hacia la carpeta folder dentro del directorio actual\ncd sin parametros te redirecciona a Home\n\n Implementacion:\n Para la implemantacion de esta funcionalidad llamamos a la funcion chdir para cambiar el directorio actual.\nEn caso de no tener argumentos cambiamos la ruta a home/user\n Si el argumento escrito por elusuario esta entre parentesis, seran removidos(Con el objetivo de poder trabajar con carpetas con espacios en el nombre\n\n",
"El comando exit finaliza la ejecucion del shell,haciendo status -1 para dar fin al ciclo de Shell.c \n'",
"El comando true simula un comando que siempre tiene exito\n",
"El comando false simula un comando que siempre fracasa\n",
"Al utilizar el operador & luego de un comando hace que este se ejecute en el background\n",
"El operador fg n trae el n-esimo proceso del background hacia el foreground.\n De no tener argumentos, trae el ultimo proceso enviado al background.\n ",
"El comando jobs lista todos los procesos que estan ocurriendo en el background con sus respectivos pids y nombres\n",
"El comando history muestra los ultimos 10 comandos ejecutados\n",
"El comando again n hace referencia al n-esimo comando de history.\n",
"El comando ctrl+c permite finalizar la ejecucion de un proceso en foreground\nPara ello se utiliza la funcion kill\n",
"Los operadores( ; , || , && ) son utilizados para concatenar comandos.\n\nEl operador ; permite ejecutar varios comandos en la misma línea.\n\nEl comando command1 && command2 ejecuta command1 y luego,si este tiene éxito, ejecuta el comando command2\n\nEl comando command1 || command2 ejecuta command1 y si este no tiene éxito ejecuta command2.\n\nAdicionalmente se pueden combinar todos estos comandos en una sola línea.\n ",
"Los comandos if, then, else y end nos permiten realizar una operación condicional en una sola línea.\n\nEl comando if <command1> then <command2> else <command3> end, ejecuta el comando <command1>, de tener exito ejecuta <command2> en caso contrario se ejecuta <command3>. \n\nEl operador end se usa para indicar en fin de la operación condicional. Es posible prescindir del comando else en la operacion condicional.\n\n Es posible anidar varias operaciones condicionales\n",
"Al utilizar el operador command1|commad2 redirecciona la salida de command1 a la entrada de command2.\n ",
"Los operadores < > y >> permiten redireccionar las salidas de nuestros comandos\n El comando command1 < file envia el contenido del archivo file a la entrada de command1\n El comando command1>file sobreescribe el contenido del archivo file con la salida de command1\n El operador >> tiene un funcionamiento similar a > pero en lugar de sobreescribir el archivo, lo escribe al final del mismo.\n",
"El comando set <var><val> le asigna el valor val a la variable del sistema var, de no estar inicializada la variable, la inicializa.\n Como value puede ser colocado un comando entre comillas '' y se ejecutara el comando y su salida se guardara en la variable\n",
"El comando get var muestra en la consola el valor que tiene asignada la variable a.\n ",
"El comando unset var elimina la variable var del sistema\n"
};



const char* COMMAND_HISTORY = "history";
const char* COMMAND_EXIT = "exit";
const char* COMMAND_HELP = "help";
const char* COMMAND_TRUE = "true";
const char* COMMAND_FALSE = "false";
const char* COMMAND_CD = "cd";
const char* COMMAND_SET = "set";
const char* COMMAND_GET = "get";
const char* COMMAND_UNSET = "unset";
const char* COMMAND_IN = "<";
const char* COMMAND_OUT = ">";
const char* COMMAND_OUTOUT = ">>";
const char* COMMAND_PIPE = "|";
const char* COMMAND_BACKGROUND = "&";
const char* COMMAND_FG = "fg";
const char* COMMAND_JOBS = "jobs";
const char* COMMAND_IF = "if";
const char* COMMAND_AGAIN = "again";





int callHelp(char * comando,char **args ) {
    if (args[1]== NULL) {
        printf("\n%s\n", commands_help[0]);

        return 0;
    }
    else {

        for (int i = 0; i < SIZE_HELP; i++) {
            if (strcmp(helps[i], comando) == 0) {
                printf("%s", commands_help[i]);
                return 0;
            }
        }

        fprintf(stderr, "%s: command not found\n", comando);

        return 1;
    }
}  

    

