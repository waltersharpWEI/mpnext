#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
 
#define BUFFSIZE 32
void Die(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  int received = 0;
 
  if (argc != 4) {
    fprintf(stderr, "USAGE: TCPecho <server_ip> <word> <port>\n");
    exit(1);
  }
  /* Create the TCP socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(atoi(argv[3]));       /* server port */
  /* Establish connection */
  if (connect(sock,
              (struct sockaddr *) &echoserver,
              sizeof(echoserver)) < 0) {
    Die("Failed to connect with server");
  } 

  /* Send the word to the server */

  const int FILE_NAME_MAX_SIZE=25;

  char file_name[FILE_NAME_MAX_SIZE+1];
  bzero(file_name, FILE_NAME_MAX_SIZE+1);

  strncpy(buffer, argv[2] , strlen(argv[2]));
  strncpy(file_name, argv[2] , strlen(argv[2]));

  if(send(sock, buffer, strlen(argv[2]), 0) != strlen(argv[2]))
  {
      Die("Send File Name Failed:");
  }

  FILE *fp = fopen(file_name, "r");
  if (NULL == fp)
  {
    Die("File not found\n");
  }
  else 
  {
    bzero(buffer, BUFFER_SIZE);
    int length = 0;
    while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
    {
      if (send(sock, buffer, length, 0) < 0)
      {
        Die("Send File Failed.\n");
      }
      bzero(buffer, BUFFER_SIZE);
    }
  }
/*
  echolen = strlen(argv[2]);
  if (send(sock, argv[2], echolen, 0) != echolen) {
    Die("Mismatch in number of sent bytes");
  }
*/
  fprintf(stdout, "\n");
  close(sock);
  exit(0);
}
