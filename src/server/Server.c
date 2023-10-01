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

ServerProperties server;
Connection * connections;
short server_running;

int InitializeServer() {
    server.send_buffer_size = 1024;
    server.receive_buffer_size = 1024;
    server.backlog = 10;
    server.max_connections = 20;
    server.active_connections = 0;
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
        perrror("Bind Error:");
        return 0;
    }
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