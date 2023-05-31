

int actual_foreground_pid; /* Process ID of the actual foreground process */
int last_actual_foreground_pid; /*Process ID of the last actual foreground process when ^C pressed*/
char *history_file_path;   /* path of the history.txt file */

int len(char** args)
{
    int i=0;
    while(args[i]!=NULL)
        i++;
    return i;
    
}
char* history_file_path_creation()
{
    char *hist_file_name = "/history.txt";
    char *home = getenv("HOME");
    char *history_file_path;

    history_file_path = malloc(strlen(home) + strlen(hist_file_name));
    for (int i = 0; i < strlen(home); i++)
        history_file_path[i] = home[i];
    for (int i = 0; i < strlen(hist_file_name); i++)
        history_file_path[strlen(home) + i] = hist_file_name[i];
    
    return history_file_path;
}

