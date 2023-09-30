/**
 * \addtogroup Process
 * @{
*/
#include <stdio.h>
#include "Data.h"
#include "Build.h"
#include "map.h"
#include "File.h"
#include "Util.h"

/** The array of users. This will be populated on initialize by functions in Build. */
User * users_array;
/** The map of userIDs to users. Populated on Initialize by functions in Build.*/
map * users_map;
/** The number of active clients.*/
int active_clients;

int Initialize() {

    // Create the data structures on the heap.
    printf("Initializing User data structures.\n");
    users_array = CreateUsersArray(accepted_userIDs, userFullNames, RECORD_COUNT);
    users_map = CreateUsersMap(users_array, RECORD_COUNT);
    active_clients = 0;
    printGreen("User data structures initialized.\n");

    // Create the registered file that tracks registered users. 
    printf("Checking for registered file.\n");
    FILE * reg_file = CreateOrOpenFileVerbose(REGISTERED_FILE, NULL);
    if(reg_file == NULL) {
        printRed("Initialization failed during accessing of file: %s.\n", REGISTERED_FILE);
        return 0;
    }

    // Update the User's map with with the data from the registered file. 
    printf("Reading registered file.\n");
    int read_error = ReadRegisteredFileIntoUsersMap(reg_file, users_map);
    fclose(reg_file);
    if(read_error) {
        printRed("Initialization failed during reading of file: %s.\n", REGISTERED_FILE);
        return 0;
    }
    printGreen("Loaded %s into users map.\n", REGISTERED_FILE);
    return 1;
}

/**
 * @}
*/