/**
 * \addtogroup map
 * @{
 */
#include "stdlib.h"
#include "string.h"
#include "map.h"
#include "math.h"

/// @private  
int hash_log2(int num_to_log)
{
    int t = 1;
    int i = 0;
    do
    {
        num_to_log = num_to_log & ~t;
        t = t << 1;
        i++;
    } while (num_to_log > 0);
    return i;
}

/// @private  
int hash_upperLimit(int bitsize)
{
    return 1 << bitsize;
}

/// @private
int char_ratio = (int)(sizeof(int) / sizeof(char));

/// Modified some stuff from : http://isthe.com/chongo/tech/comp/fnv/ @private
int hash_string(int hash_table_size, char *string, int strlen)
{
    int i, hash = 2166136261;
    for (i = 0; i < strlen; i += 1)
    {
        hash *= 16777619;
        hash ^= string[i];
    }
    if (hash < 0)
    {
        hash *= -1;
    }
    return hash % hash_table_size;
}

map *NewMap(int capacity)
{
    int log2 = hash_log2(capacity);
    int capac = hash_upperLimit(log2);
    int sz = sizeof(struct _map_bucket) * capac;
    struct _map_bucket *buckets = malloc(sz);
    memset(buckets, 0, sz);
    int i;
    for (i = 0; i < capac; i++)
    {
        buckets[i] = (struct _map_bucket){NULL, NULL, NULL};
    }
    map newm = (map){capac, buckets};
    map *map_p = malloc(sizeof(map));
    *map_p = newm;
    return map_p;
}

/// @private  
void _bucket_insert(struct _map_bucket *bucket, char *key, void *value)
{
    struct _map_bucket *check = bucket;
    while (check->key != NULL)
    {
        if (strcmp(check->key, key) == 0)
        {
            check->data = value;
            return;
        }
        if (check->next == NULL)
        {
            check->next = malloc(sizeof(struct _map_bucket));
            *(check->next) = (struct _map_bucket){NULL, NULL, NULL};
        }
        check = check->next;
    }
    check->key = key;
    check->data = value;
}

void Map_Set(map *a_map, char *key, void *value)
{
    int keyl = (int)strlen(key);
    int hash = hash_string(a_map->size, key, keyl);
    _bucket_insert(&(a_map->buckets[hash]), key, value);
}
/// @private  
void _bucket_get(struct _map_bucket *bucket, char *key, map_result *result)
{
    struct _map_bucket *check = bucket;
    while (check->key != NULL)
    {
        if (strcmp(check->key, key) == 0)
        {
            result->found = 1;
            result->data = check->data;
            return;
        }
        else if (check->next != NULL)
        {
            check = check->next;
        }
        else
        {
            result->found = 0;
            break;
        }
    }
}

map_result Map_Get(map *a_map, char *key)
{
    map_result res = (map_result){0, NULL};
    int keyl = (int)strlen(key);
    int hash = hash_string(a_map->size, key, keyl);
    _bucket_get(&(a_map->buckets[hash]), key, &res);
    return res;
}

void _bucket_delete(struct _map_bucket *bucket, char *key, short free_it, map_result *result)
{
    struct _map_bucket *last = bucket;
    struct _map_bucket *next = bucket->next;
    while (next != NULL)
    {
        if (strcmp(next->key, key) == 0)
        {
            result->found = 1;
            result->data = next->data;
            if (free_it)
            {
                free(next->data);
                result->data = NULL;
            }
            last->next = next->next;
            free(next);
        }
        else
        {
            last = next;
            next = next->next;
        }
    }
}

map_result Map_Delete(map *a_map, char *key, short free_it)
{
    map_result res = (map_result){0, NULL};
    int keyl = (int)strlen(key);
    int hash = hash_string(a_map->size, key, keyl);

    struct _map_bucket top = a_map->buckets[hash];
    if (top.key == NULL)
    {
        return res;
    }
    if (strcmp(top.key, key) == 0)
    {
        res.found = 1;
        res.data = top.data;
        if (free_it)
        {
            free(top.data);
            res.data = NULL;
        }
        if (top.next != NULL)
        {
            a_map->buckets[hash] = *(top.next);
            free(top.next);
        }
        else
        {
            a_map->buckets[hash] = (struct _map_bucket){NULL, NULL, NULL};
        }
        return res;
    }
    if (top.next == NULL)
    {
        return res;
    }
    _bucket_delete(&(a_map->buckets[hash]), key, free_it, &res);

    return res;
}
/**
 * @}
*/