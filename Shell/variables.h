#include <stdio.h>
#include <string.h>
#define MAXLINE 256
#define MAXVARS 256
#define MIN(a, b) (a < b) ? a : b


struct VariablesList
{
    int cant;
    char names[MAXVARS][MAXLINE];
    char values[MAXVARS][MAXLINE];
} vlist;

void push_variable(struct VariablesList *list, char *var_name, char *var_value)
{
    //int last_pos = 0;
    for (int i = 0; i < list->cant; i++)
    {
        if (!strcmp(var_name, list->names[i]))
        {
            strcpy(list->values[i], var_value);
            return;
        }
    }
    int pos = MIN(list->cant, MAXVARS - 1);
    strcpy(list->values[pos], var_value);
    strcpy(list->names[pos], var_name);
    list->cant++;
}

void pop_variable(struct VariablesList *list, char *var_name)
{
    int is_variable = 0;
    int last_pos = 0;
    for (int i = 0; i < vlist.cant; i++)
    {
        if (!strcmp(var_name, list->names[i]))
        {
            is_variable = 1;
            last_pos = i;
            break;
        }
    }

    if (!is_variable)
        return;

    for (int i = last_pos; i < MAXVARS - 1; i++)
    {
        strcpy(list->names[i], list->names[i + 1]);
        strcpy(list->values[i], list->values[i + 1]);
    }
    list->cant--;
}

void print_variables(struct VariablesList *list)
{
    if(list->cant ==0)
        printf("No hay variables aun\n");

    for (int i = 0; i < list->cant; i++)
        printf("%s = %s\n", list->names[i], list->values[i]);
}

void print_variable_value(struct VariablesList *list, char *var_name)
{
    for (int i = 0; i < list->cant; i++){
        if (!strcmp(var_name, list->names[i]))
        {
            printf("%s\n", list->values[i]);
            break;
        }
    }
}

