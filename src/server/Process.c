/**
 * \addtogroup Process
 * @{
*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "Data.h"
#include "Build.h"
#include "map.h"
#include "File.h"
#include "Util.h"
#include "Server.h"
#include "Log.h"
#include "Connection.h"

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

int _initializeLogger() {
    char* fileName = "log.txt";
    int printLevel, LogLevel, printAlltoStdOut;
    map_result result = Map_Get(settings_map, "log_file");
    if(!result.found) {
        printYellow("No output file found. Defaulting to 'log.txt'\n");
    } else {
        fileName = result.data;
    }

    result = Map_Get(settings_map, "print_level");
    if(!result.found) {
        printYellow("No print_level found, defaulting to 3\n");
        printLevel = 3;
    } else {
        printLevel = atoi(result.data);
        if(printLevel < 0 || printLevel > 5) {
            printYellow("Invalid print_level of %d, defaulting to 3\n", printLevel);
            printLevel = 3;
        }
    }
    
    result = Map_Get(settings_map, "log_level");
    if(!result.found) {
        printYellow("No log_level found, defaulting to 3\n");
        LogLevel = 3;
    } else {
        LogLevel = atoi(result.data);
        if(LogLevel < 0 || LogLevel > 5) {
            printYellow("Invalid log_level of %d, defaulting to 3\n", LogLevel);
            LogLevel = 3;
        }
    }

    result = Map_Get (settings_map, "log_to_console");
    if(!result.found) {
        printYellow("No log_to_console found, defaulting to true\n");
        printAlltoStdOut = 1;
    } else {
        if(strcmp(result.data, "true") == 0) {
            printAlltoStdOut = 1;
        } else if(strcmp(result.data, "false") == 0) {
            printAlltoStdOut = 0;
        } else {
            printYellow("invalid data in log_to_console, defaulting to true\n");
            printAlltoStdOut = 1;
        }
    }
    return 1;
}

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

    printf("Initializing logger.\n");
    int logger_initialized = _initializeLogger();
    if(!logger_initialized) {
        printRed("Failed to initalize logger.\n");
    }
    

    printf("Initializing server.\n");
    int server_initialized = InitializeServer(settings_map);
    if(!server_initialized) {
        printRed("Failed to initialize server.\n");
        return 0;
    }


    return 1;
}

void SignalHandle(int signo) {
    if(signo == SIGINT || signo == SIGTERM) {
        printYellow("Received signal. Shutting down server.\n");
        DeleteLockfile();
        exit(0);
    }

}

int RunCommand() {
    if (FileStatus(LOCKFILE) > 0)
    {
        printf("Server process already running.\n");
        return 0;
    }
    signal(SIGTERM, SignalHandle);
    signal(SIGINT, SignalHandle);
    int lockfile_success = CreateLockfile();
    if(!lockfile_success) {
        printRed("Failed to create Lockfile! Server cannot start.");
        return 0;
    }
    int init_success = Initialize();
    if(!init_success) {
        printRed("Could not start the server due to failed initialization.\n");
        return 0;
    }
    printf("Running server.\n");
    int server_success = StartServer(users_map);
    if(!server_success) {
        printRed("There was a problem running the server.\n");
        return 0;
    }
    int delete_lockfile_success = DeleteLockfile();
    if(!delete_lockfile_success) {
        printRed("There was a problem deleting the Lockfile.\n");
        return 0;
    }
    return 1;   
}

void RunHeadless(char *processName) {
    if (FileStatus(LOCKFILE) > 0)
    {
        printf("Server process already running.\n");
        return;
    }
    char commandFront[] = " nohup ";
    char commandEnd[] = " & exit";
    size_t comm_length = strlen(commandFront) + strlen(commandEnd) + strlen(processName) + 1;
    char *commandFull = malloc(comm_length * sizeof(char));
    memset(commandFull, 0, comm_length * sizeof(char));
    strcpy(commandFull, commandFront);
    strcat(commandFull, processName);
    strcat(commandFull, commandEnd);

    printf("Executing: %s\n", commandFull);
    popen(commandFull, "we");
    printf("Server running headlessly.\n");
}

int TerminateExistingServer()
{
    FILE *file = fopen(LOCKFILE, "r");
    if (file == NULL)
    {
        perror("Error opening lockfile");
        return -1;
    }
    int need_rewrite;
    int pid = 0;
    fscanf(file, "%d %d", &need_rewrite, &pid);
    fclose(file);
    if (pid > 0)
    {
        return kill(pid, SIGTERM);
    }
    return -2;
}

void StopCommand() {
    printYellow("\nStopping server...\n");
    int err = TerminateExistingServer();
    if (err)
    {
        if (err == -1)
        {
            printRed("Server isn't running.\n");
        }
        else if (err == -2)
        {
            printRed("Lockfile did not contain a valid process id!\n");
        }
        else
        {
            printRed("Sending terminate signal failed!\n");
        }
    }
    else
    {
        printGreen("Server terminated.\n");
    }
}

void ResetCommand() {
    if(FileStatus(REGISTERED_FILE)) {
        fclose(fopen(REGISTERED_FILE, "w")); //empties the registered file
    }
}
/**
 * @}
*/