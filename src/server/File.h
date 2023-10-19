#ifndef Files_h
#define Files_h
/**
 * \defgroup Files
 * \brief This module contains functions that interact with files.
 * @{
*/
#include <stdio.h>
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

/**
 * Contains settings for the server. 
 * Each setting row contains a key, 0 or more space, an '=' symbol, and a value.
 * Valid keys:
 * @note port; the port the server will listen on.
 * @note send_buffer_size; the size of the send buffer
 * @note receive_buffer_size; the size of the receive buffer
 * @note backlog; the quantity of allowed backlogged unprocessed connections.
 * @todo Create file functions for this.
*/
#define SERVER_SETTINGS_FILE "server-settings.txt"

// ~~~~~ General File Functions ~~~~~ //

/**
 * Determines if a file indicated by filename exists and is accesible by the user.
 * @returns 0 if the file does not exist. 1 if the file exists and the user has access. 2 if the file exists and the user does not have read and write permissions.
*/
short FileStatus(char * filename);

/**
 * Will call fopen() on a file and put default data inside, or nothing if defaultContents is NULL. Will print the results of its attempt.
 * @warning Does not close the file; returns the open file.
 * @note Prints successes and errors.
 * @param filename The file name to create or open.
 * @param defaultContents The contents to put in the file, if creating a default file, or NULL if no contents should be added.
 * @returns The opened file, or NULL on failure. 
*/
FILE * CreateOrOpenFileVerbose(char * filename, char * defaultContents);

/**
 * Reads the registered file into the user's map, by checking the IDs in the first field and setting the data at that location.
 * @note Prints warnings and errors.
 * @param reg_file The registered users file, open for reading.
 * @param users_map The user's map to read into.
 * @returns 0 if success, error code if there was an error.
*/
int ReadRegisteredFileIntoUsersMap(FILE * reg_file, map * users_map);


/**
 * Reads the settings file into the settings map, by checking each line for a key value pair separated by a "=". It mallocs each key and value string it finds.
 * @note Prints warnings and errors.
 * @param settings_file The settings file, open for reading.
 * @param users_map The settings_map to read into.
 * @returns 0 if success, an error code if there was an error. 
*/
int ReadSettingsFileIntoSettingsMap(FILE * settings_file, map * settings_map);

/**
 * @brief Updates the registered file with of all users from user map that are marked as registered
 * 
 * @param reg_file file to update to
 * @param users_map the map of users to use to update
 */
void UpdateRegisteredFileFromUsersMap(FILE * reg_file, map * users_map);

/**
 * @}
*/
#endif 