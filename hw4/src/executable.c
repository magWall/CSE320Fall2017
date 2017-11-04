#include "executable.h"
#include "builtin.h"



void executeRelative(char* args)
{
    //it sounds like execvp can be used for executableProgram too?
    //first arg == command
    //remaining args == argv

    char** words = splitStr(args," ");
    char* cmd = *(words);

    // execvp(const char *file, char *const argv[]) where file == string
    execvp(cmd, words);
    free(words);
    //wait this is so stupid, I wrote a bazillon lines of code and this new program literally runs everything I need in a few lines
}


void executableProgram(char* args)
{
    //get count of all words in args
    //then set them into executable

    pid_t pid;
    int status;
    if( (pid = Fork()) ==0 )
    {
        int numargs = 0;
        char** words = splitStr(args," ");
        while( *(words+numargs)!= '\0' ) //not null limimt
        {
            numargs++;
        }
        char* allArgs[numargs+1]; //include null character to run execve
        allArgs[0] = *words;

        int idx =1;
        while( *(words+idx)!=0) //for args -i -r -etc
        {
            allArgs[idx]=*(words+idx);
            idx++;
        }
        allArgs[numargs+1]=NULL;
        char* allPaths = getenv("PATH");
        char** allDir = splitStr(allPaths,":");
        execve(allArgs[0],allArgs,allDir);//path, arg
        free(words);
    }
    pid = wait(&status);

}


char** splitStr(char* stringToSplit, char* delim)
{
    char** spaceOfWords = malloc(512);
    if(spaceOfWords==NULL)
    {
        fprintf(stderr, "%s\n", "FAIL TO MALLOC, NO SPACE");
    }
    char* word;
    int count=0; //total words

    word = strtok(stringToSplit,delim);//may have to check for delimiter same spaces
    if(word!=NULL && strlen(word)!=0) //strlen for extra space handling, it might be redudant though
    {
        *(spaceOfWords+count) = word;
        count++; //already tokenized , so add 1 to count if length >0
    }
    while(word!=NULL)
    {
       //may need to realloc here... but idk
        word = strtok(NULL,delim);
        *(spaceOfWords+count) = word;
        count++;
    }
    return spaceOfWords; //MALLOC'D POINTER, REMEMBER TO FREE.

}