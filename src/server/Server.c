/**
 * \addtogroup Server
 * @{
*/
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/net.h>
#include <stdio.h>

#include "Server.h"
#include "Connection.h"
#include "Util.h"
#include "map.h"

ServerProperties server;
Connection * connections;
short server_running;

// A private function just for reading the settings map into the server struct and printing warnings as necessary.
void _readSettingsMapIntoServerStruct(map * server_settings) {
    map_result result = Map_Get(server_settings, "port");
    if(!result.found) {
        printYellow("No port setting found. Defaulting to 3000.\n");
        server.port = 3000;
    } else {
        int found_port = atoi(result.data);
        if(found_port <= 0) {
            printYellow("Invalid port setting: %s. Defaulting to 3000.\n", result.data);
            server.port = 3000;
        } else {
            server.port = htons(found_port);
        }
    }
    result = Map_Get(server_settings, "send_buffer_size");
    if(!result.found) {
        printYellow("No send_buffer_size setting found. Defaulting to 1024.\n");
        server.send_buffer_size = 1024;
    } else {
        int found_sb_size = atoi(result.data);
        if(found_sb_size <= 0) {
            printYellow("Invalid send_buffer_size setting: %s. Defaulting to 1024.\n", result.data);
            server.send_buffer_size = 1024;
        } else {
            server.send_buffer_size = found_sb_size;
        }
    }
    result = Map_Get(server_settings, "receive_buffer_size");
    if(!result.found) {
        printYellow("No receive_buffer_size setting found. Defaulting to 1024.\n");
        server.send_buffer_size = 1024;
    } else {
        int found_rb_size = atoi(result.data);
        if(found_rb_size <= 0) {
            printYellow("Invalid receive_buffer_size setting: %s. Defaulting to 1024.\n", result.data);
            server.receive_buffer_size = 1024;
        } else {
            server.receive_buffer_size = found_rb_size;
        }
    }
    result = Map_Get(server_settings, "backlog");
    if(!result.found) {
        printYellow("No backlog setting found. Defaulting to 10.\n");
        server.backlog = 10;
    } else {
        int found_backlog = atoi(result.data);
        if(found_backlog <= 0) {
            printYellow("Invalid backlog setting: %s. Defaulting to 10.\n", result.data);
            server.backlog = 10;
        } else {
            server.backlog = found_backlog;
        }
    }
    result = Map_Get(server_settings, "max_connections");
    if(!result.found) {
        printYellow("No max_connections setting found. Defaulting to 20.\n");
        server.max_connections = 20;
    } else {
        int found_max_connections = atoi(result.data);
        if(found_max_connections <= 0) {
            printYellow("Invalid max_connections setting: %s. Defaulting to 20.\n", result.data);
            server.max_connections = 20;
        } else {
            server.max_connections = found_max_connections;
        }
    }
};

int InitializeServer(map * server_settings) {
    _readSettingsMapIntoServerStruct(server_settings);
    connections = malloc(server.max_connections * sizeof(Connection));
    int i;
    for(i=0; i < server.max_connections; i++) {
        connections[i].active = 0;
    }
    server_running = 0;
    printGreen("Server initialized with %d max connections.\n", server.max_connections);
    return 1;
}

int StartServer(map * users_map) {
    int serverSocket = 0;
    struct sockaddr_in server_address;
    // Record the time the server started.
    time(&server.time_started);
    // Get a socket file pointer associated with ipv4 internet protocols that represents a two-way connection based byte stream.
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    // Set the address to bind to all available interfaces.
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // Set the port.
    server_address.sin_port = server.port;
    // Assign a name to the socket.
    int bind_error = bind(serverSocket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(bind_error) {
        printRed("Error binding the server to port %u.\n",server.port);
        perror("Bind Error:");
        return 0;
    }
    server_running = 1;
    while(server_running) {
        // check that we have an open connection
        // if we do, accept a connection
        // for that connection, load properties into Connection
        // start a thread for Connection
        // continue
    }

    return 1;
}



/**
 * @}
*/