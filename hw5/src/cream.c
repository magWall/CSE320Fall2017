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
       // void* connfd = dequeue(global_queue);
        //do something to fd like access, then echo data
       // echo_cnt(connfd);

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
    //int port_number = atoi(argv[2]);
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
