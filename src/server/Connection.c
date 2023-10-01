/**
 * \addtogroup Connection
 * @{
*/
#include "Connection.h"
#include <stdio.h>

void * StartConnectionThread(void * connection)
{
    Connection * myConnection = connection;
    time(&(myConnection->time_connected));
    return NULL;
}


/**
 * @}
*/
