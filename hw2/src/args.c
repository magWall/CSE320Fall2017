#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int opterr;
int optopt;
int optind;
char *optarg;

state_t *program_state;

void
parse_args(int argc, char *argv[])
{
  int i;
  char option;
  char *joined_argv;

  joined_argv = join_string_array(argc, argv);
  info("argc: %d argv: %s", argc, joined_argv);
  free(joined_argv);

  if(argc==1) //fail if no args passed,
  {
    USAGE(argv[0]);
    exit(EXIT_FAILURE);
  }
  int tmpIdx=0;
  for(tmpIdx=1;tmpIdx<argc;tmpIdx++)//check for -h as it takes precedence
  {
    if(strcmp(*(argv+tmpIdx),"-h")==0)
    {
      USAGE(argv[0]);
      exit(EXIT_SUCCESS);
    }
  }
  bool tmpEflag= false;
  tmpIdx=1;
    //-e mandatory if -h not available, and positional args infile outfile required
  if(argc<5) //if -e is not found, then -h is a must, else  infile,outfile,-e
  {                                   //bin/utf in.txt out.txt -e ENCODING == 5 args
    while(tmpIdx<argc)
    {
      if(strcmp(*(argv+tmpIdx),"-e")==0)
        tmpEflag=true;
      tmpIdx++;
    }
    if(tmpEflag == true) //can't run -e without other 4 args
    {
      USAGE(argv[0]);
      exit(EXIT_FAILURE);
    }

  }
  if(argc>5) //-h not found at this point, and you have extra args, fail
  {
    USAGE(argv[0]);
    exit(EXIT_FAILURE);
  }

  program_state = Calloc(1, sizeof(state_t));
  for (i = 0; optind < argc; ++i)
  {
    debug("%d opterr: %d", i, opterr);
    debug("%d optind: %d", i, optind);
    debug("%d optopt: %d", i, optopt);
    debug("%d argv[optind]: %s", i, argv[optind]);
    if ((option = getopt(argc, argv, "+e:")) != -1) //removed i
    {
      switch (option) {
        case 'e': {
          info("Encoding Argument: %s", optarg);
          if ((program_state->encoding_to = determine_format(optarg)) == 0)
            {
              free(program_state);
              program_state = NULL;
              USAGE(argv[0]);
              print_state();
              //may need to close and valgrind here
              exit(EXIT_FAILURE);
            }

        }
        case '?': {
          if (optopt != 'h')
           {
              fprintf(stderr, KRED "-%c is not a supported argument\n" KNRM,
                    optopt);
              USAGE(argv[0]);
              free(program_state);
              // may need to close and valgrind here with input docs
              exit(EXIT_FAILURE);
           }
            //else it's h
            USAGE(argv[0]);
            free(program_state);
            exit(EXIT_SUCCESS);
        }
        default: {
          break;
        }
      }
    }
    else if(argv[optind] != NULL)
    {
      if (program_state->in_file == NULL) {
        program_state->in_file = argv[optind];
      }
      else if(program_state->out_file == NULL)
      {
        program_state->out_file = argv[optind];
      }
      optind++;
    }
  }
 // free(joined_argv);
}

format_t
determine_format(char *argument)
{
  if (strcmp(argument, STR_UTF16LE) == 0)
    return UTF16LE;
  if (strcmp(argument, STR_UTF16BE) == 0)
    return UTF16BE;
  if (strcmp(argument, STR_UTF8) == 0)
    return UTF8;
  return 0;
}

char*
bom_to_string(format_t bom){
  switch(bom){
    case UTF8: return STR_UTF8;
    case UTF16BE: return STR_UTF16BE;
    case UTF16LE: return STR_UTF16LE;
  }
  return "UNKNOWN";
}

char*
join_string_array(int count, char *array[]) //hello world   2,a    a = hello, world, NULL
{
  char *ret;
  int i;
  int totalLen=0;
  int currentLen = 0;
  int lenOfArrayToAdd = 0;
  for (i=0;i<count;i++)
  {
    totalLen+=strlen(array[i]);
    totalLen++; //add space
  }
  totalLen++; //add null terminator
  ret = malloc(sizeof(char)*totalLen);
  for (i=0;i<count;i++)
  {
    lenOfArrayToAdd = strlen(array[i]);
    memcpy(ret+currentLen,array[i],lenOfArrayToAdd); //add next string
    currentLen+=lenOfArrayToAdd;
//    if(i+1!=count)
//    {
      memcpy(ret+currentLen," ",1);
      currentLen+=1; //increment for space
//    }
    /*else*/ if(i+1==count) //put null terminator
      *(ret+currentLen)= '\0';
  }

  return ret;
}

int
array_size(int count, char *array[])
{
  int i, sum = 1; /* NULL terminator */
  for (i = 0; i < count; ++i) {
    sum += strlen(array[i]);
    ++sum; /* For the spaces */
  }
  return sum+1;
}

void
print_state()
{
  if (program_state == NULL) {
    error("program_state is %p", (void*)program_state);
    exit(EXIT_FAILURE);
  }
  info("program_state {\n"
         "  format_t encoding_to = 0x%X;\n"
         "  format_t encoding_from = 0x%X;\n"
         "  char *in_file = '%s';\n"
         "  char *out_file = '%s';\n"
         "};\n",
         program_state->encoding_to, program_state->encoding_from,
         program_state->in_file, program_state->out_file);
}
