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
    if(argc>1 && **(argv+1)== '-' && *(*(argv+1)+1)=='h' && *(*(argv+1)+2) ==0 ) //if  2nd arg is '-h' and not anything like '-h2'
        return 0x8000;

    if(argc>1 && argc<3) //only has -f or -p
        return 0;
    if(**(argv+1)== '-' && *(*(argv+1)+1)=='p' && *(*(argv+1)+2) ==0) //for -p cypher
    {
        if( ! ((**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) ==0) || (**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) ==0)) ) //check if correct encrypt/decrypt key
            return 0;
        /*code here for checking if d or if e, changing bits*/
        if(**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) ==0) //if d, it should be 0010 0000 0000 0000,since F is 0, then we have 0010, or 0x2000
            tmpshort = tmpshort || 0x2000;
       //if e, it should be 0000 0000 0000 0000,since F is 0, then we have 0000, or 0x0000 which means don't bitwise or anything

        return tmpshort;
    }
    if(**(argv+1)== '-' && *(*(argv+1)+1)=='f' && *(*(argv+1)+2) ==0) // for -f cypher
    {
        if( ! ((**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) ==0) || (**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) ==0)) ) //check if correct encrypt/decrypt key
            return 0;
        if(argc>5) //there can be max 4 arguments here, bin/hw1, -f, -e/-d, -k, key
            return 0;
        /*code here for checking if d or if e, changing bits*/
        if(**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) ==0) //if d, it should be 0010 0000 0000 0000,since F is 1, then we have 0110, or 0x6000
            tmpshort = tmpshort || 0x6000;//if d, it should be 0010 0000 0000 0000, but we also need to include F, so it is 0110, or 0x6000
        else if(**(argv+2)== '-' && *(*(argv+2)+1)=='e' && *(*(argv+2)+2) ==0) //if e, it should be 0000 0000 0000 0000,since F is 1, then we have 0100, or 0x4000
            tmpshort = tmpshort || 0x4000;
        if(argc>=4 && !(**(argv+3) == '-' && *(*(argv+3)+1)=='k') ) //if there's over 4 args, then there should be a -k and a key arg. If the 4th arg is not -k then invalid arg
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
