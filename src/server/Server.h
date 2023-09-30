#ifndef Server_h
#define Server_h
/**
 * \defgroup Server
 * \brief Functions for running the server.
 * @{
*/
#include <stdint.h>

/**
 * Defines the properties for the server.
 * 
 * @todo Read properties from a configuration file? Wrap a sockaddr_in? Should it contain buffer size? Should it contain an array of active threads? 
*/
typedef struct {
    uint16_t port;

} ServerProperties;

/**
 * @}
*/
#endif 