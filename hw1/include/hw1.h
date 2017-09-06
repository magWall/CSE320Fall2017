#ifndef HW_H
#define HW_H

#include "const.h"

int isNum(char *argv); //pass in regular string to check, return 0 if not num/false

int convertToNum(char *argv);
int hasDuplicateCharacter(char *argv); //returns 0 if false + no duplicate, 1 if yes
int numChars(char *string); // takes a string and returns the number of characters in it, stopping at null char
int comparePolybiusAlphabetChar(char oneChar); //checks 1 char to see in -p alphabet
int comparePolybiusAlphabetAll(char *string); //checks to see if all chars in given string in polybius alphabet
int compareFractionatedMorseChar(char oneChar); //checks 1 char to see in -f alphabet
int compareFractionatedMorseAll(char *string); //checks to see if all chars in given string in -f alphabet
#endif
