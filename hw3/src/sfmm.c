/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * You should store the heads of your free lists in these variables.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
free_list seg_free_list[4] = {
    {NULL, LIST_1_MIN, LIST_1_MAX},  //LIST_#_MIN & LIST_#_MAX is in bytes
    {NULL, LIST_2_MIN, LIST_2_MAX},
    {NULL, LIST_3_MIN, LIST_3_MAX},
    {NULL, LIST_4_MIN, LIST_4_MAX}
};

int sf_errno = 0;
int currPageNum = 0; // 1 to 3

void *sf_malloc(size_t size) {
    if(size==0 || size> ( (PAGE_SZ*4) - 16)) //alloc 16 for header/footer
    {
        sf_errno=EINVAL;    //nivalid num == EINVAL
        return NULL;
    }
    if(seg_free_list[0].head == NULL && seg_free_list[1].head== NULL && seg_free_list[2].head== NULL
        && seg_free_list[3].head == NULL)
    {
        if(currPageNum<4)
        {
            seg_free_list[3].head = sf_sbrk();
            // then set header and footer for this

            (seg_free_list[3].head)->header.allocated= 0;
            (seg_free_list[3].head)->header.padded =0;
            (seg_free_list[3].head)->header.two_zeroes =0;
            (seg_free_list[3].head)->header.block_size = (PAGE_SZ>>4);
            (seg_free_list[3].head)->header.unused = 0;

            //footer in sf_header because they have same values
            (seg_free_list[3].head + ((char)(PAGE_SZ - 16)) )->header.allocated = 0;
            (seg_free_list[3].head + ((char)(PAGE_SZ - 16)) )->header.padded=0;
            (seg_free_list[3].head + ((char)(PAGE_SZ - 16)) )->header.two_zeroes=0;
            (seg_free_list[3].head + ((char)(PAGE_SZ - 16)) )->header.block_size= (PAGE_SZ)>>4;
            (seg_free_list[3].head + ((char)(PAGE_SZ - 16)) )->header.unused=0; //requested size

            currPageNum++;
        }
        else
        {
            // cannot be processed == ENOMEM
            sf_errno = ENOMEM; //fail no memory
            return NULL;
        }
    }
    //if(size> (PAGE_SZ * (4-currPageNum) + get))
    if (size <= LIST_1_MAX && size>=LIST_1_MIN)
    {
        for(int i=0;i<4;i++)
        {
            if(seg_free_list[i].head != NULL)
            {
                //loop through all list, if space avail, alloc
                //check if NULL, if not null, go to beginning of list header

            }
            //if seg_free_list[3] or all == NULL/not enough space,
            //check and (loop) add & coalesce until space
        }
        //header cause that's the 'first fit' , then move prev around
        //alloc space, else look for bigger sizes
        //bigger sizes need to readjust header + footer, and if size <the list_#_min
        //move it to the appropriate list or alloc all of it over
    }
    else if (size <= LIST_2_MAX && size>=LIST_2_MIN)
    {

    }
    else if (size <= LIST_3_MAX && size>=LIST_3_MIN)
    {

    }
    else if (size >= LIST_3_MIN)
    {

    }




    return NULL;
}

// * If there is no memory available sf_realloc should set sf_errno to ENOMEM.

void *sf_realloc(void *ptr, size_t size) {
    if(ptr == NULL) //edge cases
    {
        sf_errno = EINVAL;
        return NULL;
    }
    if(size==0) //assume valid ptr
    {
        sf_free(ptr);
        return NULL;
    }

    // check if no memory, then == ENOMEM

	return NULL;
}

void sf_free(void *ptr) {
    // if invalid ptr, abort
    if (ptr == NULL)
        abort();

    //check if alloc, if not abort

    //check if alloc, if alloc, then manage space + unalloc + free
    //move into free_list
    //combine coalesce by checking if upper is same address

	return;
}
