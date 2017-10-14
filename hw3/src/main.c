#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {

    sf_mem_init();

    sf_snapshot();
    /* void *u = */ sf_malloc(1);          //32
    void *v = sf_malloc(LIST_1_MIN); //48
 //   sf_varprint(v);
 //   sf_snapshot();
    void *w = sf_malloc(LIST_2_MIN); //160
//sf_varprint(w);
  //  sf_snapshot();
    void *x = sf_malloc(LIST_3_MIN); //544
  //  sf_varprint(x);
 //   sf_snapshot();
    void *y = sf_malloc(LIST_4_MIN); //2080
  //  sf_varprint(y);
   // sf_snapshot();
    /* void *z = */ sf_malloc(1); // 32

    int allocated_block_size[4] = {48, 160, 544, 2080};

    sf_free(v);
    sf_free(w);
    sf_free(x);
    sf_free(y);
    //sf_snapshot();
    for (int i = 0; i < FREE_LIST_COUNT; i++)
    {
        sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
        if(fh == NULL)
        {
            printf("%d is NULL\n",i);
        }
        printf("blocksize: %d v allocated array: %d\n",fh->header.block_size << 4,allocated_block_size[i]);
        printf("fh->header.allocated: %d\n",fh->header.allocated );
    }

    // There should be one free block in each list, 2 blocks in list 3 of size 544 and 1232
    for (int i = 0; i < FREE_LIST_COUNT; i++)
    {
        sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
            while(fh!=NULL)
            {
                printf("List %d: Block: %d\n",i,fh->header.block_size<<4);
                fh=fh->next;
            }
    }


    // double* ptr = sf_malloc(PAGE_SZ<<2);
    // *ptr = 320320320e-320;
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
