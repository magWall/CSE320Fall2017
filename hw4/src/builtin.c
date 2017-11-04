#include "builtin.h"
#include "debug.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
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

        char* currPath = getenv("PWD"); //get current PWD to modify and set to old PWD
        char newPath[256] = {0};
        char* newPathPtr = newPath;

        char* tmpPath = malloc(256);
        strcpy(tmpPath,currPath); //used for tokenization to get len of last directory
        char* tokens = strtok(tmpPath, "/");
        int lenLastDir;
        while(tokens != NULL)
        {
            lenLastDir = strlen(tokens);
            tokens = strtok(NULL, "/");
        } //to get how many  ../ whatever/whatever2/lastDir      need to remove length of lastDir +1 because of '/' to get ../whatever/whatever2
        strncpy(newPathPtr, currPath, (strlen(currPath)-(lenLastDir+1) ) );
        //fprintf(stdout,"%s\n",newPathPtr);
        free(tmpPath);
        //check to see if it's ../-somepath- or just ..
        if(strlen(arg)>2) //go back and access another directory
        {
            int currPathLen = (strlen(currPath)-(lenLastDir+1)); //from removing prev. directory
            int idx=2;
            while(*(arg+idx)!='\0')
            {
                *(newPathPtr+currPathLen+idx-2)= *(arg+idx);
                idx++;
            }
            *(newPathPtr+currPathLen+idx-2)='\0'; //set null char
            if(chdir(newPathPtr)!= -1)
            {
                setenv("PWD",newPathPtr,1); //overwrites prev. working directory into current.
                setenv("OLDPWD",currPath,1);
            }
            else
            {
                fprintf(stderr,BUILTIN_ERROR,"Cannot go back to a non-existent directory");
            }
        }
        else //just go back to prev. dir.
        {
            if(chdir(newPathPtr)!= -1)
            {
                setenv("PWD",newPathPtr,1); //overwrites prev. working directory into current.
                setenv("OLDPWD",currPath,1);
            }
            else
            {
                fprintf(stderr,BUILTIN_ERROR,"Cannot go back to a non-existent directory");
            }

        }
    }
    else if( strncmp(arg, ".",1)==0)
    {
        //check to see if it's ./ or just . because . does nothing
        char* currPath = getenv("PWD"); //get current PWD to modify and set to old PWD
        char newPath[256] = {0};
        char* newPathPtr = newPath; //create new pathlength
        strcpy(newPathPtr,currPath);


        int lenPwd= strlen(currPath); //increment idx to not ruin current path directory when creating new path directory
        int idx =1; //idx to replace str
        while( *(arg+idx) !='\0') //   str= ./abcd/anotherDir  .
        {
            *(newPathPtr+lenPwd+idx-1) = *(arg+idx);
            idx++;
        }
        *(newPathPtr+idx+lenPwd-1)='\0'; //set null char
        DIR* tmpDir = opendir(currPath);
        if(tmpDir != NULL)
        {
            closedir(tmpDir);
            if (chdir(newPathPtr) != -1)
            {
                setenv("OLDPWD", currPath,1);
                setenv("PWD", newPathPtr, 1);
            }
            else
            {
                fprintf(stderr, BUILTIN_ERROR, "Cannot open path Directory specified");

            }
        }
        else
        {
            fprintf(stderr, BUILTIN_ERROR, "Cannot open current Directory"); //this is probably redundant
        }


    }
    else //regular cd
    {
        char* currPath = getenv("PWD"); //for setting to old PWD
        char newPath[256] = {0};
        char* newPathPtr = newPath;
        strcpy(newPathPtr,currPath);
        strcat(newPathPtr,"/");
        strcat(newPathPtr,arg); //add currPath and then the extra path
        //test
        DIR* tmpDir = opendir(newPathPtr);
        if(tmpDir != NULL)
        {
            closedir(tmpDir);
            chdir(newPathPtr);
            setenv("OLDPWD", currPath,1);
            setenv("PWD", newPathPtr, 1);
        }
        else
        {
            fprintf(stderr, BUILTIN_ERROR, "Cannot open path Directory specified");
        }
    }


}

