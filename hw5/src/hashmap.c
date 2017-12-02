#include "utils.h"
#include "wrappers.h"
#include "errno.h"
#include "string.h"
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
    //lock, writing data
    pthread_mutex_lock(&self->write_lock);

    if(self->invalid ==true)
    {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }
    //check to see if hashmap.invalid is true, if it is, einval and return null

    if(self->size == self->capacity && force == false)
    {
        errno = ENOMEM;
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }

    bool flagKeyAdded = false;

    if(self->size == self->capacity && force == true)
    {
        // loop to check existing key,
        for(int i=0;i<self->capacity;i++)
        {
            if((self->nodes +i)->key.key_len == key.key_len &&
                memcmp(key.key_base,(self->nodes +i)->key.key_base,key.key_len))
            {
                    //if the same key, overwrite
                    self->destroy_function((self->nodes +i)->key,(self->nodes +i)->val);
                    (self->nodes+i)->val = val;
                    (self->nodes+i)->key = key;
                    self->size++;
                    flagKeyAdded=true;
                    break;
            }

        }
        //if not same key, then overwrite index
        if(flagKeyAdded==false)
        {
            int idx= get_index(self, key);
            self->destroy_function((self->nodes +idx)->key,(self->nodes +idx)->val);
            (self->nodes +idx)->key = key;
            (self->nodes +idx)->val = val;
            flagKeyAdded=true;
        }

    }


    if(self->size < self->capacity) //if size is less
    {// loop to check existing key,
        for(int i=0;i<self->capacity;i++)
        {
            if( (self->nodes +i)->key.key_base!=NULL) //if not null compare values
            {
                if( (self->nodes +i)->key.key_len == key.key_len &&
                    memcmp(key.key_base,(self->nodes +i)->key.key_base,key.key_len)==0)
                {
                    //if the same key, overwrite
                    self->destroy_function((self->nodes +i)->key,(self->nodes +i)->val);
                    (self->nodes+i)->val = val;
                    (self->nodes+i)->key = key;
                    self->size++;
                    flagKeyAdded=true;
                }
            }
            else if((self->nodes +i)->tombstone!=true)//if null, check if tombstone
            {
                //if not tombstone, then break loop as rest are empty
                //if is tombstone, do nothing and continue loop
                break;
            }
        }
        //if not, add key to first tombstone or empty node
        if(flagKeyAdded==false)
        {

            for(int i=0;i<self->capacity;i++)
            {
                if( (self->nodes + i)->tombstone == true)
                {
                    (self->nodes+i)->val = val;
                    (self->nodes+i)->key = key;
                    self->size++;
                    (self->nodes+i)->tombstone = false;
                    flagKeyAdded=true;
                    break;
                }
                else if((self->nodes +i)->key.key_base == NULL)
                {
                    (self->nodes+i)->val = val;
                    (self->nodes+i)->key = key;
                    (self->nodes+i)->tombstone = false;
                    self->size++;
                    flagKeyAdded=true;
                    break;
                }
            }
        }
    }
    // if map full & force false, errno  enomem and return false
    //force flag ignore if map not full


    //if key exists, lock write and update value, unlock write, unlock read, return true
    //if map full and force true, overwrite given index, return true
    //if node evicted, destroy_funct on evicted node

    pthread_mutex_unlock(&self->write_lock);
    //unlock
    return true;
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
    if(self->num_readers==1)
        pthread_mutex_lock(&self->write_lock);
    pthread_mutex_unlock(&self->fields_lock);

    if(self->invalid ==true)
    {
        errno = EINVAL;
        pthread_mutex_lock(&self->fields_lock);
        self->num_readers--;
        if(self->num_readers==0)
            pthread_mutex_unlock(&self->write_lock);
        pthread_mutex_unlock(&self->fields_lock);
        return MAP_VAL(NULL,0);
    }
    /*critical sections */
//    bool flagKeyFound = false;
    for(int i=0;i<self->capacity;i++)
    {
        if( (self->nodes+i)->key.key_base != NULL)
        {
            if( (self->nodes+i) ->key.key_len ==key.key_len &&
                memcmp((self->nodes+i) ->key.key_base, key.key_base, key.key_len)==0 )
            {
                pthread_mutex_lock(&self->fields_lock);
                self->num_readers--;
                if(self->num_readers==0)
                    pthread_mutex_unlock(&self->write_lock);
                pthread_mutex_unlock(&self->fields_lock);
                return MAP_VAL((self->nodes+i) ->val.val_base,(self->nodes+i) ->val.val_len);
            }
        }
        else if( (self->nodes+i)->tombstone !=true)
        {
            break;//no more elements in list, if tombstone true, continue loop
        }
    }

    /*critical sections  done*/
    pthread_mutex_lock(&self->fields_lock);
    self->num_readers--;
    if(self->num_readers==0)
        pthread_mutex_unlock(&self->write_lock);
    pthread_mutex_unlock(&self->fields_lock);
    //unlock
    //cannot find instance if up to this point

    return MAP_VAL(NULL, 0);
}

map_node_t delete(hashmap_t *self, map_key_t key) {
    //writing lock
    if(self== NULL || key.key_base == NULL || key.key_len ==0)
    {
        errno = EINVAL;
        return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid ==true)
    {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
    }
    for(int i=0;i<self->capacity;i++)
    {
        if( (self->nodes+i)->key.key_base != NULL)
        {
            if( (self->nodes+i) ->key.key_len ==key.key_len &&
                memcmp((self->nodes+i) ->key.key_base, key.key_base, key.key_len)==0 )
            {
                map_val_t tmpVal = (self->nodes+i)->val;
                map_key_t tmpKey = (self->nodes+i)->key;
                (self->nodes+i)->key= MAP_KEY(NULL,0);
                (self->nodes+i)->val = MAP_VAL(NULL,0);
                (self->nodes+i)->tombstone =true;
                self->size--;
                pthread_mutex_unlock(&self->write_lock);
                return MAP_NODE(tmpKey,tmpVal,false);
            }
        }
        else if( (self->nodes+i)->tombstone !=true)
        {
            break;//no more elements in list, if tombstone true, continue loop
        }
    }


    pthread_mutex_unlock(&self->write_lock);
    return MAP_NODE(MAP_KEY(NULL, 0), MAP_VAL(NULL, 0), false);
}

bool clear_map(hashmap_t *self) {
    if(self == NULL)
    {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid == true)
    {
        errno = EINVAL;
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }
    //set size to 0
    for(int i=0;i<self->capacity;i++)
    {
        if( (self->nodes+i)->key.key_base != NULL && (self->nodes+i)->tombstone==false)
        {
            //only on items NOT NULL, because NULL + false tombstone = unused entries in the beginning
            if( (self->nodes+i) ->key.key_base!=NULL )
            {
                self->destroy_function( (self->nodes+i)->key, (self->nodes+i)->val);
            }
        }
        // if tombstone == true || NULL
        //do nothing
    }
    self->size=0;
    pthread_mutex_unlock(&self->write_lock);

	return true;
}

bool invalidate_map(hashmap_t *self) {
    if(self == NULL)
    {
        errno = EINVAL;
        return false;
    }
    pthread_mutex_lock(&self->write_lock);
    if(self->invalid == true)
    {
        errno = EINVAL; //prevent double free
        pthread_mutex_unlock(&self->write_lock);
        return false;
    }

    for(int i=0;i<self->capacity;i++)
    {
        if( (self->nodes+i)->key.key_base != NULL && (self->nodes+i)->tombstone==false)
        {
            //only on items NOT NULL, because NULL + false tombstone = unused entries in the beginning
            if( (self->nodes+i) ->key.key_base!=NULL )
            {
                self->destroy_function( (self->nodes+i)->key, (self->nodes+i)->val);
            }
        }
        // if tombstone == true || NULL
        //do nothing
    }
    self->size=0;

    self->invalid=true;
    free(self->nodes);
    pthread_mutex_unlock(&self->write_lock);
    return true;
}
