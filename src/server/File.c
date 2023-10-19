/**
 * \addtogroup Files
 * @{
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "File.h"
#include "Data.h"
#include "Util.h"
#include "Log.h"

short FileStatus(char * filename) {
    int err = access(filename, F_OK);
    if(!err) {
        err = access(filename, F_OK | R_OK | W_OK);
        if(!err) {
            return 1;
        }
        return 2;
    }
    return 0;
}

FILE * CreateOrOpenFileVerbose(char * filename, char * defaultContents) {
    FILE * file = NULL;
    int status = FileStatus(filename);

    if(status == 2) {
        printRed("Error: %s exists but you do not have permission to access it.\n", filename);
        return NULL;        
    }

    if(status == 0) {
        printf("Creating %s.\n", filename);
        file = fopen(filename, "w+");
    } else if(status == 1) {
        printf("Opening %s.\n", filename);
        file = fopen(filename, "r+");
    }


    if(file == NULL) {
        printf(COLOR_RED);
        if(status == 0) {
            printf("Failed to create %s.\n", filename);
            perror("Error: ");
        } else if(status == 1) {
            printf("Failed to open %s.\n", filename);
            perror("Error: ");
        } else {
            printf("Unknown error opening %s.", filename);
        }
        printf(COLOR_RESET);
        return NULL; 
    }

    if(status == 0) {
        printGreen("Created %s.\n", filename);
        if(defaultContents != NULL) {
            fpos_t start_pos;
            fgetpos(file, &start_pos);
            fprintf(file, defaultContents, 0);
            fsetpos(file, &start_pos);
        }
    } else if(status == 1) {
        printGreen("Opened %s.\n", filename);
    }

    return file;
}


int ReadRegisteredFileIntoUsersMap(FILE * reg_file, map * users_map) {

    char userID[ID_MAX_LENGTH];
    int user_age;
    float user_gpa;
    char userLastIP[IP_LENGTH];
    long lastConnection;

    int scan_items;
    int line = 1;

    while( (scan_items = fscanf(reg_file, "%s\t%d\t%f\t%s\t%ld", userID, &user_age, &user_gpa, userLastIP, &lastConnection)) == 5) {
        map_result result = Map_Get(users_map, userID);
        if(result.found == 0) {
            printYellow("Couldn't find user %s. Continuing read.\n", userID);
            continue; 
        }
        User * user = (User*)result.data;
        user->age = user_age;
        user->gpa = user_gpa;
        strcpy(user->ip, userLastIP);
        user->lastConnection = lastConnection;
        user->registered = 1;
        line++;
    } 

    if(scan_items != EOF) {
        printRed("Error scanning registered file on line %d. Expected 5 items but had %d.\n", line, scan_items);
        return 1;
    }
    return 0;
}

void UpdateRegisteredFileFromUsersMap(FILE * reg_file, map * users_map) {
    int i;
    for(i = 0; i < RECORD_COUNT; i++) {
        map_result result = Map_Get(users_map, accepted_userIDs[i]);
        if(!result.found) {
            LogfError("User %s was not found in users map.", accepted_userIDs[i]);
            continue;
        }

        User * user = (User *) result.data;
        if(user->registered) {
            fprintf(reg_file, "%s\t%d\t%f\t%s\t%ld", user->id, user->age, user->gpa, user->ip, user->lastConnection);
        }
    }
}

int ReadSettingsFileIntoSettingsMap(FILE * settings_file, map * settings_map) {
    char key_read[100];
    char value_read[100];

    int scan_items;
    int line = 1;

    while( (scan_items = fscanf(settings_file, " %s = %s ", key_read, value_read)) == 2) {
        char * key_alloc = malloc( (strlen(key_read)+1) * sizeof(char));
        memset(key_alloc, 0, strlen(key_read)+1);
        strcpy(key_alloc, key_read);
        char * val_alloc = malloc( (strlen(value_read)+1) * sizeof(char));
        memset(val_alloc, 0, strlen(value_read)+1);
        strcpy(val_alloc, value_read);
        Map_Set(settings_map, key_alloc, val_alloc);
        line++;
    }

    if(scan_items != EOF) {
        printRed("Error scanning settings file on line %d. Expected 2 items but had %d.\n", line, scan_items);
        return 1;
    }
    return 0;
}

int CreateLockfile()
{
    FILE * file = fopen(LOCKFILE, "w");
    if(file == NULL) {
        return 0;
    }
    fprintf(file, "0 %d", getpid());
    fclose(file);
    return 1;
}

int DeleteLockfile()
{
    return remove(LOCKFILE);
}

/**
 * @}
*/