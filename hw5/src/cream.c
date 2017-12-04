#include "cream.h"
#include "utils.h"

/* test code */
#include "queue.h"
#include "hashmap.h"
#include "stdio.h"
#include "utils.h"
#include "string.h"
#include "csapp.h"

queue_t *global_queue;
hashmap_t *global_map;

void *thread_enqueue(void *arg) {
    enqueue(global_queue, arg);
    return NULL;
}
typedef struct map_insert_t {
    void *key_ptr;
    void *val_ptr;
} map_insert_t;


/* Used in item destruction */
void map_free_function(map_key_t key, map_val_t val) {
    free(key.key_base);
    free(val.val_base);
}
void map_init(int max_entries) {
    global_map = create_map(max_entries, jenkins_one_at_a_time_hash, map_free_function);
}

void *thread_put(void *arg) {
    map_insert_t *insert = (map_insert_t *) arg;

    put(global_map, MAP_KEY(insert->key_ptr, sizeof(int)), MAP_VAL(insert->val_ptr, sizeof(int)), false);
    return NULL;
}
void *thread_function(void *vargp)
{
    while(1) //infinite loop always true
    {
        //do something
        int connfd = *( (int*) dequeue(global_queue) );
        //do something to fd like access, then echo data
       // echo_cnt(connfd);
        // rio_t rio;
        // Rio_readinitb(&rio,connfd);
        request_header_t* req_buffer = calloc(1,sizeof(request_header_t));  // THIS IS HEADER FOR REQ CODE, KEY SIZE, VALUE SiZE
        Rio_readn(connfd, req_buffer, sizeof(request_header_t));
        int request_code = req_buffer->request_code;
        int key_size = req_buffer->key_size;
        int value_size = req_buffer->value_size;
        response_header_t* responseHdr = calloc(1,sizeof(request_header_t));

        if(request_code == PUT)
        {
            if(key_size < MIN_KEY_SIZE || key_size >MAX_KEY_SIZE ||
            value_size < MIN_VALUE_SIZE || value_size >MAX_VALUE_SIZE )
            {
                //send invalid response
                //response_codes { OK = 200, UNSUPPORTED = 220,
                //BAD_REQUEST = 400, NOT_FOUND = 404 } response_codes;
                responseHdr->response_code = BAD_REQUEST;
                responseHdr->value_size = 0;
                Rio_writen(connfd, responseHdr, sizeof(response_header_t));
                close(connfd);
                //send
            }
            else
            {
                map_key_t key;
                key.key_base = calloc(1,sizeof(int*));
                Rio_readn(connfd,key.key_base,key_size);
                key.key_len = key_size;
                map_val_t val;
                val.val_base = calloc(1,sizeof(int*));
                Rio_readn(connfd,val.val_base,value_size);
                val.val_len = value_size;
                if(put(global_map, key, val, true)==true  )
                //no indication when not to force on spec sheet
                {
                    responseHdr->response_code = OK;
                    responseHdr->value_size = value_size;
                    Rio_writen(connfd,responseHdr,sizeof(response_header_t));
                    close(connfd);
                }
                else //put statement failed
                {
                    //free
                    free(key.key_base);
                    free(val.val_base);
                    responseHdr->response_code = BAD_REQUEST;
                    responseHdr->value_size = 0;
                    Rio_writen(connfd,responseHdr,sizeof(response_header_t));
                    close(connfd);
                }
            }

        }
        else if(request_code == GET)
        {
            if(key_size < MIN_KEY_SIZE || key_size >MAX_KEY_SIZE)
             {
                responseHdr->response_code = BAD_REQUEST;
                responseHdr->value_size = 0;
                Rio_writen(connfd, &responseHdr, sizeof(response_header_t));
                close(connfd);
                //send
             }
             else
             {
                map_key_t key;
                key.key_base = calloc(1,sizeof(int*));
                Rio_readn(connfd,key.key_base,key_size);
                key.key_len = key_size;
                map_val_t result = get(global_map, key);
                if(result.val_base ==NULL && result.val_len ==0)
                {
                    //not found
                    responseHdr->response_code = NOT_FOUND;
                    responseHdr->value_size = 0;
                    Rio_writen(connfd,responseHdr,sizeof(response_header_t));
                    close(connfd);
                }
                else
                {
                    //success
                    responseHdr->response_code = OK;
                    responseHdr->value_size = result.val_len;
                    Rio_writen(connfd,responseHdr,sizeof(response_header_t));
                    Rio_writen(connfd,result.val_base, sizeof(result.val_len));
                    close(connfd);

                }
             }
        }
        else if(request_code == EVICT)
        {
             if(key_size < MIN_KEY_SIZE || key_size >MAX_KEY_SIZE)
             {
                responseHdr->response_code = BAD_REQUEST;
                responseHdr->value_size = 0;
                Rio_writen(connfd, responseHdr, sizeof(response_header_t));
                close(connfd);
                //send
             }
             else
             {
                //accept delete
                map_key_t key;
                key.key_base = calloc(1,sizeof(int*));
                Rio_readn(connfd,key.key_base,key_size);
                key.key_len = key_size;
                map_node_t result = delete(global_map,key);
                //free node
                free(key.key_base);
                free(result.val.val_base);
                responseHdr->response_code = OK;
                responseHdr->value_size = 0;
                Rio_writen(connfd, responseHdr, sizeof(response_header_t));
                close(connfd);

             }
        }
        else if(request_code == CLEAR)
        {
            if(clear_map(global_map) ==true) //clears map here
            {
                //cleared, send ok
                responseHdr->response_code = OK;
                responseHdr->value_size = 0;
                Rio_writen(connfd, responseHdr, sizeof(response_header_t));
                close(connfd);

                //send
            }
            else //clear map didn't work bcz invalid map? probably won't happen
            {
                responseHdr->response_code = BAD_REQUEST;
                responseHdr->value_size = 0;
                Rio_writen(connfd, responseHdr, sizeof(response_header_t));
                close(connfd);
                //send
            }
        }
        else //error, invalid request code
        {
            //send invalid response
            responseHdr->response_code = UNSUPPORTED;
            responseHdr->value_size = 0;
            Rio_writen(connfd, responseHdr, sizeof(response_header_t));
            close(connfd);
            //send
        }


    }
}

int main(int argc, char *argv[]) {
    for(int i=0;i<argc;i++)
    {
        if(strcmp((argv[i]) ,"-h")==0 )
        {
            fprintf(stdout,"-h                 Displays this help menu and returns EXIT_SUCCESS.\n"
                "NUM_WORKERS        The number of worker threads used to service requests.\n"
                "PORT_NUMBER        Port number to listen on for incoming connections.\n"
                "MAX_ENTRIES        The maximum number of entries that can be stored in `cream`'s underlying data store.\n");
        return EXIT_SUCCESS;
        }
    }
    if(argc !=4) //3 required args, 1 for ./cream
    {
        fprintf(stderr,"-h                 Displays this help menu and returns EXIT_SUCCESS.\n"
                "NUM_WORKERS        The number of worker threads used to service requests.\n"
                "PORT_NUMBER        Port number to listen on for incoming connections.\n"
                "MAX_ENTRIES        The maximum number of entries that can be stored in `cream`'s underlying data store.\n");
        return EXIT_FAILURE;
    }
    pthread_t tid;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    int i, listenfd, connfd; //connfd is file descriptor, fd is an int
    int workerThreads = atoi(argv[1]); //num_workers
    //int Portt_number = atoi(argv[2]);
    int max_entries = atoi(argv[3]);
    map_init(max_entries); //create hashmap + queue
    global_queue = create_queue();

    //spawn worker threads
    for (i=0;i<workerThreads;i++)
    {
        Pthread_create(&tid,NULL,thread_function,NULL); //threads will run funct when dequeue avail
    }
    listenfd = Open_listenfd(argv[2]);

    while(1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA*) &clientaddr, &clientlen);//accept fd
        enqueue(global_queue, &connfd);//accepts void pointer, pass by reference on fd
    }
    //for

    /* test code */
    // global_queue = create_queue();
    // int NUM_THREADS = 100;
    //  pthread_t thread_ids[NUM_THREADS];
    //  int num_items;
    // // spawn NUM_THREADS threads to enqueue elements
    // for(int index = 0; index < NUM_THREADS; index++) {
    //     int *ptr = malloc(sizeof(int));
    //     *ptr = index;

    //     if(pthread_create(&thread_ids[index], NULL, thread_enqueue, ptr) != 0)
    //         exit(EXIT_FAILURE);

    // if(sem_getvalue(&global_queue->items, &num_items) != 0)
    //     exit(EXIT_FAILURE);
    // }
    // dequeue(global_queue);
    // // wait for threads to die before checking queue
    // for(int index = 0; index < NUM_THREADS; index++) {
    //     pthread_join(thread_ids[index], NULL);
    // }

    // // get number of items in queue


    // printf("%d ",num_items);

    /*test code hashmap*/
    // map_init();
    //  pthread_t thread_ids[NUM_THREADS];
    //  int num_items = 0;
    // // spawn NUM_THREADS threads to put elements
    // for(int index = 0; index < NUM_THREADS; index++) {
    //     int *key_ptr = malloc(sizeof(int));
    //     int *val_ptr = malloc(sizeof(int));
    //     *key_ptr = index;
    //     *val_ptr = index * 2;

    //     map_insert_t *insert = malloc(sizeof(map_insert_t));
    //     insert->key_ptr = key_ptr;
    //     insert->val_ptr = val_ptr;
    //     if(pthread_create(&thread_ids[index], NULL, thread_put, insert) != 0)
    //         exit(EXIT_FAILURE);
    //     num_items = global_map->size;

    // }
    // printf("%d \n",num_items);
    // // wait for threads to die before checking queue
    // for(int index = 0; index < NUM_THREADS; index++) {
    //     pthread_join(thread_ids[index], NULL);
    // }



    exit(0);
}
