

#include "variables.h"
#include "shell.h"
#include "functionsAux.h"
#include "help.c"
#define MAXARGS 256
#define MAXPROCESSES 256
/*
Funciones
*/
int lsh_cd(char**args,int argc);
int lsh_help(char**args,int argc);
int lsh_exit(char**args,int argc);
int lsh_history(char**args,int argc);
int lsh_again(char**args,int argc);
int lsh_set(char**args,int argc);
int lsh_get(char** args,int argc);
int lsh_unset(char** args,int argc);
int lsh_jobs(char**args,int argc);
int lsh_fg(char**args,int argc);
int lsh_true(char**args,int argc);
int lsh_false(char**args,int argc);
int lsh_if(char**args,int argc);
char *read_LineCommand(void);
char **splitLineCommand(char *line);
int launch(char **args);
int Execute(char**args,int argc);




/*
Lista de comandos incorporados,seguidos por sus correspondientes funciones.
*/
char *builtin_str[] = {
"cd",
"help",
"exit",
"history",
"again",
"jobs",
"fg",
"set",
"get",
"unset",
"true",
"false",
"if"
};

int (*builtin_func[]) (char **,int) = {
&lsh_cd,
&lsh_help,
&lsh_exit,
&lsh_history,
&lsh_again,
&lsh_jobs,
&lsh_fg,
&lsh_set,
&lsh_get,
&lsh_unset,
&lsh_true,
&lsh_false,
&lsh_if
};





int lsh_num_builtins() {
return sizeof(builtin_str) / sizeof(char *);
}
/*
implementationes de las funciones.
*/
int lsh_cd(char**args,int argc){
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
     else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}
int lsh_help(char**args,int argc){
   return  callHelp(args[1],args);   
}
int lsh_exit(char**args,int argc){
    return -1;
}
int lsh_set(char**args,int argc){

    //Crear el archivo output.txt
    char*name = "output.txt";
    int output_fd = open("output.txt", O_TRUNC | O_RDONLY, 0);
    create_archive_if_doesnt_exit(output_fd,name);
    close(output_fd);

    if (argc == 1)//set
    {
        print_variables(&vlist);
        return 1;
    }

    if( args[2][0]!= '`')
    {
        push_variable(&vlist, args[1], args[2]);
        return 1;
    }
    else{
       //char *command_line[MAXARGS];
        char *command_line = (char *) malloc(len(args[3]));
       // printf(" %s arg",args[3]);
        char vname[MAXLINE];
        char output[MAXLINE];

        command_line = args[3];
        // use_of_commas(args, command_line);
       int i=0;
        for (int i = 1; i < len(args[3]); i++)
        {
            printf(" ento en el for ");
            if (args[2][i] == '`')
            {
                printf(" ult cosita ");
                command_line[i - 1] = '\0';
                break;
            }

            if (args[2][i] == '&')
            {
                command_line[i - 1] = ' ';
            }

            command_line[i - 1] = args[2][i];
            //printf("%c",command_line[i-1]);

        }
        //printf("%s-cl ",command_line);
        //executing_the_command(args,vname, argc, command_line);
        strcpy(vname, args[1]);
        char buf[MAXLINE]; 
        //argc = 0;      
        strcat(command_line, " > output.txt\n");
        strcpy(buf, command_line);
       
        printf("%s",buf);

       //basic_parse(buf, args, &argc);
       int a = len(buf);
       printf("%d",a);
       all_Excecute(buf,a);

       // reading_the_output_from_txt(output,output_fd);
        //push_variable(&vlist, vname, output);
    }

    // if (argc != 3){
    //     printf("estoy en dist de 3");
    //     return 0;
    // }
    //free(command_line);
    return 1;
}

int lsh_get(char** args,int argc){
    if(argc!=1)
        print_variable_value(&vlist, args[1]);
    return 1;
}

int lsh_unset(char** args,int argc){
    pop_variable(&vlist, args[1]);
    return 1;
}

int lsh_history(char**args,int argc){

    int history_fd = open(history_file_path, O_CREAT | O_RDONLY, 0);

    char line_read[MAXLINE];
    char c;
    int cont = 1;
    printf("%d: ", cont);
    while (read(history_fd, &c, sizeof(c) != 0))
    {
        if (c == '\n')
        {
            printf("%c", '\n');
            cont++;
            printf("%d: ", cont);
        }
        else
            printf("%c", c);
    }
    printf("%c", '\n');
    close(history_fd);
    return 1;
}

//Preprocessing Part
void history(char**args,int argc){ 

    char data[10 * MAXLINE];
    for (int i = 0; i < 10 * MAXLINE; i++)
        data[i] = '\0';

    int history_fd = open(history_file_path, O_RDONLY, 0);
    create_archive_if_doesnt_exit(history_fd, history_file_path);

    char c;
    int number_of_lines = 1;
    while (read(history_fd, &c, sizeof(c) != 0)) //Contanndo el numero de lineas de comandos
        if (c == '\n')
            number_of_lines++;
    close(history_fd);

    if (number_of_lines > 9)
    {
        int number_of_commands_to_ignore = number_of_lines - 9;
        history_fd = open(history_file_path, O_RDONLY, 0);
        while (number_of_commands_to_ignore) //Ignora todos los comando excepto los ultimos 10
        {
            read(history_fd, &c, sizeof(c) != 0);
            if (c == '\n')
                number_of_commands_to_ignore--;
        }

        read(history_fd, &data, sizeof(data)); 
        close(history_fd);
        history_fd = open(history_file_path, O_TRUNC); 
        close(history_fd);
        history_fd = open(history_file_path, O_APPEND | O_WRONLY);
        int index = 0;
        while (data[index] != '\0')
        {
            write(history_fd, &data[index], sizeof(data[index]));
            index++;
        }

        close(history_fd);
    }

    //Saving Part
    history_fd = open(history_file_path, O_APPEND | O_WRONLY);
    analysis_if_file_empty(history_fd);
    add_last_command(args,argc, history_fd);
    close(history_fd);
}

int lsh_again(char **args, int argc) {

    if(args[1]==NULL)
    {
        perror("lsh");
        return 0;
    }

    FILE *fr = fopen(history_file_path, "r");

    char line[100];
    int line_count = 1;
    char **new_args = malloc(100 * sizeof(char *));
    int index = 0;
    
    while (fgets(line, 100, fr) != NULL && strlen(line) > 0) {
        line_count++;
        if (atoi(args[1]) == line_count) {
            //printf("entre al if");
            char *cmd = sub_str(line, 0, strlen(line) - 2);
            char *token = strtok(cmd, " ");
            
            while (token != NULL) {
                new_args[index] = token;
                token = strtok(NULL, " ");
                index++;
            }
        new_args[index] = NULL;
        fclose(fr);
        }
    }

    int i = len(new_args);
    all_Excecute(new_args,i);
    return 1;
}

int lsh_jobs(char**args,int argc){

    for(int i=0;i<backgroundGetTotal();i++)
        if(jobs[i].status==0)
        {
            printf("%d",i);
            printf(" ");
            printf("%s", jobs[i].name);
            printf("\n");
        }
    return 0;
}
int lsh_fg(char**args,int argc){

    int* i;
    if(args[1]==NULL)
        waitpid(jobs[backgroundGetTotal()].pid,i,WUNTRACED);
    else 
    {
        if(jobs[atoi(args[1])].status==0)
        //debo crear funcion para transformar string en entero
            waitpid(jobs[atoi(args[1])].pid,i,WUNTRACED);//no se si se castea asi XD
    }
    return 0;
}
int lsh_true(char**args,int argc){
    return 1;
}
int lsh_false(char**args,int argc){
    return 0;
}
int lsh_if(char**args,int argc){
    return caseIf(args,argc);
}