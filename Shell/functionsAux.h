#define MAXPROCESSES 256
//#include "exec.h"
//metodos auxiliares
//set
void create_archive_if_doesnt_exit(int output_fd, char* name){

    if (output_fd != -1)
        return;

    close(output_fd);
    output_fd = open(name, O_CREAT);
    close(output_fd);
    output_fd = open(name, O_TRUNC | O_RDONLY);
    

}
void reading_the_output_from_txt(char* output,int output_fd){
    
    char ch;
    int index = 0;
    for(int i = 0; i < MAXLINE; i++)
        output[i] = '\0';

    output_fd = open("output.txt", O_RDONLY, 0);
    while (read(output_fd, &ch, sizeof(ch) != 0))
    {
        if (ch == '\n')
        {
            output[index] = ' ';
            index++;
            continue;
        }
        output[index] = ch;
        index++;
    }
    close(output_fd);
}
void executing_the_command(char**args, char * vname, int argc, char* command_line){
    
    printf("ejecutando el comando");
     strcpy(vname, args[1]);
    char buf[MAXLINE]; 
    argc = 0;      
    strcat(command_line, " > output.txt\n");
    strcpy(buf, command_line);
    basic_parse(buf, args, &argc);
    all_Excecute(args,argc);
}
void use_of_commas(char** args, char* command_line){
   
    printf("comillas");
    //char* command_line;
    for (int i = 1; i < MAXLINE; i++)
    {
        if (args[2][i] == '`')
        {
            command_line[i - 1] = '\0';
            break;
        }

        if (args[2][i] == '&')
        {
            command_line[i - 1] = ' ';
        }

        command_line[i - 1] = args[2][i];
    }
    printf("%s cm2 ", command_line);
}
//history
void add_last_command(char** args,int argc, int history_fd){

    for (int i = 0; i < argc; i++) 
        {
            char space = ' ';
            int pos = 0;
            while (args[i][pos] != '\0')
            {
                write(history_fd, &args[i][pos], sizeof(args[i][pos]));
                pos++;
            }
            write(history_fd, &space, sizeof(space));
        }
}
void analysis_if_file_empty(int history_fd){

    struct stat buff;
    fstat(history_fd, &buff);
    if (buff.st_size != 0) //si el file no esta vacio, entonces escribimos un salto de linea
    {
        char jump_line = '\n';
        write(history_fd, &jump_line, sizeof(jump_line));
    }

}

int string_to_int_parse(char *p)
{
    int ans = 0;
    int pos = 0;
    while (p[pos] != '\0')
    {
        ans = (ans * 10) + p[pos] - '0';
        pos++;
    }

    return ans;
}
//if
int caseIf(char** args, int argc)
{
    int posif=0;
    int posthen=-1;
    int poselse=-1;
    int posend=-1;
    int contif=0;
    //primero guardo las posiciones de if, then, else, end
    for(int i=1;i<argc;i++)
    {
        if(strcmp(args[i],"if")==0)
        //si hay multiif tengo que ignorar todo lo que este antes de un end
        {
            contif++;
            continue;
        }
        if(strcmp(args[i],"end")==0)
        {
            //Analizo el caso que se haya abierto otro if
            if(contif==0)
            {
                posend=i;
                break;
            }
            contif--;
            continue;
        }
        //si hay un if abierto los ignoro
        if(posthen==-1 && contif==0 && strcmp(args[i],"then")==0)
        {
            posthen=i;
            continue;
        }
        if(poselse==-1 && contif==0 && strcmp(args[i],"else")==0)
            poselse=i;
    }
    
    if((posthen>poselse && poselse!=-1) || posthen==-1 || posend==-1)
    //then debe existir y no puede estar antes de un else 
    {
        printf("%s","SyntaxError\n");
        return 0;
    }

    char ** comand[3];
    Separate(args,argc,posend,comand);
    //dame lo que esta antes del end y despues del if
    Separate(comand[0],posend,0,comand);
    
    if(poselse!=-1)
    {
        //dame lo que esta antes y despues del else
        Separate(comand[1],posend-1,poselse-1,comand);
        comand[2]=comand[1];
        //dame lo que esta antes y despues del then
        Separate(comand[0],poselse-1,posthen-1,comand);
    }
    else
        //dame lo que esta antes y despues del then
        Separate(comand[1],posend-1,posthen-1,comand);

    if(Execute(comand[0],posthen-1))
        return Execute(comand[1],(poselse==-1?posend:poselse)-posthen-1);
    else if(poselse!=-1)
        return Execute(comand[2],posend-poselse-1);
}

char *sub_str(char *line, int init, int end) {
  char *new_line = (char *) malloc(end - init + 1);

  int i;
  for (i = 0; i < end - init + 1; i++) {
      new_line[i] = line[i + init];
  }
  new_line[i] = 0;

  return new_line;
}

struct
{
    int status;
    pid_t pid;
    char* name;

} jobs[MAXPROCESSES];

int totalBackground=0;

int backgroundGetTotal()
{
    int i=totalBackground-1;
    for(;i>=0;i--)
    {
        if(jobs[i].status!=0)
        {
            free(jobs[i].name);
            totalBackground--;
            continue;
        }
        break;
    }
    return totalBackground;
}

void BackgroundClean()
{
    int i=0,j=0;
    for(;i<totalBackground;i++)
    {
        if(jobs[i].status!=0)
        {
            for(j=i+1;j<totalBackground;j++)
            {
                if(jobs[j].status==0)
                {
                    free(jobs[i].name);
                    jobs[i].name=malloc(strlen((jobs[j].name)+1) * sizeof(char));
                    strcpy(jobs[i].name,jobs[j].name);
                    jobs[i].pid=jobs[j].pid;
                    jobs[i].status=0;
                    jobs[j].status=1;
                    break;
                }
            }
            if(j==totalBackground)break;
        }
    }
    backgroundGetTotal();
}

int backgroundAdd(pid_t pid,char* name)
{
    BackgroundClean();
    if(totalBackground==MAXPROCESSES)
    {
        printf("top Reached \n");
        return -1;
    }
    jobs[totalBackground].name=name;
    jobs[totalBackground].pid=pid;
    jobs[totalBackground++].status=0;
    return 0;
}

void BackgroundFinish(pid_t pid)
{
    int i=0;
    for(i=0;i<totalBackground;i++)
    {
        if(pid==jobs[i].pid)
        {
            jobs[i].status=1;
            break;
        }
    }
}