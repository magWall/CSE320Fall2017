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
        convertedNum+=*(argv+idx)-48;
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

int numCharsConst(const char *string)
{
    int numCharsRead = 0;
    while( *(string+numCharsRead) != '\0')
    {
        numCharsRead++;
    }
    return numCharsRead;
}
int compareKey(char oneChar)
{
    int idx= 0;
    while(*(key+idx)!= '\0')
    {
        if( *(key+idx)== oneChar)
            return 1;
        idx++;
    }
    return 0;
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
char* findCharPolybiusTable(char oneChar, int colLen, int rowLen)
{
    int colNum =0;
    int rowNum =0;
    long space;
    char* tmpVar=(char*)&space;
    while(rowNum<rowLen)
    {
        while(colNum<colLen)
        {
            if(*(polybius_table +  colNum + colLen*rowNum)==oneChar)
            {
                if(rowNum>=10)
                {
                    *tmpVar=(char)(rowNum+55);
                }
                else
                {
                    *tmpVar=(char)(rowNum+48);
                }

                if(colNum>=10)
                {
                    *(tmpVar+1)=(char)(colNum+55);
                }
                else
                {
                    *(tmpVar+1)=(char)(colNum+48);
                }
                *(tmpVar+2)='\0';
                return tmpVar; //returns row then col as a 2 digit number (row,col)
            }
            colNum++;
        }
        colNum=0;
        rowNum++;
    }
    return "error"; //cannot find it if -1
}
char* findCharPolybiusTable2(char oneChar, char twoChar, int rowLen, int colLen)
{
    int rowNum=0;
    int colNum=0;
    long space;
    char* tmpVar=(char*)&space;
    if(oneChar>=65 && oneChar<=70)
    {
        rowNum= ((int)oneChar) -55; //65 = A , 65-55 = 10, A =10
    }
    else if(oneChar >=48 && oneChar<=57)
    {
        rowNum= ((int)oneChar)-48;
    }
    else
    {
        rowNum = -1;
    }
    if(twoChar>=65 && twoChar<=70)
    {
        colNum= ((int)twoChar) -55; //65 = A , 65-55 = 10, A =10
    }
    else if(twoChar >=48 && twoChar<=57)
    {
        colNum= ((int)twoChar)-48;
    }
    else
    {
        colNum = -1;
    }
    if(rowNum ==-1 || colNum == -1 || colNum>colLen || rowNum > rowLen)
        return "error";
    *(tmpVar)= *(polybius_table+  colNum +  rowNum*colLen);
    *(tmpVar+1)='\0';
    return tmpVar;
}
int compareFm_key(char oneChar)
{
    int idx=0;
    while(*(fm_key+idx) != '\0')
    {
        if(*(fm_key+idx) == oneChar)
            return idx;
        idx++;
    }
    return -1;
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
        if(**(argv+2)== '-' && *(*(argv+2)+1)=='d' && *(*(argv+2)+2) =='\0') //if d, it should be 0010 0000 0000 0000,since P is 0, then we have 0010, or 0x2000
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
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cArg=1; //cFlag raised
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
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rArg=1; //rFlag raised
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
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cArg=1; //cFlag raised
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
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rArg=1; //rFlag raised
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
                    tmpshort = tmpshort|numInDecimal; //bitwise or
                    cArg=1; //cFlag raised
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
                    tmpshort = tmpshort| (numInDecimal<<4); //shift left by 4 and then bitwise or
                    rArg=1; //rFlag raised
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
        if(colLength * rowLength < numChars(polybius_alphabet)) //if row*col length < alphabet characters, fail
            return 0;

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
            tmpshort = tmpshort | 0x4000;
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

    return 0;
}
void fillPolybiusTable(int rowLength, int colLength)
{
    if(key == NULL)
        {
            int colNum =0;
            int rowNum =0;
            int idxOfAlphabet = 0;
            while(rowNum<rowLength)
            {
                while(colNum<colLength)
                {
                    if(idxOfAlphabet < numChars(polybius_alphabet))
                    {
                        *(polybius_table +  colNum + colLength*rowNum)= *(polybius_alphabet+idxOfAlphabet);
                        idxOfAlphabet++;
                    }
                    else
                    {
                        *(polybius_table +  colNum + colLength*rowNum) = '\0';
                    }
                    colNum++;
                }
                colNum=0;
                rowNum++;
            }
        }
        else
        {
            int colNum =0;
            int rowNum =0;
            int idxOfAlphabet = 0;
            int keyLength = numCharsConst(key);
            int keyNum =0;
            while(rowNum<rowLength)
            {
                while(colNum<colLength)
                {
                    if(idxOfAlphabet < numChars(polybius_alphabet))
                    {
                        if(keyNum<keyLength)
                        {
                            *(polybius_table +  colNum + colLength*rowNum)= *(key+keyNum);
                            keyNum++;
                            colNum++;
                        }
                        else
                        {
                            if(compareKey(*(polybius_alphabet+idxOfAlphabet)) == 0)
                            {
                                *(polybius_table +  colNum + colLength*rowNum)= *(polybius_alphabet+idxOfAlphabet);
                                colNum++;
                            }
                            idxOfAlphabet++;
                        }

                    }
                    else
                    {
                        *(polybius_table +  colNum + colLength*rowNum) = '\0';
                        colNum++;
                    }
                }
                colNum=0;
                rowNum++;
            }
        }
}
int polybius_encrypt(int rowLength, int colLength)
{
    char inputChar = ' ';
    while( (inputChar = getchar())!=EOF)
        {
            if(inputChar== ' ' || inputChar == '\n' || inputChar == '\t')
            {
                printf("%c",inputChar);
            }
            else if (comparePolybiusAlphabetChar(inputChar)==0)
            {
                return -1; //stop and end
            }
            else
            {
                char* charPos = findCharPolybiusTable(inputChar, colLength, rowLength);
                printf("%s",charPos);
                //then function call to check if valid input and run encryption/decryption based on mode
            }
        }
        return 1;
}
int polybius_decrypt(int rowLength,int colLength)
{
    char inputChar = ' ';
    while( (inputChar = getchar())!=EOF)
        {
            if(inputChar== ' ' || inputChar == '\n' || inputChar == '\t')
            {
                printf("%c",inputChar);
            }
            //char must be not whitespace, and only characters accepted from input are those from encryped files
            else
            {

                char inputChar2 = getchar(); //2nd char of a number
                char* charPos = findCharPolybiusTable2(inputChar, inputChar2, rowLength, colLength);
                printf("%s",charPos);
                //then function call to check if valid input and run encryption/decryption based on mode
            }
        }
    return 1;

}
void fill_fm_key()
{
    int idx=0;
    if(key==NULL)
    {
        while(  *(fm_alphabet+idx) != '\0')
        {
            *(fm_key+idx) = *(fm_alphabet+idx);
            idx++;
        }
    }
    else
    {
        int fm_alphabetNum_idx =0;
        int keyLength = numCharsConst(key);
        int keyNum =0;
        while(keyNum <keyLength)
        {
            *(fm_key+idx) = *(key+keyNum);
            keyNum++;
            idx++;
        }
        while(*(fm_alphabet+fm_alphabetNum_idx)!= '\0')
        {
            if(compareKey(*(fm_alphabet+fm_alphabetNum_idx)) == 0)
            {
                *(fm_key+idx)=  *(fm_alphabet+fm_alphabetNum_idx);
                idx++;
            }
            fm_alphabetNum_idx++;
        }
        *(fm_key+idx)= '\0';//null terminator
    }
}
void clearPolybiusTable() //in case for random contents in polybius table
{
    int idx= 0;
    while(idx <sizeof(polybius_table) )
    {
        *(polybius_table+idx)='\0';
        idx++;
    }
}
void storeMorseIntoPolybius(char* string)
{
    int idx =0;
    int stringIdx =0;
    while( *(polybius_table+idx) !='\0') //avoid confliting charaacters
    {
        idx++;
    }
    while( *(string+stringIdx) !='\0') //add into empty places
    {
        *(polybius_table + idx) = *(string+stringIdx);
        idx++;
        stringIdx++;
    }
}
int compareFractionatedMorseTableString(char* string)
{
    int idx = 0;
    while( *(fractionated_table+idx)!= NULL) //if not null char pointer
    {
        if(**(fractionated_table+idx)== *(string) && *(*(fractionated_table+idx)+1)== *(string+1) && *(*(fractionated_table+idx)+2)== *(string+2))
            return idx;
        idx++;
    }
    return -1;
}
void shiftPolybiusTableMorseEncoding()
{
    int idx=3; //0 1 2 are morse codes to be removed
    if( *(polybius_table+idx)=='\0') //only 3 in the polybius table
        clearPolybiusTable();//empty it
    else
    {
        while(*(polybius_table+idx)!='\0' ) //shift everything by 3 to the left
        {
            *(polybius_table+idx-3)= *(polybius_table+idx);
            idx++;
        }
        *(polybius_table+idx-3)= '\0';
        *(polybius_table+idx-2)= '\0';
        *(polybius_table+idx-1)= '\0';

    }
}
char* grabMorseChar()
{
    long tmpSpace = 0;
    char* tmpMorseChar = (char*)&tmpSpace;
    *tmpMorseChar = *(polybius_table);
    *(tmpMorseChar+1) = *(polybius_table+1);
    *(tmpMorseChar+2) = *(polybius_table+2);
    int idxFmKey = compareFractionatedMorseTableString(tmpMorseChar);

    shiftPolybiusTableMorseEncoding();
    if(idxFmKey!= -1)
    {
        *tmpMorseChar = *(fm_key+idxFmKey);
        *(tmpMorseChar+1) = '\0';

        return tmpMorseChar;
    }
    //check if match, then return char
    //remember to move all characters back to 0 after grabbing first 3
    return "error grab morse char";
}
int checkForXXWhitespace() //returns idx, idx and idx+1 is 'xx', fail if -1
{
    int idx=0;
    char a = *(polybius_table+idx);
    char b = *(polybius_table+idx+1);
    while(*(polybius_table+idx) != '\0')
    {
        a = *(polybius_table+idx);
        b = *(polybius_table+idx+1);
        if(a == 'x' && a == b)
        {
            return idx;
        }
        idx++;
    }
    return -1;
}
int getDecryptedCharacterIdx(int posOfXX)//function assumes checkForXXWhitespace returns >=0  , might need to check for a case where there's no whitespace at the end of string and still parse
{
    int idx=0;
    long tmpSpace; //tmpSpace is 8 bytes
    char* tmpMorseChar = (char*)&tmpSpace;
    while(idx != posOfXX)
    {
        *(tmpMorseChar+idx) = *(polybius_table+idx);
        idx++;
    }
    *(tmpMorseChar+idx) = '\0';
    //length of tmpMorseChar = idx+1;
    int morseTableIdx = 0;
    while(*(morse_table+morseTableIdx)!= NULL)
    {
        if( idx == numCharsConst( *(morse_table+morseTableIdx))) //compares number of characters in tmpMorseChar to the number of characters in a morse_table string
        {
            int morseTableStringIdx=0;
            int matchedString = 0;
            while( *(*(morse_table+morseTableIdx)+morseTableStringIdx)!= '\0')
            {
                if( *(tmpMorseChar+morseTableStringIdx)== *(*(morse_table+morseTableIdx)+morseTableStringIdx))
                {
                    morseTableStringIdx++;
                    matchedString = 1;
                }
                else
                {
                    matchedString = 0;
                    break;
                }
            }
            if(matchedString==1)
            {
                return morseTableIdx;
            }
        }
        morseTableIdx++;
    }
    return -1;
}
void shiftPolybiusTableMorseEncodingDecryption(int idxToShift) //more efficient? ASSUME idxToShift shifts the length of translated morse chars, including the 'xx' whitespace characters
{
    int idx=0; //0 1 2 are morse codes to be removed
    if( *(polybius_table+idx)=='\0') //if there's a parse error I didn't encounter here it will be erased
        clearPolybiusTable();//empty it
    else
    {
        while(*(polybius_table+idx)!='\0' ) //shift everything by idxToShift to the left
        {
            *(polybius_table+idx)= *(polybius_table+idx+idxToShift);
            idx++;
        }

    }
}
int checkForLetterDelimiterX()
{
    int idx=0;
    char a = *(polybius_table+idx);
    while( *(polybius_table+idx)!= '\0' )
    {
        a = *(polybius_table+idx);
        if(a == 'x')
            return idx;
        idx++;
    }
    return -1;
}
int fm_encrypt()
{
    // use the polybius table as buffer since it is not used here
    clearPolybiusTable();
    char* tmpMorseChars;
    long tmpSpace;
    tmpMorseChars = (char*)&tmpSpace;
    *tmpMorseChars = 'x';
    *(tmpMorseChars+1) = '\0';//separate words with whitespace
    char inputChar= ' ';
    int spaceFlag = 0; //false whitespace
    while( (inputChar = getchar()) != EOF )
    {
        if( (inputChar == '\t' || inputChar ==' ') )
        {
            if(spaceFlag==0)
            {
                spaceFlag=1; //turn it on to prevent repetitive  whitespace
                storeMorseIntoPolybius(tmpMorseChars);
                while(numChars(polybius_table)>=3)
                {
                 printf("%s",grabMorseChar());
                }

            }
        }
        else if(inputChar == '\n' && spaceFlag == 0)
        {
            spaceFlag = 1;
            storeMorseIntoPolybius(tmpMorseChars);
            while(numChars(polybius_table)>=3)
            {
                printf("%s",grabMorseChar());
            }
            printf("\n");
            clearPolybiusTable();
        }
        else if (inputChar == '\n' && spaceFlag ==1)
        {
            while(numChars(polybius_table)>=3)
            {
                printf("%s",grabMorseChar());
            }
            printf("\n");
            clearPolybiusTable();
        }
        else if( ((int)inputChar -33) <0 || ((int)inputChar-33) >89 )  //morse codes go up to 90 elements, [89] ==z, and ! == [0], but since ! is given 33, then subtract 33 to get [0]
        {
            return -1;
        }
        else
        {
            spaceFlag=0; //turn it off if new letter
            if(numCharsConst(*(morse_table+ ((int)inputChar)-33 )) ==0) //invalid input
            return -1;
            //store + convert
            storeMorseIntoPolybius( (char*) (*(morse_table+ ((int)inputChar)-33 )) );
            storeMorseIntoPolybius(tmpMorseChars);
            while(numChars(polybius_table)>=3)
            {
                printf("%s",grabMorseChar());
            }

        }


    }
    //reaches EOF, remaining characters dropped
    return 1;
}
int fm_decrypt()
{
    clearPolybiusTable();
    char inputChar= ' ';
    while( (inputChar = getchar()) != EOF )
    {
        if(inputChar == '\n') //flushes out polybius_table buffer and go to next line
        {
            int numPolybiusCharsLeft = numChars(polybius_table);
            int morseTableIdx = getDecryptedCharacterIdx(numPolybiusCharsLeft);
            if(morseTableIdx != -1)
            {
                shiftPolybiusTableMorseEncodingDecryption(numCharsConst(*(morse_table+morseTableIdx)));// 2 for XX characters
                printf("%c",(char)(morseTableIdx+33));
            }
            printf("\n");
        }
        else
        {
            int idx = compareFm_key(inputChar);
            if(idx==-1) //fail case?
                return -1;
            storeMorseIntoPolybius( (char*) (*(fractionated_table+ idx)) );
            int xxWhitespace = checkForXXWhitespace(); //parse the x out, find the xx
            if(xxWhitespace>=0)
            {
                int morseTableIdx = getDecryptedCharacterIdx(xxWhitespace);
                if(morseTableIdx == -1)
                {
                    return -1;
                }
                shiftPolybiusTableMorseEncodingDecryption(numCharsConst(*(morse_table+morseTableIdx))+2);// 2 for XX characters
                printf("%c ",(char)(morseTableIdx+33));
            }
            else //if no whitespace xx or if just a single x, check if next string before x can form into character, else loop and maybe you will get xx in next character
            {
                int delimiterIdx = checkForLetterDelimiterX();
                if(delimiterIdx>0)
                {
                    int morseTableIdx = getDecryptedCharacterIdx(delimiterIdx);
                    if(morseTableIdx != -1)
                    {
                        shiftPolybiusTableMorseEncodingDecryption(numCharsConst(*(morse_table+morseTableIdx))+1);// 1 for X character
                        printf("%c",(char)(morseTableIdx+33));
                    }
                }
                else if(delimiterIdx==0)
                {
                    shiftPolybiusTableMorseEncodingDecryption(1); //that's a space
                    printf(" ");
                }
                //else delimiterIdx == -1 and no parse
            }
        }

    }
    //last character is in polybius_table can be found by finding the length of the polybius_table and then setting that as the parameter for getDecryptedCharacterIdx

    return 1;
}