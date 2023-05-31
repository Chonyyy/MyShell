#define BUFSIZE 1024
#define TOKBUFFERSIXE 64
#define  TOKDELIM " \t\r\n\a"
#define MAXARGS 256

int CheckSpace(int position,int bufferSize,char *buffer);
int PutSpace(char c, char prev);
int basic_parse(char *buf, char **argv, int *argc);

/*
Funcion para leer el comando con los argumentos correspondientes a la entrada del usuario.
*/
 char *read_LineCommand(void){
     int bufSize=BUFSIZE;
     int position=0;
     char *buffer=malloc(sizeof(char)*bufSize);
     int c;
     //malloc si esta llena la memoria dinamica devuelve NULL, si hay coloca el valor en ese espacio de memoria.
     //errores de allocate
     if(!buffer){
        fprintf(stderr,"allocationError \n");
        exit(EXIT_FAILURE);
     }
     
    int comentario=0;//ADICION: PARA PONER COMENTARIOS

     while(1){
        //leyendo un char
        c=getchar();
        //si eof se remplaza por char vacio y se retorna
        if(c==EOF || c=='\n'){
            buffer[position]='\0';
            return buffer;
        }
        
        //-----------------------------------------------------------------------------------
        //ADICION: Quito los espacios en blanco repetidos, los iniciales e ignora comentarios
        //Ademas, pondre espacios necesarios, Ej: C1|C2 ---> C1 | C2
        
        if(c=='#')//a partir de aqui ignora la entrada
            comentario=1;
        
        if(comentario==1 || (c==' ' && (position==0 || buffer[position-1]==' ')))
            continue;//ignora el espacio innecesario y el comentario
        
        if(position != 0 && PutSpace(c,buffer[position-1]))
        //pone un espacio antes si es necesario
        {
            buffer[position]=' ';
            position++;
            int aux = CheckSpace(position,bufSize,buffer);
            if(aux==0)
                bufSize+=BUFSIZ;
        }
        //-----------------------------------------------------------
            
        
        buffer[position]=c;
        position++;
        
        //--------------------------------------
        //MODIFICACION: Necesito reutilizar esto
        int aux = CheckSpace(position,bufSize,buffer);
            if(aux==0)
                bufSize+=BUFSIZ;
        //--------------------------------------
      }
 }

//si se excede del buffer reallocate
int CheckSpace(int position,int bufferSize,char *buffer)
//nota: puede fallar lo del int* de la entrada, no recuerdo bien como se hacia.
{
    if(position > bufferSize){
        buffer=realloc(buffer ,(bufferSize+BUFSIZ) * sizeof(char*));
        //errores de allocate
        if(!buffer){
         fprintf(stderr,"allocationError \n");
         exit(EXIT_FAILURE);
        }
        return 0;
    }
    return 1;
}

int PutSpace(char c, char prev)
//mucho texto, en resumen, cosas como a|b;a <b&& c>> a pasa a ser
//a | b ; a < b && c >> a
{
    return (c=='`' && prev!=' ') || (c!=' ' && prev=='`')
    || (c==';' && prev!=' ') || (c!=' ' && prev==';')
    || ((c=='|' || c=='&' || c=='>') && prev!=' ' && prev!=c) ||
    (c!=' ' && c!=prev && (prev=='|' || prev=='&' || prev=='>'))
    || (c=='<' && prev!=' ') || (c!=' ' && prev=='<');
    
}


 /*
  separa comandos y argumentos
  ejemplo: se separan los argumentos a partir de los espacios en blanco, echo: this dog llama a echo con dos argumentos "this" y "dog"
 */
 char **splitLineCommand(char *line){
     int bufferSize=TOKBUFFERSIXE;
     int position=0;
     char **tokens=malloc(bufferSize * sizeof(char*));
     char *token;
     //errores de allocate
      if(!tokens){
         
        fprintf(stderr,"allocationError \n");
        exit(EXIT_FAILURE);
      }
      token=strtok(line,TOKDELIM);

      while(token!=NULL){
        tokens[position]=token;
        position++;

        if(position>=bufferSize){
            bufferSize+=TOKBUFFERSIXE;
            tokens=realloc(tokens,bufferSize*sizeof(char*));
            //errores de allocate
            if(!tokens){
                fprintf(stderr,"allocationError \n");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL,TOKDELIM);
    }
    tokens[position]=NULL;
    
    return tokens;
}
/*
para llevar procesos que se ejecutan en el shell
*/
int launch(char **args){
    pid_t pid, wpid;
    int status;
    pid=fork();
    if(pid==0){
        //proceso hijo
        if(execvp(args[0],args)==-1){
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid<0){
        //error fork
        perror("lsh");
    }
    else{
        //parent
        do{
            wpid=waitpid(pid,&status,-1);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
 }
void special_set_parse(char *buf, char **argv, int *argc)
{
    int line_length = 0;
    while (buf[line_length] != 10 && buf[line_length] != '#')
        line_length++;

    for (int i = 0; i < MAXARGS; i++)
        argv[i] = NULL;

    int pos = 0;
    while (buf[pos] == ' ')
        pos++;

    char token[MAXLINE];
    int cont = 0;
    while (pos != line_length && (*argc) < 2)
    {
        int ini_pos = pos;
        while (pos < line_length && buf[pos] != ' ')
        {
            token[pos - ini_pos] = buf[pos];
            pos++;
        }
        token[pos - ini_pos] = '\0';
        // printf("%d\n", pos-ini_pos);
        argv[*argc] = (char *)malloc((pos - ini_pos) * sizeof(char));
        for (int i = 0; i <= pos - ini_pos; i++)
            argv[*argc][i] = token[i];
        (*argc)++;

        while (pos < line_length && buf[pos] == ' ')
            pos++;
    }

    if (pos < line_length)
    {
        argv[*argc] = malloc(line_length - pos);
        for (int i = pos; i < line_length; i++)
            argv[*argc][i - pos] = buf[i];
        (*argc)++;
    }
}

int basic_parse(char *buf, char **argv, int *argc){

    if (!strcmp(argv[0], "set"))
    {
        (*argc) = 0;
        special_set_parse(buf, argv, argc);
    }

}
