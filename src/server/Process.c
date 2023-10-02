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
#include "Server.h"

/** The array of users. This will be populated on initialize by functions in Build. */
User * users_array;
/** The map of userIDs to users. Populated on Initialize by functions in Build.*/
map * users_map;
/** The map of settings stored in the server settings file. */
map * settings_map;
/** The default contents of the settings file, if it doesn't exist. */
char * default_settings = "port                = 3000\n"
                          "send_buffer_size    = 1024\n"
                          "receive_buffer_size = 1024\n"
                          "backlog             = 10\n"
                          "max_connections     = 20\n"
                          "log_file            = log.txt\n"
                          "log_level           = 1\n"
                          "log_to_console      = true";

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

    printf("Reading settings file.\n");
    settings_map = NewMap(50);
    FILE * settings_file = CreateOrOpenFileVerbose(SERVER_SETTINGS_FILE, default_settings);
    if(settings_file == NULL) {
        printRed("Initialization failed during accessing of file: %s.\n", SERVER_SETTINGS_FILE);
        return 0;
    }
    int settings_read_err = ReadSettingsFileIntoSettingsMap(settings_file, settings_map);
    if(settings_read_err) {
        printRed("Initialization failed while reading settings file %s. Correct this file or delete it so a default can be generated.\n", SERVER_SETTINGS_FILE);
        return 0;
    }
    fclose(settings_file);
    printGreen("Read %s.\n", SERVER_SETTINGS_FILE);

    printf("Initializing server.\n");
    int server_initialized = InitializeServer(settings_map);
    if(!server_initialized) {
        printRed("Failed to initialize server.\n");
        return 0;
    }

    return 1;
}

int RunCommand() {
    int init_success = Initialize();
    if(!init_success) {
        printRed("Could not start the server due to failed initialization.\n");
        return init_success;
    }
    printf("Running server.\n");
    int server_success = StartServer(users_map);
    if(!server_success) {
        printRed("There was a problem running the server.\n");
        return 0;
    }
    return 1;
    
}
/**
 * @}
*/