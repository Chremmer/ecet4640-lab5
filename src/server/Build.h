#ifndef Build_h
#define Build_h
/**
 * \defgroup Build
 * \brief Functions for creating and populating data structures.
 * @{
*/
#include "Data.h"
#include "map.h"

/**
 * Mallocs a new array of User structs.
 * @param userIDs An array of userIDs to set.
 * @param userNames An array of userNames corresponding to the userIDs.
 * @param recordsCount The number of records in userIDs and userNames, and the size of the created an array.
 * @returns A malloced array of user structs.
*/
User * CreateUsersArray(char ** userIDs, char ** userNames, int recordsCount);

/**
 * Given a user's array, initializes a new map that points to the underlying data in the array, using the user's ID as a key.
 * 
 * @param usersArray The array used to build the user's map.
 * @param recordsCount The number of records in the user's array.
 * @returns A map
*/
map * CreateUsersMap(User * usersArray, int recordsCount);



/**
 * @}
*/
#endif