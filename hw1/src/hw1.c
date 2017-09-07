#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif




int isNum(char *argv) //0 is false, anything else is true according to C convention
{
    int idx=0;
    while(*(argv+idx) != '\0') // '\0' = null character
    {
        if(  ((int)(*(argv+idx))) <48 || ((int)(*(argv+idx))) >57)  //ascii ranges 48 to 57 for 0 to 9
            return 0;
        idx++;
    }
    return 1;
}

int convertToNum(char *argv)
{
    int convertedNum=0;
    int idx=0;
    while(*(argv+idx) != '\0') // '\0' = null character
    {
        convertedNum=convertedNum*10;
        convertedNum+=*(argv+idx);
        idx++;
    }
    return convertedNum;
}
int hasDuplicateCharacter(char *argv)
{
    //return 0 if duplicate, 1 otherwise
    int idx = 0;
    int idx2 = idx+1;
    while( *(argv+idx) != '\0')  //basic nested loop to check each character with itself, O(n^2)
    {
        while( *(argv+idx2) != '\0')
        {
            if( *(argv+idx) == *(argv+idx2) )
                return 0;
            idx2++;
        }
        idx++;
        idx2=idx+1;
    }
    return 1;

}
int numChars(char *string)
{
    int numCharsRead = 0;
    while( *(string+numCharsRead) != '\0')
    {
        numCharsRead++;
    }
    return numCharsRead;
}
int comparePolybiusAlphabetChar(char oneChar)   //boolean 0 = false, 1 = true
{
    int idx = 0;
    while( *(polybius_alphabet+idx) != '\0')
    {
        if(oneChar == *(polybius_alphabet+idx))
            return 1;
        idx++;
    }
    return 0;
}
int comparePolybiusAlphabetAll(char *string)    //boolean 0 = false, 1 = true
{
    int idx=0;
    while( *(string+idx)!=0)
    {
        if(comparePolybiusAlphabetChar(*(string+idx)) == 0)
            return 0;
        idx++;
    }
    return 1;
}
int compareFractionatedMorseChar(char oneChar)  //boolean 0 = false, 1 = true
{
    int idx = 0;
    while( *(fm_alphabet+idx) != '\0')
    {
        if(oneChar == *(fm_alphabet+idx))
            return 1;
        idx++;
    }
    return 0;
}
int compareFractionatedMorseAll(char *string)   //boolean 0 = false, 1 = true
{
    int idx=0;
    while( *(string+idx)!=0)
    {
        if(compareFractionatedMorseChar(*(string+idx)) == 0)
            return 0;
        idx++;
    }
    return 1;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the program
 * and will return a unsigned short (2 bytes) that will contain the
 * information necessary for the proper execution of the program.
 *
 * IF -p is given but no (-r) ROWS or (-c) COLUMNS are specified this function
 * MUST set the lower bits to the default value of 10. If one or the other
 * (rows/columns) is specified then you MUST keep that value rather than assigning the default.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return Refer to homework document for the return value of this function.
 */
unsigned short validargs(int argc, char **argv) {
    unsigned short tmpshort = 0x0000;
    if(argc==1) //args = blank , only has bin/hw1,  return 0 and exit failure
        return 0;
    if(argc>1 && **(argv+1)== '-' && *(*(argv+1)+1)=='h' && *(*(argv+1)+2) =='\0' ) //if  2nd arg is '-h' and not anything like '-h2'
        return 0x8000;

    if(argc>=1 && argc<3) //only has -f or -p
        return 0;
    if(**(argv+1)== '-' && *(*(argv+1)+1)=='p' && *(*(argv+1)+2) =='\0') //for -p cypher
    {
        int rowLength = 10;
        int colLength = 10;
        int cArg =0;    //decided to use flags because since optional args can be placed in any order, there will be a lot of repositioning conditional statements to handle
        int rArg =0;
        int kArg =0;

        if( ! ((**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) =='\0') || (**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) =='\0')) ) //check if correct encrypt/decrypt key
            return 0;
        /*code here for checking if d or if e, changing bits*/
        if(**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) =='\0') //if d, it should be 0010 0000 0000 0000,since F is 0, then we have 0010, or 0x2000
            tmpshort = tmpshort | 0x2000;
       //if e, it should be 0000 0000 0000 0000,since F is 0, then we have 0000, or 0x0000 which means don't bitwise or anything
        /* then check if you have 5, 7, or 9 arguments. If the argc value is not equal to 3, 5, 7 or 9, it is invalid.
         * bin/hw1 -p -e/-d -k key -r # -c #
         * has 9 possible arguments if bin/hw1 is included
         */
        if( !(argc== 3 || argc==5 || argc ==7 || argc == 9) || argc>9 ) //invalid # of args if doesn't pass
            return 0x0000;
        if(argc ==3 ) //default col/row = 10
        {
            tmpshort = tmpshort | 0x00AA;
        }
        else if(argc >=5)
        {
            if( **(argv+3) == '-' && *(*(argv+3)+1)=='k' && *(*(argv+2)+2) =='\0') //default column and row if -k
            {
                if(hasDuplicateCharacter( *(argv+4)) ==0 )
                    return 0;
                if(comparePolybiusAlphabetAll( *(argv+4))== 0)
                    return 0;
                //true if passses tests, pass string in
                key = *(argv+4);
                kArg =1; //flag raised

            }
            else if( **(argv+3) == '-' && *(*(argv+3)+1)=='c' && *(*(argv+3)+2) =='\0') //if -c, pass in column
            {
                if(isNum(*(argv+4))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+4));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                    {
                        return 0;
                    }
                    colLength = numInDecimal;
                    if(colLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cFlag=1; //cFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else if( **(argv+3) == '-' && *(*(argv+3)+1)=='r' && *(*(argv+3)+2) =='\0' )
            {
                //get row, set col to 10
                if(isNum(*(argv+4))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+4));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                        return 0;
                    rowLength = numInDecimal;
                    if(rowLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rFlag=1; //rFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else
            {
                return 0x0000; //argument invalid for arg5 or repeated args aka args not -k -r -c
            }
        }

        if(argc >=7)
        {
            if( **(argv+5) == '-' && *(*(argv+5)+1)=='k' && *(*(argv+5)+2) =='\0' && kArg==0) //default column and row if -k
            {
                if(hasDuplicateCharacter( *(argv+6)) ==0 )
                    return 0;
                if(comparePolybiusAlphabetAll( *(argv+6))== 0)
                    return 0;
                //true if passses tests, pass string in
                key = *(argv+6);
                kArg =1; //flag raised

            }
            else if( **(argv+5) == '-' && *(*(argv+5)+1)=='c' && *(*(argv+5)+2) =='\0' && cArg==0) //if -c, pass in column
            {
                if(isNum(*(argv+6))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+6));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                    {
                        return 0;
                    }
                    colLength = numInDecimal;
                    if(colLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cFlag=1; //cFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else if( **(argv+5) == '-' && *(*(argv+5)+1)=='r' && *(*(argv+5)+2) =='\0' &&rArg==0)
            {
                //get row, set col to 10
                if(isNum(*(argv+6))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+6));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                        return 0;
                    rowLength = numInDecimal;
                    if(rowLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rFlag=1; //rFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else
            {
                return 0x0000; //argument invalid for arg5 or repeated args aka args not -k -r -c
            }
        }
        if(argc==9)
        {
            if( **(argv+7) == '-' && *(*(argv+7)+1)=='k' && *(*(argv+7)+2) =='\0' && kArg==0) //default column and row if -k
            {
                if(hasDuplicateCharacter( *(argv+8)) ==0 )
                    return 0;
                if(comparePolybiusAlphabetAll( *(argv+8))== 0)
                    return 0;
                //true if passses tests, pass string in
                key = *(argv+8);
                kArg =1; //flag raised

            }
            else if( **(argv+7) == '-' && *(*(argv+7)+1)=='c' && *(*(argv+7)+2) =='\0' && cArg==0) //if -c, pass in column
            {
                if(isNum(*(argv+8))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+8));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                    {
                        return 0;
                    }
                    colLength = numInDecimal;
                    if(colLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cFlag=1; //cFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else if( **(argv+7) == '-' && *(*(argv+7)+1)=='r' && *(*(argv+7)+2) =='\0' &&rArg==0)
            {
                //get row, set col to 10
                if(isNum(*(argv+8))) //check to see if -c's # arg has only numbers, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertToNum(*(argv+8));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                        return 0;
                    rowLength = numInDecimal;
                    if(rowLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
                        return 0;
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rFlag=1; //rFlag raised
                }
                else
                {
                    return 0; // not a num
                }
            }
            else
            {
                return 0x0000; //argument invalid for arg5 or repeated args aka args not -k -r -c
            }
        }
        if(cArg ==0)
        {
            tmpshort = tmpshort |0xA;
        }
        if(rArg ==0)
        {
            tmpshort = tmpshort | 0xA0;
        }



        return tmpshort;
    }
    if(**(argv+1)== '-' && *(*(argv+1)+1)=='f' && *(*(argv+1)+2) =='\0') // for -f cypher
    {
        if( ! ((**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) =='\0') || (**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) =='\0')) ) //check if correct encrypt/decrypt key
            return 0;
        if(argc>5) //there can be max 4 arguments here, bin/hw1, -f, -e/-d, -k, key
            return 0;
        /*code here for checking if d or if e, changing bits*/
        if(**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) =='\0') //if d, it should be 0010 0000 0000 0000,since F is 1, then we have 0110, or 0x6000
            tmpshort = tmpshort | 0x6000;//if d, it should be 0010 0000 0000 0000, but we also need to include F, so it is 0110, or 0x6000
        else if(**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) =='\0') //if e, it should be 0000 0000 0000 0000,since F is 1, then we have 0100, or 0x4000
            tmpshort = tmpshort || 0x4000;
        else
            return 0;
        if(argc>3) //if argc >3, then check for -k args, if argc ==4, then fail. if argc >5, fail. If not -k, fail.
        {
            if(argc==5 && (**(argv+3) == '-' && *(*(argv+3)+1)=='k'&& *(*(argv+2)+2) =='\0') ) //if there's over 4 args, then there should be a -k and a key arg. If the 4th arg is not -k then invalid arg
            {
                if(hasDuplicateCharacter( *(argv+4)) ==0 )
                    return 0;
                if(compareFractionatedMorseAll( *(argv+4))== 0)
                    return 0;
                //true if passses tests, pass string in
                key = *(argv+4);
            }
             else
            {
                return 0;
            }
        }

        return tmpshort;
    }
    /*if argc >1, check for -p flag, else check for -f flag
     *if first passed in arg is not -h, or -p, or -f, then immediately return 0
     *if first arg is -p, check for -d and -e
     *if first arg is -f, check for -d and -e
     */


    return 0;
}
