#ifndef BUILT_IN_H
#define BUILT_IN_H
#include <unistd.h>

#include "sfish.h"

pid_t Fork();
void printHelp();
void printPwd();
void cd(char* arg);
#endif
