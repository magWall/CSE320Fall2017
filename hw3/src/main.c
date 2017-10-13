#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {

    sf_mem_init();

    sf_snapshot();
    double* ptr = sf_malloc(sizeof(double));
    *ptr = 320320320e-320;
    //sf_snapshot();
//    sf_blockprint(ptr-1);
    sf_varprint(ptr);
    printf("%f\n", *ptr);

    sf_free(ptr);

    sf_mem_fini();

    return EXIT_SUCCESS;
}
