#include "../Headers/SocketTools.h"

int sockfd;

//1 on bad 0 on good

//0 on bad 1 on good
int openSocket()
{
    int portno;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;

    hostname = "10.39.28.2";
    portno = 1130;

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
#if VERBOSITY >= 1
        printf("Could not open socket!\n");
#endif
	return 0;
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL)
    {
#if VERBOSITY >= 1
        printf("ERROR, no such host as %s\n", hostname);
#endif
 	return 0;
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
    {
#if VERBOSITY >=1
      printf("ERROR: Error connecting\n");
#endif
      return 0;
    }
    return 1;
}

int writeToSocket(char* data)
{
    /* send the message line to the server */
    int n = write(sockfd, data, strlen(data) + 1);
    if (n < 0)
    {
      return 0;
    }
#if VERBOSITY >=2
    printf("Sent %d bytes\n", n);
#endif
    return 1;
}
