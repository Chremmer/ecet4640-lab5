#ifndef Files_h
#define Files_h
/**
 * \defgroup Files
 * \brief This module contains functions that interact with files.
 * @{
*/
#include "map.h"

// ~~~~ Macros ~~~~ //

/**
 * The presence of a lockfile indicates that a server process is already running. The lockfile contains the process ID of the running process.
*/
#define LOCKFILE "/tmp/lab5.lock"

/**
 * This file contains a list of registered users and their data, with fields tab-delimited.
 * @note (1) The userID of the user.
 * @note (2) The age of the user.
 * @note (3) The GPA of the user.
 * @note (4) The IP address of the user.
 * @note (5) The last connection time of a user.
*/
#define REGISTERED_FILE "registered.txt"

// ~~~~~ General File Functions ~~~~~ //

/**
 * Determines if a file indicated by filename exists and is accesible by the user.
 * @returns 0 if the file does not exist. 1 if the file exists and the user has access. 2 if the file exists and the user does not have read and write permissions.
*/
short FileStatus(char * filename);

/**
 * Reads the registered file into the user's map, by checking the IDs in the first field and setting the data at that location.
 * @param reg_file The registered users file.
 * @param users_map The user's map to read into.
*/
void ReadRegisteredFileIntoUsersMap(FILE * reg_file, map * users_map);
/**
 * @}
*/
#endif 