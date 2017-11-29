#include "queue.h"
#include "errno.h"

queue_t *create_queue(void) {
    queue_t* newQueue = calloc(1,sizeof(queue_t));
    if(newQueue == NULL) //won't be able to access pointer if NULL
        return NULL;
    sem_init(&(newQueue->items),0,1); //initalize to 1
    pthread_mutex_init(&(newQueue->lock),NULL); //lock, attr
    return newQueue;
}

bool invalidate_queue(queue_t *self, item_destructor_f destroy_function) {
    return false;
}

bool enqueue(queue_t *self, void *item) {
    if(self == NULL || item == NULL)
    {
        errno = EINVAL;
        return false;
    }
    //lock
    if (self->front == NULL)
    {
        //sem
        //add to queue
        //unsem
    }
    else if(self ->front == NULL)//second item in list, add to rear, link front and rear
    {
        //sem
        //add to queue
        //unsem
    }
    else //not second item, replace rear with item, link prev. rear to new rear
    {
        //sem
        //add to queue
        //unsem
    }
    //unlock

    return false;
}

void *dequeue(queue_t *self) {
    if(self == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    return NULL;
}
