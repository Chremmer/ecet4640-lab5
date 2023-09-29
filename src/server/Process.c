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
    printf("Initializing User data structures.\n");
    users_array = CreateUsersArray(accepted_userIDs, userFullNames, RECORD_COUNT);
    users_map = CreateUsersMap(users_array, RECORD_COUNT);
    active_clients = 0;
    printGreen("User data structures initialized.\n");

    printf("Checking for registered file.\n");
    int reg_exists = FileStatus(REGISTERED_FILE);
    FILE * reg_file;
    if(reg_exists == 0) {
        printf("Creating registered file.\n");
        reg_file = fopen(REGISTERED_FILE, "w+");
        if(reg_file == NULL) {
            printf("%sFailed to create %s.\n", COLOR_RED, REGISTERED_FILE);
            perror("Error:");
            printf(COLOR_RESET);
            return 0;
        }
        printGreen("%s created.\n", REGISTERED_FILE);
    } else if(reg_exists == 2) {
        printRed("Error: %s exists but you do not have permission to access it.\n", REGISTERED_FILE);
        return 0;
    } else {
        printf("Opening registered file.\n");
        reg_file = fopen(REGISTERED_FILE, "r");
        if(reg_file == NULL) {
            printf("%sFailed to open %s.\n", COLOR_RED, REGISTERED_FILE);
            perror("Error:");
            printf(COLOR_RESET);
            return 0;
        }
        printGreen("%s opened.\n", REGISTERED_FILE);
    }

    printf("Reading registered file.\n");
    ReadRegisteredFileIntoUsersMap(reg_file, users_map);
    printGreen("Loaded %s into users map.\n", REGISTERED_FILE);
    fclose(reg_file);
    return 1;
}

/**
 * @}
*/