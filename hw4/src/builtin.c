#include "builtin.h"
#include "debug.h"
#include <sys/wait.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void cd(char* arg)
{
    if(arg == NULL)
    {
        char* envPWD = getenv("PWD");
        setenv("OLDPWD",envPWD,1);
        char* envirHome = getenv("HOME"); //path to home
        chdir(envirHome);//change directory to home
        setenv("PWD",envirHome,1); //1 to overwrite old path
    }
    else if( strcmp(arg,"-")==0)
    {
        char* oldpwdPath = getenv("OLDPWD"); //prev pwdPath
        if( strcmp(oldpwdPath,"")!=0 ) // check to see if there is an prev path, if not, then it's blank
        {
            char* pwdPath = getenv("PWD");
            chdir(oldpwdPath);
            setenv("PWD",oldpwdPath,1); //overwrites prev. working directory into current.
            setenv("OLDPWD",pwdPath,1);
        }
        else
            fprintf(stderr,"OLDPWD not set.");
    }
    else if( strncmp(arg,"..",2)==0)
    {
        //check to see if it's ../-somepath- or just ..
    }
    else if( strncmp(arg, ".",1)==0)
    {
        //check to see if it's ./ or just . because . does nothing
    }


}
