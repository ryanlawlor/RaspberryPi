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
    while(1)
    {
        print_prompt();
        char* input = read_command();
        if(strcmp(input, "") != 0) // checks to see if user enters nothing
        {
            char **a = malloc(5*sizeof(char *)); // creates array for command and parameters
            int p = 0;
            char* token = strtok(input, " ");
            while(token != NULL)
            {
                a = realloc(a, sizeof(char) * 2);
                a[p] = token; // loops through input tokens and adds to the array
                p++;
                token = strtok(NULL, " ");
            }

            if (is_built_in(a[0]))
            {
                do_built_in(a[0], a[1]); // checks and does built in commands (exit and cd)
            }
            else
            {
                pid_t child_pid = fork(); // saves the child pid
                if(child_pid != 0)
                {
                    waitpid(child_pid); // waits for child to finish and then resumes shell
                }
                else
                {
                    execvp(a[0], a); // executes user's command with first item of array as the command and the entire array as the arguments

                    printf("command not found\n"); // error message if execute doesn't work
                    exit(0);
                }
            }
        }
    }
}