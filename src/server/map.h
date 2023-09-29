#ifndef map_h
#define map_h

/**
 * \defgroup map
 * @brief Functions that implement a hash map data structure.
 * @details Karl's take on a simple hash map structure, which maps strings to void pointers. You can use casting to convert the void pointers into most of whatever else is needed.
 *
 * Example usage, casting an int into the data part of the map.
 *
 * ```c
 * int myfunc() {
 *      map *mymap = NewMap(100);
 *      Map_Set(mymap, "age", (void*)55);
 *      map_result result = Map_Get(mymap, "age");
 *      int age;
 *      if(result.found) {
 *          age = (int) map_result.data;
 *      }
 * }
 * ```
 *
 * Note, with this simple implementation, the map cannot change its capacity. A change to its capacity would change the hashing.
 *
 * Ultimately there are really only three things you need to do with the map.
 *
 * Initialize it, with some capacity larger than you will use. EG map * mymap = NewMap(100). The bigger it is, the fewer collisions (which are pretty rare anyway).
 *
 * Set some values in it. Eg Map_Set(mymap, "key", &value);
 *
 * You can cast numbers to void pointers to put them in the map, or you can use the pointers as references to, for example, strings malloced somewhere.
 *
 * Get some values from it. Eg void* myval = Map_Get(mymap, "key");
 *
 * Delete some values from it. For example Map_Delete(mymap, "key", 0);
 *
 * Note that the last parameter, 'free it', tells the map whether it should call 'free' on the underyling data in memory. If this is 1, and the underyling data is not a reference to a malloced part of the heap, errors will result.
 * @{
 */

// -----------------------------
//        Hashing Math
// -----------------------------

/**
 * @brief Get's a log2 ceiling. Eg, hash_log2(5) == 3.
 * @param number_to_log The number to calculate the log of.
 * @returns The log ceiling; eg, the lowest exponent to raise 2 with which would yield a number greater or equal to number_to_log.
 * @private
 */
int hash_log2(int number_to_log);

/**
 * Uses some clever, prime-number-multiplication, ORing, and bitwise operations to generate a number than, when modulused with the hash_table_size, will produce numbers ('buckets') of even distribution, to minimize the number of collisions. This function contains the meat of the hashing algorithm; it converts a key-string to an array index.
 * @see http://isthe.com/chongo/tech/comp/fnv/
 * @param hash_table_capacity The number of buckets this table holds.
 * @param string The key to hash.
 * @param strlen The length of the key.
 * @returns The index of the bucket that should be used.
 * @private
 */
int hash_string(int hash_table_capacity, char *string, int strlen);

/**
 * @brief This calculates what the actual capacity of the map will be. Given a result from hash_log2, it gets the maximum storable for that many bits. For example, hash_upperLimit(3) returns the maximum that 3 bits can hold, which is 8. hash_upperLimit(4) returns 16.
 * @param bitsize The number of bits to calculate the max from.
 * @returns The max value that number of bits can hold.
 * @private
 */
int hash_upperLimit(int bitsize);

// ------------------------------------
//        General Map Operations
// ------------------------------------

/**
 * @struct _map_bucket
    @brief map_bucket is an endpoint in the map. It is also a node in a linked list; if there were collisions, then the buckets are appended to the linked list at that location, then traversed until the matching key is found.
    @private
*/
struct _map_bucket
{
    /// @private
    char *key;
    /// @private
    void *data;
    /// @private
    struct _map_bucket *next;
};

/**
 * \struct map
   \brief A map. Stores key-value pairs for near constant lookup and insertion time.

   \note Use NewMap() to create a new map.
   \note Use Map_Set() to set a key in the map.
   \note Use Map_Get() to get a value from the map.

   The values stored are of type void pointer.
*/
typedef struct
{
    int size; /** \property The number of base buckets in this map. */
    struct _map_bucket *buckets; /** The buckets for this map.  \property  */
} map;

/**
    @struct map_result
    @brief The result of a map retrieval.
*/
typedef struct
{
    
    short found; /** 1 if succesfully found. 0 if not found.  \property */
    void *data; /** The data linked with that key; indeterminate if found == 0.  \property */
} map_result;

/**
    Creates a new map. The map capacity will be a power of 2 that is large enough to contain the estimated size.

    @param capacity The estimated required capacity of the map.
    @returns A pointer to the heap allocated map.
*/
map *NewMap(int capacity);

/**
    @brief Sets a value in the map.
    @param map The map to set a key in.
    @param key The key to use.
    @param keylen The length of the key.
    @param value The pointer to the data stored at that location.
*/
void Map_Set(map *a_map, char *key, void *value);

/**
    @brief Gets a value from the map. It will return a map_get_result describing whether it was succesful, and possibly containing the data sought, or NULL if it was unsuccesful.
    @param map The map to retrieve from.
    @param key The key of the item.
    @returns A map_get_result containing the sought data.
*/
map_result Map_Get(map *a_map, char *key);

/**
    @brief Deletes a key from the map. Returns a map_get_result describing whether the delete was succesful and containing the removed data, if extant.
    @param map The map to delete the key from.
    @param key The key to delete.
    @param free_it Whether to call free() on the underlying data.
    @returns A map_get_result with the data that was removed.
*/
map_result Map_Delete(map *a_map, char *key, short free_it);

#endif
