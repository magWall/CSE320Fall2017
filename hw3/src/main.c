#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {

    sf_mem_init();

    sf_snapshot();
    double* ptr = sf_malloc(PAGE_SZ<<2);
    *ptr = 320320320e-320;
//     sf_snapshot();
// //    sf_blockprint(ptr-1);
//      double* ptr2 = sf_malloc(sizeof(double));
//      *ptr2= 1;
//      double* ptr3 = sf_malloc(sizeof(double));
//      *ptr3= 1;
//      double* ptr4 = sf_malloc(sizeof(double));
//      *ptr4= 1;
//      int* ptrA = sf_malloc(sizeof(int));
//      *ptrA= 1;
//     // sf_varprint(ptr);
//     // sf_varprint(ptr2);
//     // sf_varprint(ptr3);
//     // sf_varprint(ptr4);
//     // sf_varprint(ptrA);
//     sf_snapshot();

//     printf("%f\n", *ptr);

//     sf_free(ptr);
//     sf_free(ptr2);
//     sf_free(ptr3);
//     sf_free(ptr4);
//     sf_free(ptrA);
//     sf_snapshot();

    sf_mem_fini();

    return EXIT_SUCCESS;
}
