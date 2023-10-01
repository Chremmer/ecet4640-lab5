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

typedef struct {
    /** Whether this connection is closed (0) or active (1) */
    short active;
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

} Connection;


/** Starts a connection thread */
void * StartConnectionThread(void * connection);

/**
 * @}
*/
#endif 