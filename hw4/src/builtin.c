#include "builtin.h"
#include "debug.h"
#include <sys/wait.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

pid_t Fork()
{
    pid_t pid;
    if( (pid=fork())<0 )
    {
        write(STDERR_FILENO,"fork error\n",11);
    }
    return pid;

}
void printHelp()
{
    pid_t pid=0;
    int status;
    if( (pid=Fork())==0 )
    {//221
        write(STDOUT_FILENO,"The Shell\n"                                      \
            "These commands are built-in. Type 'help' to see this list. \n"    \
            "cd [-L|[-P [-e]] [-@]] [dir]      ->change directory\n"           \
            "help        ->Displays this usage menu.\n"                        \
            "pwd         ->Print working directory\n"                          \
            "exit        ->Exits the Shell\n"                                  \
            ,231);
        exit(0); //10 + 60 + 53+40+38+30 = 231
    }
    wait(&status);
}

void printPwd()
{
    char* buffer; //store pwd
    pid_t pid;
    int status;

    if( (pid = Fork()) ==0 )
    {
        buffer = getcwd(NULL, 0);
        fprintf(stdout, "%s\n",buffer);
        exit(0);
    }
    wait(&status);

}

