#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "sfish.h"
#include "debug.h"
#include "builtin.h"

int main(int argc, char *argv[], char* envp[]) {
    char* input;
    bool exited = false;
    char newpathName[256] = {0};
    char* workingDirectoryPath = getcwd(NULL,0); //...  home/student/kennylee/hw4

    //printf("%s\n", workingDirectoryPath);
    if(strncmp(workingDirectoryPath, "/home/student",13) ==0)
    {
        char* tokens = strtok(workingDirectoryPath,"/");
        tokens = strtok(NULL, "/");
        tokens = strtok(NULL, "/"); //remove home/student
        strcat(newpathName,"~"); //replace with ~
        while(tokens != NULL)
        {
            strcat(newpathName, "/");
            strcat(newpathName, tokens);
            tokens = strtok(NULL, "/");
        }

    }
    else
    {
        strcat(newpathName,workingDirectoryPath);
    }
    free(workingDirectoryPath);

    if(!isatty(STDIN_FILENO)) {
        // If your shell is reading from a piped file
        // Don't have readline write anything to that file.
        // Such as the prompt or "user input"
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }
    }

    strcat(newpathName,"::kennylee >> ");
    do {

        input = readline(newpathName);

        write(1, "\e[s", strlen("\e[s"));
        write(1, "\e[20;10H", strlen("\e[20;10H"));
        write(1, "SomeText", strlen("SomeText"));
        write(1, "\e[u", strlen("\e[u"));

        // If EOF is read (aka ^D) readline returns NULL
        if(input == NULL) {
            continue;
        }


        // Currently nothing is implemented
//        printf(EXEC_NOT_FOUND, input);
        char* delimiter = " ";
        char* words = strtok(input, delimiter);

        // You should change exit to a "builtin" for your hw.
        //exited = strcmp(input, "exit") == 0;

        if( strcmp(words, "exit")==0 )
        {
            break;
        }
        else if(strcmp(words, "help")==0)
            printHelp();
        else if(strcmp(words, "pwd")==0)
            printPwd();
        else if (strcmp(words, "cd")==0)
        {
            words = strtok(NULL, delimiter);
            cd(words);
        }

        // Readline mallocs the space for input. You must free it.
        rl_free(input);
        memset(newpathName, 0, 256); //clear buffer
        char* workingDirectoryPath = getcwd(NULL,0); //...  home/student/kennylee/hw4
        if(strncmp(workingDirectoryPath, "/home/student",13) ==0)
        {
            char* tokens = strtok(workingDirectoryPath,"/");
            tokens = strtok(NULL, "/");
            tokens = strtok(NULL, "/"); //remove home/student
            strcat(newpathName,"~"); //replace with ~
            while(tokens != NULL)
            {
                strcat(newpathName, "/");
                strcat(newpathName, tokens);
                tokens = strtok(NULL, "/");
            }

        }
        else
        {
            strcat(newpathName,workingDirectoryPath);
        }
        free(workingDirectoryPath);
        strcat(newpathName,"::kennylee >> ");

    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}
