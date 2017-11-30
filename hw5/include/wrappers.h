#ifndef WRAPPERS_H
#define WRAPPERS_H
#include <stdlib.h>
#include <stdio.h>

void unix_error(char *msg);
void Sem_init(sem_t* t, int num, int num2);
void P(sem_t* sem);
void V(sem_t* sem);
#endif