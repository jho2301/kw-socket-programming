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
  int server_socket;
  int client_socket;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  socklen_t client_address_size;

  if (argc < 2)
    error("please enter port");

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
    error("socket error occured!");

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    error("bind error occured!");

  if (listen(server_socket, 4) == -1)
    error("listen error occured!");

  client_address_size = sizeof(client_address);
  client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
  if (client_socket == -1)
    error("accept error occured!");

  char msg[] = "you are successfully connected!!\n";
  send(client_socket, msg, sizeof(msg), 0);
  write(1, "successfully accecpted\n", 23);

  while (1)
  {

    char buffer[BUFSIZE];
    ssize_t byte_size_received = recv(client_socket, buffer, BUFSIZE, 0);
    if (byte_size_received == -1)
      error("recv error occured!");
    else if (byte_size_received == 0)
      error("client connection closed");
    buffer[byte_size_received] = 0;

    if (strncmp(buffer, "quit", 4) == 0)
      break;
    // char server_buffer[BUFSIZE];
    // server_buffer[0] = 0;
    // strcat(server_buffer, "client(");
    // strcat(server_buffer, inet_ntoa(client_address.sin_addr));
    // strcat(server_buffer, "): ");
    // strcat(server_buffer, buffer);
    // ssize_t bytes_size_sent = send(client_socket, server_buffer, strlen(server_buffer), 0);

    printf("client(%s): %s\n", inet_ntoa(client_address.sin_addr), buffer);

    buffer[0] = 0;
    write(STDOUT_FILENO, "input: ", 7);
    ssize_t byte_size_read = read(STDIN_FILENO, buffer, BUFSIZE);
    buffer[byte_size_read] = 0;

    ssize_t bytes_size_sent = send(client_socket, buffer, strlen(buffer), 0);
    if (bytes_size_sent == -1)
      error("send error occured");
  }

  close(client_socket);
  close(server_socket);

  return 0;
}
