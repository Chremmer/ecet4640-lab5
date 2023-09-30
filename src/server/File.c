/**
 * \addtogroup Files
 * @{
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "File.h"
#include "Data.h"
#include "Util.h"

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
            fprintf(file, defaultContents, 0);
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
        scan_items = fscanf(reg_file, "%s\t%d\t%f\t%s\t%ld", userID, &user_age, &user_gpa, userLastIP, &lastConnection);
        line++;
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
    } 

    if(scan_items != EOF) {
        printRed("Error scanning registered file on line %d. Expected 5 items but had %d.\n", line, scan_items);
        return 1;
    }
    return 0;
}

/**
 * @}
*/