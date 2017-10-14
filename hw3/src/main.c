#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {

    sf_mem_init();

//     sf_snapshot();
//     /* void *u = */ sf_malloc(1);          //32
//     void *v = sf_malloc(LIST_1_MIN); //48
//  //   sf_varprint(v);
//  //   sf_snapshot();
//     void *w = sf_malloc(LIST_2_MIN); //160
// //sf_varprint(w);
//   //  sf_snapshot();
//     void *x = sf_malloc(LIST_3_MIN); //544
//   //  sf_varprint(x);
//  //   sf_snapshot();
//     void *y = sf_malloc(LIST_4_MIN); //2080
//   //  sf_varprint(y);
//    // sf_snapshot();
//     /* void *z = */ sf_malloc(1); // 32

//     int allocated_block_size[4] = {48, 160, 544, 2080};

//     sf_free(v);
//     sf_free(w);
//     sf_free(x);
//     sf_free(y);
//     //sf_snapshot();
//     for (int i = 0; i < FREE_LIST_COUNT; i++)
//     {
//         sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
//         if(fh == NULL)
//         {
//             printf("%d is NULL\n",i);
//         }
//         printf("blocksize: %d v allocated array: %d\n",fh->header.block_size << 4,allocated_block_size[i]);
//         printf("fh->header.allocated: %d\n",fh->header.allocated );
//     }

//     // There should be one free block in each list, 2 blocks in list 3 of size 544 and 1232
//     for (int i = 0; i < FREE_LIST_COUNT; i++)
//     {
//         sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
//             while(fh!=NULL)
//             {
//                 printf("List %d: Block: %d\n",i,fh->header.block_size<<4);
//                 fh=fh->next;
//             }
//     }

    // void* x = sf_malloc(4080);
    // void* y = sf_malloc(1);

    // sf_varprint(x);
    // sf_varprint(y);
    // sf_free(x);
    // void* z = sf_malloc(6286);
    // sf_header* header = (sf_header*)((char*)z-8);
    // printf("%d",header->block_size << 4);
    // sf_free(y);



    // void *x = sf_malloc(sizeof(double) * 8);
    // sf_varprint(x);
    // sf_blockprint( (char*)x-8);
    // void *y = sf_realloc(x, sizeof(int));

    // if(y == NULL)
    // {
    //     printf("Y should not be null!");
    // }

    // sf_header *header = (sf_header*)((char*)y - 8);
    // if( header->block_size << 4 != 32)
    //     printf("Realloc'ed block size not what was expected!");
    // if(header->allocated != 1)
    //     printf("Allocated bit is not set!");


    // // After realloc'ing x, we can return a block of size 48 to the freelist.
    // // This block will coalesce with the block of size 4016.
    // int tmpNum;
    // if (4064 >= LIST_1_MIN && 4064 <= LIST_1_MAX) tmpNum=0;
    // else if (4064 >= LIST_2_MIN && 4064 <= LIST_2_MAX) tmpNum=1;
    // else if (4064 >= LIST_3_MIN && 4064 <= LIST_3_MAX) tmpNum=2;
    // else
    //     tmpNum=3;
    // free_list *fl = &seg_free_list[tmpNum];

    // if(fl->head == NULL)
    //     printf("No block in expected free list!");
    // if(fl->head->header.allocated == 0)
    // printf("Allocated bit is set!");
    // if(fl->head->header.block_size << 4 != 4064)
    // printf("Free block size not what was expected!");


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

     printf("%f\n", *ptr);

    sf_free(ptr);
//     sf_free(ptr2);
//     sf_free(ptr3);
//     sf_free(ptr4);
//     sf_free(ptrA);
//     sf_snapshot();

    sf_mem_fini();

    return EXIT_SUCCESS;
}
