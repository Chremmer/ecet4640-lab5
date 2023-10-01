#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define RCVBUFSIZE 76   /* Size of receive buffer */
#define SNDBUFSIZE 76   /* Size of receive buffer */
#define PORT  3000
char *strupr(char * text)
{
	int i, j=strlen(text);
	
	for (i=0;i<j;i++)
	{
		if ((text[i]>=97)&&(text[i]<=122))
			text[i]=text[i]-32;
	}
	return text;
}

/*
	From the man pages: (https://www.man7.org/linux/man-pages/man7/ip.7.html)

	struct sockaddr_in {
               sa_family_t    sin_family; // address family: AF_INET 
               in_port_t      sin_port;   // port in network byte order 
               struct in_addr sin_addr;   // internet address 
           };

           // Internet address 
           struct in_addr {
               uint32_t       s_addr;     // address in network byte order 
           };
*/

int main(int argc, char *argv[])
{
    int servSock = 0, clntSock = 0;
    struct sockaddr_in serv_addr; 
    struct sockaddr_in clnt_addr; 
	int clntLen, recvMsgSize;
	
    int quit=0;

    char sendBuff[SNDBUFSIZE];
	char recvBuff[RCVBUFSIZE];
    time_t ticks; 

	// calling socket gets a filepointer which serves as an endpoint for communication. The first argument is domain, which is the communication domain. AF_INET means this socket will use ipv4 internet protocols (as opposed to, for example, bluetooth, appletalk, or various forms of local communication ). The second parameter, 'type', somewhat determines the layer. You could access close to raw datagrams with a different layer. SOCK_STREAM is a reliable, two-way, connection-based byte stream. The protocol defines the protocol in that family, if there are multiple protocols supported. Not all sockets have multiple protocols and, in that case, this is a 0.
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

	// in_family is *always* AF_INET
    serv_addr.sin_family = AF_INET;
	// INADDR_ANY binds the server to all available interfaces (not just localhost). If we wanted to just bind to localhost, we could run: my_sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1"); before calling bind.
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT); 
	// *what's 'htonl' and 'htons'? these ht functions seem to be "host to order network". They convert numbers from 'host' byte order to 'network' byte order. To convert back, we have 'ntohs'. There are hostlongs (l) and hostshorts(s) and netlongs (l) and netshorts (s)

	// 'assigning a name to a socket'. Sets errno on fail, otherwise 0. When a socket is first created, it exists in a name space / address family but has no address assigned to it until bind is called. We are basically merging the socket struct with the serv_addr struct here. 
    bind(servSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

	printf("Listening on port %u.\n", PORT);

	// marks socket as a passive socket, that will be used to accept incoming connections with accept. The second parameter is the backlog. Sets errno on fail.
	listen(servSock, 10);

	printf("127.0.0.1 = %i\n",inet_addr("127.0.0.1"));
	printf("INADDR_ANY = %s\n", inet_ntoa(serv_addr.sin_addr));

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clnt_addr);

        /* Wait for a client to connect */
		// accept is only used with connection-based sockets, which we are (SOCK_STREAM)
		// it extracts the first connection request on the queue of pending connections for the listening socket, creates a new connected socket, and returns a new file descriptor referring to that socket. The original socket (the server socket) is not affected by this call.
		// this is blocking if there are no pending connections and we are not marked as nonblocking.
		// you could also be notified of incoming connections with select, poll, or epoll, or by having the socket deliver a SIGIO signal.
		// accept4 lets us pass SOCK_NONBLOCK if desired. 
        if ((clntSock = accept(servSock, (struct sockaddr *) &clnt_addr, 
                               &clntLen)) < 0)
            printf("accept() failed\n");

        /* clntSock is connected to a client! */

        printf("Handling client from %s\n", 
		//inet_ntoa converts a byte address to the ipv4 numbers-and-dots notation string.
		inet_ntoa(clnt_addr.sin_addr));

		strcpy(sendBuff, "Welcome to Copy Cat. Send me a string WITHOUT spaces, I will echo you back\n");
		
		do 
		{
			/* Echo message back to client */
			// send is equivalent to write when flags are 0.
			// its like write because a socket is a file descriptor.
			// this can only be used when the socket is in a connected state.
			if (send(clntSock, sendBuff, sizeof(sendBuff), 0) < 0)
				printf("send() failed\n");

			/* See if there is more data to receive */
			memset(&sendBuff, '0', sizeof(sendBuff)); 	
			if ((recvMsgSize = recv(clntSock, recvBuff, RCVBUFSIZE, 0)) < 0)
				printf("recv() failed \n");
			strcpy(sendBuff, recvBuff);
			if (strcmp(strupr(sendBuff), "QUIT")==0)
			{
				printf("Received \"%s\", Disconnect.\n", sendBuff);
				recvMsgSize=0;
			}
			else
				printf("Received \"%s\", send it back\n", sendBuff);
			
		} while (recvMsgSize > 0);      /* zero indicates end of transmission */
		close(clntSock);
	}
 

 }

