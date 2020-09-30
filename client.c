#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024

void error(char *msg)
{
  printf("%s\n", msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    error("enter server_address server_port");

  char *server_ip = argv[1];
  in_port_t server_port = atoi(argv[2]);
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(server_ip);
  server_address.sin_port = htons(server_port);

  int server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
    error("socket error occured");

  if (connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    error("connect error!");

  char buffer[BUFSIZE];
  ssize_t byte_size_received = recv(server_socket, buffer, BUFSIZE, 0);
  if (byte_size_received == -1)
    error("recv error!");
  printf("server: %s\n", buffer);
  while (1)
  {
    write(STDOUT_FILENO, "input: ", 7);
    char buffer[BUFSIZE];
    ssize_t byte_size_read = read(STDIN_FILENO, buffer, BUFSIZE);
    buffer[byte_size_read] = 0;

    ssize_t byte_size_sent = send(server_socket, buffer, strlen(buffer), 0);
    if (strncmp(buffer, "quit", 4) == 0)
      break;

    ssize_t byte_size_received = recv(server_socket, buffer, BUFSIZE, 0);
    if (byte_size_received == -1)
      error("recv error!");
    printf("server: %s\n", buffer);
  }
  close(server_socket);
  return (0);
}
