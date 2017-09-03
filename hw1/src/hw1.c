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

    if(argc>1 && argc<3) //only has -f or -p
        return 0;
    if(**(argv+1)== '-' && *(*(argv+1)+1)=='p' && *(*(argv+1)+2) =='\0') //for -p cypher
    {
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
        if( !(argc= 3 || argc==5 || argc ==7 || argc == 9) ) //invalid # of args if doesn't pass
            return 0x0000;
        if(argc ==5)
        {
            if( **(argv+3) == '-' && *(*(argv+3)+1)=='k' && *(*(argv+2)+2) =='\0') //default column and row if -k
                tmpshort =  tmpshort | 0x00AA;
            else if( **(argv+3) == '-' && *(*(argv+3)+1)=='c' && *(*(argv+2)+2) =='\0') //if -c, pass in column
            {
                if(isNum(*(argv+4)) //check to see if -c's # arg has no letters, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertedNum(*(argv+4));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                        return 0;
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    tmpshort = tmpshort|0xA0; //set row to 10
                }
                else
                    return 0;
            }
            else if(( **(argv+3) == '-' && *(*(argv+3)+1)=='r' && *(*(argv+2)+2) =='\0'))
            {
                //get row, set col to 10
                if(isNum(*(argv+4)) //check to see if -c's # arg has no letters, else fail it
                {
                    //get col, set row to 10, may need atoui to count full length of arg being passed in
                    int numInDecimal = convertedNum(*(argv+4));
                    //rows and columns must be between 9 and 15 inclusive. if over or under, invalid arg.
                    if(numInDecimal<9 || numInDecimal>15)
                        return 0;
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    tmpshort = tmpshort|0xA;
                }
                else
                    return 0;
            }
            else
                return 0x0000; //argument invalid
        }
        else if(argc ==7)
        {

        }
        else if(argc==9)
        // if it is only 3 args, then column and row set to 10
        // if 5 args, check 4th arg, if it's -k, then set col and row to 10 , else if row or col, set that to the number given
        // repeat for 7 and 9
        //assume it passes the test, check for all  individual cases



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
        if(argc>=4 && !(**(argv+3) == '-' && *(*(argv+3)+1)=='k'&& *(*(argv+2)+2) =='\0') ) //if there's over 4 args, then there should be a -k and a key arg. If the 4th arg is not -k then invalid arg
            tmpshort = 0x0000;
        return tmpshort;
    }
    /*if argc >1, check for -p flag, else check for -f flag
     *if first passed in arg is not -h, or -p, or -f, then immediately return 0
     *if first arg is -p, check for -d and -e
     *if first arg is -f, check for -d and -e
     */


    return 0;
}
