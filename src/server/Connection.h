#ifndef Connection_h
#define Connection_h
/**
 * \defgroup Connection
 * \brief This module handles an individual user's active connection.
 * @{
*/
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <Data.h>
#include "map.h"

/** 
 * Shared between the Connections and the Server. 
*/
typedef struct {
    /** The user's map. */
    map * users;
    /** A mutex to provide mutual-exclusion to connection threads operating on the user's map. */
    pthread_mutex_t mutex;
    /** Whether there were changes to the user's map that need to be saved in a file. */
    short dirty;
    /** Whether the server is shutting down. */
    short shutting_down;

    /** Passed along from server settings at the time shared is initialized*/
    size_t send_buffer_size;
    /** Passed along from server settings at the time shared is initialized*/
    size_t receive_buffer_size;
} ClientShared;

#define ClientState_ENTRY 1
#define ClientState_ACCESSING 2
#define ClientState_REGISTERED 3 
typedef short ClientState;

#define ConnectionStatus_CLOSED 0
#define ConnectionStatus_ACTIVE 1
#define ConnectionStatus_CLOSING 2
typedef short ConnectionState;

/**
 * Data for a single client socket connection to the server. Passed into the thread runner as the parameter. 
*/
typedef struct {
    /** Whether this connection is closed (0) or active (1) or closing (2). This is set by the SERVER just prior to starting the thread. The thread sets it back to 0 when it is completely done.*/
    ConnectionState status;
    /** The underlying socket file descriptor. */
    int socket;
    /** The socket address of the connection. */
    struct sockaddr_in address;
    /** The actual size of the client address; send by accept. */
    socklen_t address_length;
    /** The pthread ID of this client thread. */
    pthread_t thread_id;
    /** When the client connected. */
    time_t time_connected;
    /** The client state. */
    ClientState state;
    /** The user associated with this client. */
    User * user;

} Connection;

/**
 * Initializes the structure that shares data between connections and the server.
 * @param users_map The map of User structs.
 * @returns A pointer to the same ClientShared object seen by the connection threads.
*/
ClientShared * InitializeShared(map * users_map, size_t send_buffer_size, size_t receive_buffer_size);


/** Starts a connection thread
 * @param connection A pointer to a Connection structure from the server's connections array.
 * @returns NULL
 */
void * StartConnectionThread(void * connection);

/**
 * Sends send_buffer to the socket referenced by connection, then memsets send_buffer to 0.
 * Memsets receive_buffer to 0, then receives a message from the client. If this length is 0, assumes the connection was closed and sets connection->active to 0.
 * @warning send_buffer and receive_buffer must be the size specified in shared.
 * @param send_buffer A message to send to the client.
 * @param receive_buffer The message received by the client.
 * @param connection The socket's Connection
 * @returns The number of bytes read into receive_buffer, or 0 if the connection closed. 
*/
int MessageOrClose(char * send_buffer, char * receive_buffer, Connection * connection);

/**
 * Sends send_buffer to the socket referenced by connection, then sets connection.active to 0.
 * @param send_buffer The send buffer. Should be shared.send_length in size.
 * @param connection The socket's Connection.
*/
void MessageAndClose(char * send_buffer, Connection * connection);

/**
 * Starts an update thread.
 * This thread is responsible for checking shared.dirty. If it is, it writes the user's data to a file and sets dirty to 0.
 * @param paramter None.
 * @returns NULL
*/
void * StartUpdateThread(void * parameter);

/**
 * Registers the user from connection
 * 
 * @param connection connection the users is on
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
int _register(Connection * connection, char* response);

/**
 * Returns the functions available to the user
 * 
 * @param connection connection the user is on
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
void _help(Connection* connection, char* response);

/**
 * Returns the info of the user to the client
 * 
 * @param connection connection the user is on
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
int _myinfo(Connection* connection, char* response);

/**
 * Sets response buffer to be a list a userIDs that are connected.
 * 
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
void _who(char* response);

/**
 * Randomly changes the age of the user
 * 
 * @param connection connection the user is on
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
void _rand_age(Connection* connection, char* response);

/**
 * Randomly changes the gpa of the user
 * 
 * @param connection connection the user is on
 * @param response fills the response buffer with what to send to the client
 * @return int 1 if successful, 0 if not
 */
void _rand_gpa(Connection* connection, char* response);
/**
 * @}
*/
#endif 
