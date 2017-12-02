#include "utils.h"
#include "wrappers.h"
#include "errno.h"

#define MAP_KEY(base, len) (map_key_t) {.key_base = base, .key_len = len}
#define MAP_VAL(base, len) (map_val_t) {.val_base = base, .val_len = len}
#define MAP_NODE(key_arg, val_arg, tombstone_arg) (map_node_t) {.key = key_arg, .val = val_arg, .tombstone = tombstone_arg}

hashmap_t *create_map(uint32_t capacity, hash_func_f hash_function, destructor_f destroy_function) {
   // hashmap_t* map = calloc(capacity, sizeof(map_node_t*));
    if(capacity ==0 || hash_function==NULL || destroy_function == NULL)
    {
        errno = EINVAL;
        return false;
    }
    hashmap_t* map = (hashmap_t*) calloc(1, sizeof(hashmap_t));
    if(map == NULL)
    {
        return NULL;
    }
    if( pthread_mutex_init(&(map->write_lock), NULL)<0)
    {
        unix_error("Invalid pthread_mutex init write lock");
        return NULL;
    }
    if( pthread_mutex_init(&map->fields_lock, NULL)<0)
    {
        unix_error("Invalid pthread_mutex init fields lock");
        return NULL;
    }

    //may need to lock this for updating fields?
    map->nodes = calloc(capacity, sizeof(map_node_t));
    if(map->nodes == NULL)
    {
        return NULL;
    }
    map->capacity = capacity;
    //map->size incremented when items inserted
    map->hash_function = hash_function;
    map->destroy_function = destroy_function;

    return map;
}

//to do
bool put(hashmap_t *self, map_key_t key, map_val_t val, bool force) {
    if(self== NULL || key.key_base == NULL || key.key_len ==0 || val.val_base == NULL
        || val.val_len == 0)
    {
        errno = EINVAL;
        return false;
    }
    //lock, read data
    pthread_mutex_lock(&self->fields_lock);     //read
    self->num_readers++;
    if(self->num_readers==1)                    //block writing
        pthread_mutex_lock(&self->write_lock);
    pthread_mutex_unlock(&self->fields_lock);   //read unlock as reader decrement

    if(self->invalid ==true)
    {
        errno = EINVAL;
        pthread_mutex_lock(&self->fields_lock); //read lock
        self->num_readers--;                    //decrement
        pthread_mutex_unlock(&self->fields_lock);   //read unlock
        if(self->num_readers==0)                    //if no readers, unlock writer
            pthread_mutex_unlock(&self->write_lock);
        return false;
    }
    //check to see if hashmap.invalid is true, if it is, einval and return null

    if(self->size == self->capacity && force == false)
    {
        pthread_mutex_lock(&self->fields_lock);
        self->num_readers--;
        pthread_mutex_unlock(&self->fields_lock);
        if(self->num_readers==0)
            pthread_mutex_unlock(&self->write_lock);
        errno = ENOMEM;
        return false;
    }
    if(self->size < self->capacity) //if size is less
    {// loop to check existing key, if not, add key

    }
    // if map full & force false, errno  enomem and return false
    //force flag ignore if map not full


    //if key exists, lock write and update value, unlock write, unlock read, return true
    //if map full and force true, overwrite given index, return true
    //if node evicted, destroy_funct on evicted node

    pthread_mutex_unlock(&self->fields_lock);
    //unlock
    return false;
}

map_val_t get(hashmap_t *self, map_key_t key) {
    if(self== NULL || key.key_base == NULL || key.key_len ==0)
    {
        errno = EINVAL;
        return MAP_VAL(NULL,0);
    }
    //lock for reading
    pthread_mutex_lock(&self->fields_lock);
    self->num_readers++;
    int idx = get_index(self, key);
    if( (self->nodes+idx) ->key.key_base ==key.key_base &&
        (self->nodes+idx) ->key.key_len ==key.key_len )
    {
        //may need to linear probe?
    }

    self->num_readers--;

    pthread_mutex_unlock(&self->fields_lock);
    //unlock

    return MAP_VAL(NULL, 0);
}

map_node_t delete(hashmap_t *self, map_key_t key) {
    return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
}

bool clear_map(hashmap_t *self) {
    if(self == NULL)
    {
        errno = EINVAL;
        return false;
    }

	return false;
}

bool invalidate_map(hashmap_t *self) {
    return false;
}
