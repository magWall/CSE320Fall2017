#ifndef EXECUTABLE_H
#define EXECUTABLE_H
#include "sfish.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void executableProgram();
char** splitStr(char* stringToSplit, char* delim); //returns a malloc'd string! REMEMBER TO FREE!!!



#endif