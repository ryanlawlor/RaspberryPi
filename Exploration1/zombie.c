#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

char *read_command()
{
    int bufsize = 4;
    char *buffer = malloc(sizeof(char) * bufsize); // initial allocation of memory
    int pos = 0;

    while(1)
    {
        if(pos > bufsize)
        {
            bufsize = bufsize * 2;
            buffer = realloc(buffer, sizeof(char) * bufsize); // reallocation of memory if input gets longer
        }
        char c = getchar();
        if(c != '\n')
        {
            buffer[pos++] = c; // recording user input
        }
        else
        {
            buffer[pos] = '\0'; // ending input with null terminator when enter is pressed
            return buffer;
        }
    }
}

void print_prompt()
{
    char cwd[32];
    getcwd(cwd, sizeof(cwd)); // gets current working directory to display in prompt
    char* token = strtok(cwd, "/");
    char* lastdir = token;
    while(token != NULL)
    {
        lastdir = token; // loops through each location in directory to find last current folder or location
        token = strtok(NULL, "/");
    }
    printf("person@machine:%s $ ", lastdir); // displays prompt
}

int is_built_in(char* cmd)
{
    if(strcmp(cmd, "exit") == 0) // checks if the command is exit
    {
        return 1;
    }
    else if(strcmp(cmd, "cd") == 0) // checks if the command is cd
    {
        return 1;
    }
    else
    {
        return 0; // command is neither of those
    }
}

int do_built_in(char* cmd, char* param)
{
    if(strcmp(cmd, "exit") == 0)
    {
        exit(0); // executes the exit() command if user wants to exit
    }
    else if(strcmp(cmd, "cd") == 0)
    {
        if (chdir(param) != 0) // executes the chdir() command if user wants to change directory
        {
            perror("chdir() failed"); // error message if directory not found
        }
    }
    return 0;
}

int main()
{
    int n = 4;
    for (int i = 0; i < n; i++)
    {
        pid_t child_pid = fork();
        if(child_pid != 0)
        {
            
        }
        else
        {
            exit(0);
        }
    }
    getchar();

    return 0;
}