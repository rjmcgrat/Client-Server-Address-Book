// Rodney McGrath 55352053
// Ashton Garcia 86229480
// ICS 53 Winter 2018
// Lab 5: Client/Server Address Book
// Lab5_client.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <csapp.h>

int open_clientfd(char *hostname, int port) {
  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1; /* check errno for cause of error */

  /* Fill in the server's IP address and port */
  if ((hp = gethostbyname(hostname)) == NULL)
    return -2; /* check h_errno for cause of error */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr_list[0],
        (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(port);

  /* Establish a connection with the server */
  if (connect(clientfd, (struct sockaddr *) &serveraddr,
      sizeof(serveraddr)) < 0)
    return -1;
  return clientfd;
}

void package(char *content, char *packaging) {
	unsigned char size = strlen(content)-1;
	packaging[0] = size;
	for (int i = 0; i < size; i++) {
		packaging[i+1] = content[i];
	}
}

void unpackage(char *packaging, char *content) {
	for (int i = 1; i < strlen(packaging); i++) {
		content[i-1] = packaging[i];
	}
}

int main(int argc, char *argv[]){
  int clientfd, port;
  char *host, buf[256];
  char *buffer[256];
  char *input[256];
  char *c[256];
  int n;
  host = argv[1];
  port = atoi(argv[2]);
  clientfd = open_clientfd(host, port);
  while(1){
    fflush(stdout);
    fflush(stdin);
    printf(">");
    bzero(buffer, 256);
    bzero(buf, 256);
    bzero(c, 256);
    fgets(buf, 256, stdin);
    strncpy(c, buf, 3);
    if (strcmp(c, "+++") == 0){
      break;
    }

    fputs(buf, stdout);
    fflush(stdout);
  }
  close(clientfd);
  return 0;
}
