#include "queue.h"
#include "errno.h"
#include "wrappers.h"


void unix_error(char *msg)
{
    fprintf(stderr, "%s: %d\n", msg, errno);
    exit(0);
}
void Sem_init(sem_t* t, int num, int num2) //probably used once, not needed[?]
{
    if( sem_init(t, num,num2) <0) //initalize to 0
        unix_error("semaphore init error");
}
void P(sem_t* sem)
{
    if( sem_wait(sem) < 0)
      unix_error("P error");
}
void V(sem_t* sem)
{
    if( sem_post(sem) <0)
        unix_error("V error");
}
queue_t *create_queue(void) {
    queue_t* newQueue = calloc(1,sizeof(queue_t));
    if(newQueue == NULL) //won't be able to access pointer if NULL
        return NULL;
    if( sem_init(&(newQueue->items),0,0) <0) //initalize to 0 for 0 items
        unix_error("semaphore init error");
    if( pthread_mutex_init(&(newQueue->lock),NULL) <0)
        unix_error("pthread_mutex_init error"); //lock, attr

    return newQueue;
}

bool invalidate_queue(queue_t *self, item_destructor_f destroy_function) {
    //before invalidating, lock and then check
    if(self == NULL)
    {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->lock);
    if(self->invalid == true)
    {
        errno =EINVAL;
        return false;
    }

    queue_node_t* nodeToLoop = self->front;
    while(nodeToLoop != self->rear)
    {
        destroy_function(nodeToLoop->item);
        queue_node_t* tmp = nodeToLoop;
        nodeToLoop = nodeToLoop->next;
        free(tmp);
    }
    destroy_function(nodeToLoop->item);
    free(nodeToLoop); //last item where front == rear
    pthread_mutex_unlock(&self->lock);

    return false;
}

bool enqueue(queue_t *self, void *item) {

    if(self == NULL || item == NULL)
    {
        errno = EINVAL;
        return false;
    }
    //lock
    pthread_mutex_lock(&self->lock);
    if(self->invalid == true)
    {
        errno = EINVAL;
        return false;
    }

    if (self->front == NULL) //aka front and rear are empty
    {
        //sem
        queue_node_t* node = malloc(sizeof(queue_node_t));
        node->item = item;
        node->next = NULL;
        self->front = node;
        self->rear = node;
        //add to queue
        V(&self->items);
        pthread_mutex_unlock(&self->lock);
        //unsem
    }
    else //not first item, replace rear with item, link prev. rear to new rear
    {
        //sem
        queue_node_t* node = malloc(sizeof(queue_node_t));
        node->item = item;
        node->next = NULL;
        self->rear->next = node;
        self->rear = node;
        //add to queue
        V(&self->items);
        pthread_mutex_unlock(&self->lock);
        //unsem
    }
    //unlock

    return true;
}

void *dequeue(queue_t *self) {
    void* item;
    queue_node_t* node;
    if(self == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    pthread_mutex_lock(&self->lock);
    if(self->invalid == true)
    {
        errno = EINVAL;
        return false;
    }
    P(&self->items); //this checks count
    //if(self->front != NULL)
    //{
    //last element where front and rear == same pointer
        node = self->front;
        if(self->front != self->rear)   //if not same, continue
            self->front= self->front->next;
        else
        {
            self->front = NULL;
            self->rear = NULL;
        }
    //}
    item = node->item;
    free(node);
    pthread_mutex_unlock(&self->lock);
    return item;
}
