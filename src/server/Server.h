#ifndef Server_h
#define Server_h
/**
 * \defgroup Server
 * \brief Functions for running the server.
 * @{
*/
#include <stdint.h>
#include <time.h>
#include "map.h"
#include "Connection.h"

/**
 * Defines the properties for the server.
 * 
 * Defined in server-settings.txt, a configuration file.
 * 
 * @todo Read properties from a configuration file? Wrap a sockaddr_in? Should it contain buffer size? Should it contain an array of active threads? 
*/
typedef struct {
    /** The port the server will connect on. */
    uint16_t port;
    /** The size of each send buffer. */
    size_t send_buffer_size;
    /** The size of each receive buffer. */
    size_t receive_buffer_size;
    /** The size of the backlog of unprocessed connections. */
    int backlog;
    /** The number of active connections. */
    int active_connections;
    /** The maximum number of active connections the server supports. */
    int max_connections;
    /** The time the server was started. */
    time_t time_started;
} ServerProperties;

/**
 * Initializes the server properties structure and the structures for holding Connection objects.
 * @note Prints initialization status.
 * @returns 1 of it was able to initialize, otherwise 0.
*/
int InitializeServer();

/**
 * Starts the server. 
 * @note This is a blocking call that will start a loop until SIGINT is received.
 * @param users_map The user's map.
 * @returns 1 if the server ran and shutdown gracefully, 0 if there was an error during setup.
*/
int StartServer(map * users_map);

/**
 * Iterates through the Connections array until it finds one whose 'active' field is false and returns it.
 * If it iterates through the array and fails to find a connection, it returns NULL.
 * @returns A Connection struct or null.
*/
Connection * NextAvailableConnection();

/**
 * @}
*/
#endif 