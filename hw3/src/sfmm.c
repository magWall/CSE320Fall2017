/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "hw3functs.h"

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

void add_page() //assuming you can add a page
{
    currPageNum++;
    sf_free_header* tmpHeaderList = seg_free_list[FREE_LIST_COUNT-1].head;
    if(tmpHeaderList== NULL)
    {
        seg_free_list[FREE_LIST_COUNT-1].head= sf_sbrk();
        (seg_free_list[FREE_LIST_COUNT-1].head)->header.allocated= 0;
        (seg_free_list[FREE_LIST_COUNT-1].head)->header.padded =0;
        (seg_free_list[FREE_LIST_COUNT-1].head)->header.two_zeroes =0;
        (seg_free_list[FREE_LIST_COUNT-1].head)->header.block_size = (PAGE_SZ>>4);
        (seg_free_list[FREE_LIST_COUNT-1].head)->header.unused = 0;

        //footer in sf_header because they have same values
        ((sf_free_header*)((char*)seg_free_list[FREE_LIST_COUNT-1].head + PAGE_SZ - 8))->header.allocated = 0;
        ((sf_free_header*)((char*)seg_free_list[FREE_LIST_COUNT-1].head + PAGE_SZ - 8))->header.padded=0;
        ((sf_free_header*)((char*)seg_free_list[FREE_LIST_COUNT-1].head + PAGE_SZ - 8))->header.two_zeroes=0;
        ((sf_free_header*)((char*)seg_free_list[FREE_LIST_COUNT-1].head + PAGE_SZ - 8))->header.block_size= (PAGE_SZ)>>4;
        ((sf_free_header*)((char*)seg_free_list[FREE_LIST_COUNT-1].head + PAGE_SZ - 8))->header.unused=0; //requested size
    }
    else
    {
        sf_free_header* tmpSpace = sf_sbrk();
        //loop through list because freed block may be in another part of linked list to coalesce
        while(tmpHeaderList != NULL)
        {
            if( ((sf_free_header*)((char*)
                tmpHeaderList+(tmpHeaderList->header.block_size>>4) )) == tmpSpace)
            {
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) -8))->header.block_size =0; //remove old footer
                tmpHeaderList->header.block_size = tmpHeaderList->header.block_size+(PAGE_SZ>>4); //set new header block size
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) - 8))->header.block_size = tmpHeaderList->header.block_size; //adjusted block size
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) - 8))->header.allocated = 0;
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) - 8))->header.padded = 0;
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) - 8))->header.two_zeroes = 0;
                ((sf_free_header*)((char*) tmpHeaderList+(tmpHeaderList->header.block_size<<4) - 8))->header.unused = 0; //prevent corruption
                return;
            }

            tmpHeaderList = tmpHeaderList->next;
        } //failed to coalesce because no matching linked block
        tmpSpace->header.block_size = PAGE_SZ>>4;
        tmpSpace->header.allocated=0;
        tmpSpace->header.padded = 0;
        tmpSpace->header.two_zeroes =0;
        tmpSpace->header.unused=0;
        ((sf_free_header*)((char*)tmpSpace+PAGE_SZ-8))->header.allocated=0;
        ((sf_free_header*)((char*)tmpSpace+PAGE_SZ-8))->header.block_size=PAGE_SZ>>4;
        ((sf_free_header*)((char*)tmpSpace+PAGE_SZ-8))->header.padded=0;
        ((sf_free_header*)((char*)tmpSpace+PAGE_SZ-8))->header.two_zeroes=0;
        ((sf_free_header*)((char*)tmpSpace+PAGE_SZ-8))->header.unused=0;
        tmpHeaderList = seg_free_list[FREE_LIST_COUNT-1].head;
        tmpHeaderList->prev = tmpSpace;
        tmpSpace->prev = NULL;
        tmpSpace->next = tmpHeaderList;
        seg_free_list[FREE_LIST_COUNT-1].head = tmpSpace;
    }
}
void *sf_malloc(size_t size) {
    if(size==0 || size> ( (PAGE_SZ*4) - 16)) //alloc 16 for header/footer
    {
        sf_errno=EINVAL;    //nivalid num == EINVAL
        return NULL;
    }
    if(get_heap_start() == get_heap_end())
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
            ((sf_free_header*)((char*)seg_free_list[3].head + PAGE_SZ - 8))->header.allocated = 0;
            ((sf_free_header*)((char*)seg_free_list[3].head + PAGE_SZ - 8))->header.padded=0;
            ((sf_free_header*)((char*)seg_free_list[3].head + PAGE_SZ - 8))->header.two_zeroes=0;
            ((sf_free_header*)((char*)seg_free_list[3].head + PAGE_SZ - 8))->header.block_size= (PAGE_SZ)>>4;
            ((sf_free_header*)((char*)seg_free_list[3].head + PAGE_SZ - 8))->header.unused=0; //requested size

            currPageNum++;
            //block_size is measured in hex, the remaining are in bits
            //sf_blockprint(seg_free_list[3].head);

        }
        else
        {
            // cannot be processed == ENOMEM
            sf_errno = ENOMEM; //fail no memory
            return NULL;
        }
    }
    //if(size> (PAGE_SZ * (4-currPageNum) + get))

    int paddedSize = size%16;

        while(currPageNum<4)
        {
            int i = findListIdxofNum(size+(16-paddedSize)+16);
            for( ;i<FREE_LIST_COUNT;i++) //am I evolving?! I finally used ;!
            {
                if(seg_free_list[i].head != NULL)
                {
                    sf_free_header* tmpHeader = seg_free_list[i].head;
                    while(tmpHeader !=NULL)
                    {
                        if( (tmpHeader->header.block_size<<4) >(size+16+(16-paddedSize)) )//add 16 bytes for header and footer
                        {
                            tmpHeader->header.allocated = 1;
                            if( (tmpHeader->header.block_size <<4)-((16-paddedSize)+size+16)>LIST_1_MIN)//if new block can be formed
                            {
                                //unsplit block [ ][ this block ] footer
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.allocated=0;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.unused=0;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.two_zeroes=0;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.padded=0;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.block_size=((tmpHeader->header.block_size<<4)-(size+16+(16-paddedSize)))>>4;
                                //split block

                                if(paddedSize!=0)
                                {
                                    tmpHeader->header.padded=1;
                                    ((sf_free_header*)((char*)tmpHeader+(16-paddedSize)+size+8 ))->header.padded=1;
                                }
                                else
                                {
                                    tmpHeader->header.padded=0;
                                    ((sf_free_header*)((char*)tmpHeader+(16-paddedSize)+size+8 ))->header.padded=0;
                                }
                                //original  [] [this block] header
                                ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16 ))->header.allocated=0;
                                ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16 ))->header.unused=0;
                                ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16 ))->header.two_zeroes=0;
                                ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16 ))->header.padded=0;
                                ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16 ))->header.block_size=((tmpHeader->header.block_size<<4)-(size+16+(16-paddedSize)))>>4;

                                tmpHeader->header.block_size= (size+16+(16-paddedSize))>>4;
                                tmpHeader->header.two_zeroes=0;
                                // split [this block] [] footer
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.allocated=1;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.unused=size;//requested size
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.two_zeroes=0;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.block_size=(size+16+(16-paddedSize))>>4;

                                //new point to split block, update
                                sf_free_header* freeBlock = ((sf_free_header*)((char*)tmpHeader+ (16-paddedSize)+size+16));
                                if(tmpHeader->prev != NULL) //remove from old list
                                {
                                    tmpHeader->prev->next = tmpHeader->next;
                                    if(tmpHeader->next!=NULL)
                                    {
                                        tmpHeader->next->prev = tmpHeader->prev;
                                        tmpHeader->next = NULL;  //remove connections
                                    }
                                    tmpHeader->prev = NULL;//remove connections

                                }
                                else //is null, so nothing behind, first on list
                                {
                                    if(tmpHeader->next!=NULL)
                                    {
                                        tmpHeader->next->prev = tmpHeader->prev;//or NULL
                                        //set as new head
                                        seg_free_list[i].head = tmpHeader->next;
                                        tmpHeader->next = NULL;
                                    }
                                    else //both null, only one in list
                                        seg_free_list[i].head = NULL;

                                }
                                int idxNum = findListIdxofNum(freeBlock->header.block_size<<4);
                                if(seg_free_list[idxNum].head!= NULL)
                                {
                                    seg_free_list[idxNum].head->prev = freeBlock;
                                    freeBlock->next = seg_free_list[idxNum].head;
                                    freeBlock->prev = NULL;
                                    seg_free_list[idxNum].head = freeBlock;
                                }
                                else //head is null, just add block in
                                {
                                    seg_free_list[idxNum].head = freeBlock;
                                    freeBlock->next = NULL;
                                    freeBlock->prev = NULL;
                                }

                               // sf_blockprint(tmpHeader);
                                return ((char*)tmpHeader+8);

                            }
                            else //don't split block
                            {
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.allocated=1;
                                ((sf_free_header*)((char*)tmpHeader+ (tmpHeader->header.block_size<<4)-8 ))->header.unused=size;//requested size

                                if(paddedSize!=0)
                                {
                                    tmpHeader->header.padded=1;
                                    ((sf_free_header*)((char*)tmpHeader+(tmpHeader->header.block_size<<4)-8 ))->header.padded=1;
                                }
                                else
                                {
                                    tmpHeader->header.padded=0;
                                    ((sf_free_header*)((char*)tmpHeader+(tmpHeader->header.block_size<<4)-8 ))->header.padded=0;
                                }
                            }
                            //update
                            if(tmpHeader->prev != NULL) //remove from old list
                            {
                                tmpHeader->prev->next = tmpHeader->next;
                                if(tmpHeader->next!=NULL)
                                {
                                    tmpHeader->next->prev = tmpHeader->prev;
                                    tmpHeader->next = NULL;  //remove connections
                                }
                                tmpHeader->prev = NULL;//remove connections

                            }
                            else //is null, so nothing behind, first on list
                            {
                                if(tmpHeader->next!=NULL)
                                {
                                    tmpHeader->next->prev = tmpHeader->prev;//or NULL
                                    //set as new head
                                    seg_free_list[i].head = tmpHeader->next;
                                    tmpHeader->next = NULL;
                                }
                                else
                                    seg_free_list[i].head = NULL;

                            }
                            //return payload
                           // sf_blockprint(tmpHeader);
                            return ((char*)tmpHeader+8) ;
                        }
                        tmpHeader = tmpHeader->next;
                    }//if out of loop, check currPages <4, may need to trap code into another loop
                    //break out if add more than 4 pages cause no mem

                    //loop through all list, if space avail, alloc
                    //check if NULL, if not null, go to beginning of list header

                }
            //if seg_free_list[3] or all == NULL/not enough space,
            //check and (loop) add & coalesce until space
            }
            if(currPageNum!=4)
                add_page();
            else
            {
                sf_errno = ENOMEM;
                return NULL;
            }
        }


        //header cause that's the 'first fit' , then move prev around
        //alloc space, else look for bigger sizes
        //bigger sizes need to readjust header + footer, and if size <the list_#_min
        //move it to the appropriate list or alloc all of it over

        //if list not null and size not enough but you can get more pages
        //get another page and if ptr+sizeofblock == sf_breaks ptr address, coalesce
        //else just add into list

    return NULL;
}
int findListIdxofNum(int num)
{
    if(num>=LIST_1_MIN && num<=LIST_1_MAX)
        return 0;
    else if(num>=LIST_2_MIN && num<=LIST_2_MAX)
        return 1;
    else if(num>=LIST_3_MIN && num<=LIST_3_MAX)
        return 2;
    else if(num>=LIST_4_MIN)
        return 3;
    return -1;
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
    //ptr is payload

    //check if alloc, if not abort

    //check if alloc, if alloc, then manage space + unalloc + free
    //move into free_list
    //combine coalesce by checking if upper is same address

    //when coalescing, check if memory address of next block is allocated or freed
    //if freed, then combine, check new size of block, allocate into right
    //free list, remove old freed block from the previous free list

	return;
}
