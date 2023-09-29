/**
 * \addtogroup Files
 * @{
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "File.h"
#include "Data.h"

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


void ReadRegisteredFileIntoUsersMap(FILE * reg_file, map * users_map) {

    char userID[ID_MAX_LENGTH];
    int user_age;
    float user_gpa;
    char userLastIP[IP_LENGTH];
    long lastConnection;

    while (fscanf(reg_file, "%s\t%d\t%f\t%s\t%ld", userID, &user_age, &user_gpa, userLastIP, &lastConnection) == 5)
    {
        map_result result = Map_Get(users_map, userID);
        if(result.found == 0) {
            continue; 
        }
        // ((User *)result.data)->
    }
}

/**
 * @}
*/