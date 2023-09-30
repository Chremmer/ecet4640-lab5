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

#define RCVBUFSIZE 1024   /* Size of receive buffer */
#define SNDBUFSIZE 1024   /* Size of receive buffer */
#define PORT  4400
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

    servSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

	// in_family is *always* AF_INET
    serv_addr.sin_family = AF_INET;
	// The address seems to be the IP address, converted to its underlying numeric value. 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT); 

    bind(servSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(servSock, 10); 

	
    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clnt_addr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &clnt_addr, 
                               &clntLen)) < 0)
            printf("accept() failed\n");

        /* clntSock is connected to a client! */

        printf("Handling client from %s\n", inet_ntoa(clnt_addr.sin_addr));

		strcpy(sendBuff, "Welcome to Copy Cat. Send me a string WITHOUT spaces, I will echo you back\n");
		
		do 
		{
			/* Echo message back to client */
			if (send(clntSock, sendBuff, sizeof(sendBuff), 0) < 0)
				printf("send() failed\n");

			/* See if there is more data to receive */
			memset(&sendBuff, '0', sizeof(sendBuff)); 	
			if ((recvMsgSize = recv(clntSock, recvBuff, RCVBUFSIZE, 0)) < 0)
				printf("recv() failed \n");
			strcpy(sendBuff, recvBuff);
			if (strcmp(strupr(sendBuff), "QUIT")==0)
			{
				printf("Received \"%s\", Disconnet.\n", sendBuff);
				recvMsgSize=0;
			}
			else
				printf("Received \"%s\", send it back\n", sendBuff);
			
		} while (recvMsgSize > 0);      /* zero indicates end of transmission */
		close(clntSock);
	}
 

 }

