#ifndef Connection_h
#define Connection_h
/**
 * \defgroup Connection
 * \brief This module handles an individual user's active connection.
 * @{
*/

typedef struct {
    /** Whether this connection is closed (0) or active (1) */
    short active;

} Connection;

/**
 * @}
*/
#endif 