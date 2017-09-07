#include <stdlib.h>

#include "hw1.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{
    unsigned short mode;

    mode = validargs(argc, argv);
    debug("num args: %d",argc);
    debug("Mode: 0x%X", mode);
    debug("stuff : %s",* (argv+1) );
    debug("stuff : %s",* (argv+1)+2);


    if(mode ==0){

        debug("failed: %s", *argv); //check to see if it hits this line
        USAGE(*argv, mode); //upon failure, may need to change [mode] to retcode
        return EXIT_FAILURE;
    }
    if(mode & 0x8000) { // if mode & 1000 0000 0000 0000 != 0, check for help

    debug("worked : %s",* (argv+1)+2); //check statement
        USAGE(*argv, EXIT_SUCCESS);
    }
    /*create a check for mode for p and f*/
    debug("argv+4: %s",*(argv+4) );
    debug("key: %s, %p",key,key);
    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */