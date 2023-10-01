/**
 * \addtogroup Connection
 * @{
*/
#include "Connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Util.h"

ClientShared shared;

ClientShared * InitializeShared(map * users_map, size_t send_buffer_size, size_t receive_buffer_size)
{
    shared.users = users_map;
    shared.dirty = 0;
    shared.shutting_down = 0;
    shared.send_buffer_size = send_buffer_size;
    shared.receive_buffer_size = receive_buffer_size;
    pthread_mutex_init(&(shared.mutex), NULL);
    return &shared;
}

void * StartUpdateThread(void * parameter)
{
    while(shared.shutting_down == 0) {
        sleep(1);
        // update the registered file 

    }
    return NULL;
}

void * StartConnectionThread(void * p_connection)
{
    Connection * connection = (Connection *) p_connection;
    connection->state = ClientState_ENTRY;
    connection->user = NULL;
    time(&(connection->time_connected));
    // allocate send and receive buffers.
    char * send_buffer = malloc(shared.send_buffer_size);
    char * receive_buffer = malloc(shared.receive_buffer_size);
    int bytes_received;
    map_result result;

    // ask for their user ID initially, or disconnect them.
    strcpy(send_buffer, "~Message~Welcome. Please send your user ID.");
    bytes_received = MessageOrClose(send_buffer, receive_buffer, connection);
    if(connection->status == ConnectionStatus_ACTIVE) {
        result = Map_Get(shared.users, receive_buffer);
        if(!result.found)
        {
            printYellow("Unauthorized access attempt by %s with name '%s'.\n", inet_ntoa(connection->address.sin_addr), receive_buffer);
            strcpy(send_buffer, "~Error~You are not permitted to access this resource.");
            MessageAndClose(send_buffer, connection);
            // send a one-way message to the client
        } else {
            User * user = (User *) result.data;
            if(user->connected) {
                printYellow("User %s attempted to double connect from IP %s.\n", connection->user->id, inet_ntoa(connection->address.sin_addr));
                strcpy(send_buffer, "~Error~You are already connected.");
                MessageAndClose(send_buffer, connection);
                // send the other connected user an informative message?
            } else {
                connection->user = user;
                connection->user->connected = 1;
                if(connection->user->registered) {
                    connection->state = ClientState_REGISTERED;
                } else {
                    connection->state = ClientState_ACCESSING;
                }                
            }
        }
    }


    while(connection->status == ConnectionStatus_ACTIVE)
    {
        if(connection->state == ClientState_ACCESSING) {
            // call a function for processing this state.
        } else if(connection->state == ClientState_REGISTERED)
        {
            // call a function for processing this state.
        } else {
            printRed("Client entered invalid state. Disconnecting. \n");
            strcpy(send_buffer, "~Error~You entered an invalid state!");
            MessageAndClose(send_buffer, connection);
            connection->status = ConnectionStatus_CLOSING;
        }
    }

    if(connection->user != NULL) {
        connection->user->connected = 0;
    }
    

    free(send_buffer);
    free(receive_buffer);
    close(connection->socket);
    connection->status = ConnectionStatus_CLOSED;
    return NULL;
}


int MessageOrClose(char * send_buffer, char * receive_buffer, Connection * connection) {
    memset(receive_buffer, 0, shared.receive_buffer_size);
    if(send(connection->socket, send_buffer, shared.send_buffer_size, 0) < 0) {
        printRed("Failed to send message to %d. Disconnecting.\n", inet_ntoa(connection->address.sin_addr));
        perror("Error:");
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    int received_size = recv(connection->socket, receive_buffer, shared.receive_buffer_size, 0);
    if(received_size < 0) {
        printRed("Failed to receive message from %d. Disconnecting.\n", inet_ntoa(connection->address.sin_addr));
        perror("Error: ");
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    if(received_size == 0 ) {
        printBlue("%d disconnected.", inet_ntoa(connection->address.sin_addr));
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    memset(send_buffer, 0, shared.send_buffer_size);
    return received_size;
}

void MessageAndClose(char * send_buffer, Connection * connection) {
    strcat(send_buffer, "~Disconnect~");
    send(connection->socket, send_buffer, shared.send_buffer_size, 0);
    connection->status = ConnectionStatus_CLOSING;
}

/**
 * @}
*/
