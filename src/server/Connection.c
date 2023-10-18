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
#include "Log.h"
#include "Data.h"

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
        pthread_mutex_lock(&(shared.mutex));
        shared.dirty = 0;
        pthread_mutex_unlock(&(shared.mutex));
        sleep(1);
        /// @todo update the registered file 

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
    strcpy(send_buffer, "<Message>Welcome. Please send your user ID.");
    bytes_received = MessageOrClose(send_buffer, receive_buffer, connection);
    if(connection->status == ConnectionStatus_ACTIVE) {
        result = Map_Get(shared.users, receive_buffer);
        if(!result.found)
        {
            printYellow("Unauthorized access attempt by %s with name '%s'.\n", inet_ntoa(connection->address.sin_addr), receive_buffer);
            strcpy(send_buffer, "<Error>No such user");
            MessageAndClose(send_buffer, connection);
            // send a one-way message to the client
        } else {
            User * user = (User *) result.data;
            if(user->connected) {
                printYellow("User %s attempted to double connect from IP %s.\n", user->id, inet_ntoa(connection->address.sin_addr));
                strcpy(send_buffer, "<Error>You are already connected.");
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

    if(connection->state == ClientState_ACCESSING && connection->status == ConnectionStatus_ACTIVE) {
        strcpy(send_buffer, "<Message>Say something, unregistered user!");
    } else if (connection->state == ClientState_REGISTERED && connection->status == ConnectionStatus_ACTIVE) {
        strcpy(send_buffer, "<Message>Say something, registered user!");
    }

    while(connection->status == ConnectionStatus_ACTIVE)
    {
        //This is very repetitive, do we need separate separate paths for registered/unregistered?
        //I wonder if it'd be better to have the same commands, but they do different actions based on registered/unregistered
        if(connection->state == ClientState_ACCESSING) {
            MessageOrClose(send_buffer, receive_buffer, connection);
            if (strcmp(receive_buffer, "help") == 0) {
                _help(connection, send_buffer);
            } else if (strcmp(receive_buffer, "exit") == 0) {
                strcpy(send_buffer, "<Message>Goodbye.");
                MessageAndClose(send_buffer, connection);
            } else if (strcmp(receive_buffer, "register") == 0) {
                _register(connection, send_buffer);
            } else {
                strcpy (send_buffer, "<Error>Invalid command, use 'help' for list of commands");
            }
        } else if(connection->state == ClientState_REGISTERED) {
            MessageOrClose(send_buffer, receive_buffer, connection);
            if (strcmp(receive_buffer, "help") == 0) {
                _help(connection, send_buffer);
            } else if (strcmp(receive_buffer, "exit") == 0) {
                strcpy(send_buffer, "<Message>Goodbye.");
                MessageAndClose(send_buffer, connection);                
            } else if (strcmp(receive_buffer, "myinfo") == 0) {
                _myinfo(connection, send_buffer);
            } else if (strcmp(receive_buffer, "who") == 0) {
                _who(send_buffer);
            } else if(strcmp(receive_buffer, "random-gpa") == 0) {
                _rand_gpa(connection, send_buffer);
            } else if(strcmp(receive_buffer, "random-age") == 0) {
                _rand_age(connection, send_buffer);
            } else {
                strcpy(send_buffer, "<Error>Invalid command, use 'help' for list of commands");
            }
            // call a function for processing this state.
        } else {
            printRed("Client entered invalid state. Disconnecting. \n");
            strcpy(send_buffer, "<Error>You entered an invalid state!");
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
    if(connection->user != NULL) {
        connection->user->connected = 0;
    }
    connection->status = ConnectionStatus_CLOSED;
    return NULL;
}


int MessageOrClose(char * send_buffer, char * receive_buffer, Connection * connection) {
    receive_buffer[0] = '\0';
    //memset(receive_buffer, 0, shared.receive_buffer_size);
    if(send(connection->socket, send_buffer, shared.send_buffer_size, 0) < 0) {
        printRed("Failed to send message to %s. Disconnecting.\n", inet_ntoa(connection->address.sin_addr));
        perror("Error:");
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    int received_size = recv(connection->socket, receive_buffer, shared.receive_buffer_size, 0);
    if(received_size < 0) {
        printRed("Failed to receive message from %s. Disconnecting.\n", inet_ntoa(connection->address.sin_addr));
        perror("Error: ");
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    if(received_size == 0 ) {
        printBlue("%s disconnected.\n", inet_ntoa(connection->address.sin_addr));
        connection->status = ConnectionStatus_CLOSING;
        return 0;
    }
    send_buffer[0] = '\0';
    // memset(send_buffer, 0, shared.send_buffer_size);
    return received_size;
}



void MessageAndClose(char * send_buffer, Connection * connection) {
    strcat(send_buffer, "<Disconnect>");
    send(connection->socket, send_buffer, shared.send_buffer_size, 0);
    connection->status = ConnectionStatus_CLOSING;
    if(connection -> user != NULL) {
        connection->user->connected = 0;
    }
}

void _help(Connection* connection, char* response) {
    if(connection->state != ClientState_REGISTERED) {
        strcpy(response, "<Message>help - get a list of available commands\n");
        strcat(response, "register - register your user\n");
        strcat(response, "exit - disconnect from the server");
    } else if(connection->state == ClientState_REGISTERED) {
        strcpy(response, "<Message>h lp- get a list of available commands\n");
        strcat(response, "exit - disconnect from the server\n");
        strcat(response, "who - get a list of online users.\n");
        strcat(response, "random-gpa - set your gpa to a new random value.\n");
        strcat(response, "random-age - set your age to aa new random value.\n");
        strcat(response, "advertisement - get a colorful advertisement\n");
        strcat(response, "myinfo - get info about yourself");
    }
}

int _register(Connection * connection, char* response) {
    
    if(connection->user->registered) {
        strcpy(response, "<Error>");
        strcat(response, connection->user->id);
        strcat(response, " is already registered.");

        LogfError("%s from ip %s has attempted to register a second time.\n", connection->user->id, inet_ntoa(connection->address.sin_addr));
        return 0;
    }

    pthread_mutex_lock(&(shared.mutex));
    
    connection->user->registered = 1;
    shared.dirty = 1;

    connection->user->age = RandomInteger(18, 22);

    if(RandomFlag(.4)) {
        connection->user->gpa = 4.0;
    } else {
        connection->user->gpa = RandomFloat(2.5, 4);
    }

    connection->state = ClientState_REGISTERED;

    LogfDebug("%s has been registered.\n", connection->user->id);

    pthread_mutex_unlock(&(shared.mutex));

    strcpy(response, "<Message>You Have been registered ");
    strcat(response, connection->user->name);

    return 1;
}

int _myinfo(Connection* connection, char* response) {
    InitializeLogger(stdout, 0, 0, 0);
    
    if (!(connection->user->registered)) {
        strcpy(response, "<Error>");
        strcat(response, connection->user->id);
        strcat(response, " is not registered.");

        LogfError("%s from ip %s has attempted to view their information as an unregistered user.\n", connection->user->id, inet_ntoa(connection->address.sin_addr));

        return 1;
    }

    //Referenced snprintf from https://cplusplus.com/reference/cstdio/snprintf/
    //This line is way too long, but snprintf starts at the beginning of the buffer each time it is called. Maybe something else is needed.
    snprintf(response, shared.send_buffer_size, "<User.Name>%s<User.Age>%d<User.GPA>%.2f<User.IP>%s", connection->user->name, connection->user->age, connection->user->gpa, inet_ntoa(connection->address.sin_addr));

    LogfInfo("%s viewed their information.\n", connection->user->id);

    return 0;
}

void _who(char * response) {
    int i;
    for(i = 0; i < RECORD_COUNT; i++) {
        map_result result = Map_Get(shared.users, accepted_userIDs[i]);
        if(result.found) {
            User* user = (User *) result.data;

            if(user->connected) {
                strcat(response, "<OnlineUser>");
                strcat(response, user->id);
            }
        }
    }
}

void _rand_gpa(Connection* connection, char* response) {
    char gpa_str[5];
    pthread_mutex_lock(&(shared.mutex));
    if(RandomFlag(.4)) {
        connection->user->gpa = 4.0;
    } else {
        connection->user->gpa = RandomFloat(2.2, 4.0);
    }
    shared.dirty = 1;
    pthread_mutex_unlock(&(shared.mutex));
    sprintf(gpa_str, "%.2f", connection->user->gpa);
    strcat(response, "<User.GPA>");
    strcat(response, gpa_str);
}

void _rand_age(Connection* connection, char * response) {
    char age_str[5];
    pthread_mutex_lock(&(shared.mutex));
    connection->user->age = RandomInteger(18, 22);
    shared.dirty = 1;
    pthread_mutex_unlock(&(shared.mutex));

    sprintf(age_str, "%d", connection->user->age);
    strcat(response, "<User.AGE>");
    strcat(response, age_str);
}

/**
 * @}
*/
