#ifndef HW_H
#define HW_H

#include "const.h"
#include <unistd.h>

int isNum(char *argv); //pass in regular string to check, return 0 if not num/false

int convertToNum(char *argv);
int hasDuplicateCharacter(char *argv); //returns 0 if false + no duplicate, 1 if yes
int numChars(char *string); // takes a string and returns the number of characters in it, stopping at null char
int numCharsConst(const char *string); //cons version of numChars
int compareKey(char oneChar); //compares Key with char
int comparePolybiusAlphabetChar(char oneChar); //checks 1 char to see in -p alphabet
int comparePolybiusAlphabetAll(char *string); //checks to see if all chars in given string in polybius alphabet
int compareFractionatedMorseChar(char oneChar); //checks 1 char to see in -f alphabet
int compareFractionatedMorseAll(char *string); //checks to see if all chars in given string in -f alphabet
char* findCharPolybiusTable(char oneChar,int colLen, int rowLen); //returns position of char in polybius table as 2 chars
char* findCharPolybiusTable2(char oneChar, char twoChar, int rowLen, int colLen); //returns char of given position, parameters are char to account for A-F
void fillPolybiusTable(int rowLength, int colLength);
int polybius_encrypt(int rowLength, int colLength); //returns -1 if error, returns 1 otherwise
int polybius_decrypt(int rowLength,int colLength); // decrypts message, -1 if character invalid
void fill_fm_key(void); //fills fm_key with fm_alphabet and key if provided
int compareFractionatedMorseTableString(char* string); //compares and returns idx of fm_key  where string of morse characters matches the string of fractionated morse characters
void clearPolybiusTable(void); //sets all values to '\0'
void shiftPolybiusTableMorseEncoding(void); //shift all characters by 3 to remove 1 'character' via morse
void storeMorseIntoPolybius(char* string); //stores string into polybius table as buffer
char* grabMorseChar(void);//changes the first 3 characters of polybius table into a character from fm_key through the fractionated table

int fm_encrypt(void); //encrypts into stdin
int fm_decrypt(void);
#endif
