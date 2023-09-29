#ifndef Data_h
#define Data_h
/**
 * \defgroup Data
 * \brief This module describes structures used in this program.
 * @{
*/

/**
    The total count of records.
*/
#define RECORD_COUNT 17
/**
    The amount of memory (bytes) required to be allocated for the ID field.
    Equal to the longest name in Data_IDs, "mes08346", plus the null terminator
*/
#define ID_MAX_LENGTH 9
/**
    The amount of memory (bytes) required to be allocated for the Name field.
    Equal to the longest name in Data_Names, "Assefa Ayalew Yoseph", plus the null terminator
*/
#define NAME_MAX_LENGTH 21

/**
 * The amount of memory (bytes) required to be allocated for the IP field.
 * Large enough to store '111.111.111.111' plus the null terminator.
*/
#define IP_LENGTH 16


/**
 * An array of the accepted userIDs.
*/
extern char * accepted_userIDs[];

/**
 * An array of the full names, where the index of the name corresponds to the id in accepted_userIDs.
*/
extern char * userFullNames[];

/**
 * A User of this server. The ID and Name fields are populated initially. GPA and age are populated at the time a user is registered, and saved and loaded from a file. Active is set and unset when a user connects. IP is set each time a user connects, and saved in the file.
*/
typedef struct 
{
    /// @brief The user ID; equal to an element in accepted_userIDs.
    char id[ID_MAX_LENGTH];
    /// @brief The user's real name; equal to an element in userFullNames.
    char name[NAME_MAX_LENGTH];
    /// @brief The user's age, randomized between 18 and 22.
    int age;
    /// @brief The user's gpa, randomized between 2.5 and 4.0.
    float gpa;
    /// @brief Whether the user is connected.
    short connected;
    /// @brief The last IP used by the user; set on connection.
    char ip[IP_LENGTH];
    /// @brief A unix timestamp representing the last time a user connected.
    long lastConnection;
} User;

/**
 * @}
*/
#endif 