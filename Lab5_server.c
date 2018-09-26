// Rodney McGrath 55352053
// Ashton Garcia 86229480
// ICS 53 Winter 2018
// Lab 5: Client/Server Address Book
// Lab5_server.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

int open_listenfd(int port)
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval , sizeof(int)) < 0)
        return -1;

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept
       connection requests */
    if (listen(listenfd, 10) < 0)
        return -1;

   return listenfd;
}

void package(char *payload, char *buffer){
  unsigned char size = strlen(payload);
  buffer[0] = size;
  int i;
  for (i = 0; i < size; i++){
    buffer[i + 1] = payload[i];
  }
}

void unpackage(char *buffer, char *payload){
  int i;
  for (i = 0; i < strlen(buffer) - 1; i++){
    payload[i] = buffer[i + 1];
  }
}

int main(int argc, char *argv[]) {
    int listenfd, connfd, port;
    int clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    unsigned short client_port;
    char buffer[256];
    char payload[256];
    port = atoi(argv[1]);
    listenfd = open_listenfd(port);
    printf("listenfd: %d\n", listenfd);
    while (1) {
        clientlen = sizeof(clientaddr);
        printf("clientlen: %d\n", clientlen);
        connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
        printf("a\n");
        printf("%d\n", connfd);
        printf("b\n");
        hp = gethostbyaddr((const char *) &clientaddr.sin_addr.s_addr,
          sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        client_port = ntohs(clientaddr.sin_port);
        printf("Address server started\n");
        printf("%zd\n", read(connfd, payload, 255));
        /*
        while(read(connfd, payload, 255) > 0)
        {
          fflush(stdout);
          fflush(stdin);
          bzero(payload, 256);
          bzero(buffer, 256);
          int i;
          unpackage(buffer, payload);
          printf("%s\n", payload);
          bzero(buffer, 256);
          if (strcmp(payload, "harris@ics.uci.edu") == 0)
          {
            package("Ian G. Harris", buffer);
          }
          else if (strcmp(payload, "joe@cnn.com") == 0)
          {
            package("Joe Smith", buffer);
          }
          else if (strcmp(payload, "jane@slashdot.org") == 0)
          {
            package("Jane Smith", buffer);
          }
          else if (strcmp(payload, "+++") == 0)
          {
            break;
          }
          else
          {
            package("unknown", buffer);
          }

      }
      */
    printf("closing connection\n");
    close(connfd);
  }
    return 0;
}
