#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define warn(msg, ...)                                                         \
  printf("\x1b[38;2;240;190;100m"                                              \
         "\u26A0  | " msg "\x1b[0m\n",                                         \
         ##__VA_ARGS__);

#define err(msg, ...)                                                          \
  printf("\x1B[33m"                                                            \
         "\x1b[38;2;255;100;100m"                                              \
         "\u2757 | " msg "\x1B[0m\n",                                          \
         ##__VA_ARGS__)

#define ok(msg, ...)                                                           \
  printf("\x1B[92m"                                                            \
         "\u2705 | " msg "\x1B[0m\n",                                          \
         ##__VA_ARGS__)

#define PORT 3000
#define ADDRESS "127.0.0.1"

typedef int Handle;
typedef struct sockaddr_in Address;

// address: %s, port: %d\n", inet_ntoa(serv_address.sin_addr),
// ntohs(serv_address.sin_port));

int main() {
  Handle socket_handle = socket(AF_INET, SOCK_STREAM, 0);
  Address serv_address = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr(ADDRESS),
      .sin_port = htons(PORT),
  };
  if (socket_handle < 0) {
    err("socket creation failed !");
    exit(EXIT_FAILURE);
  }
  ok("socket successfully created !");
  if (bind(socket_handle, (struct sockaddr *)&serv_address,
           sizeof(serv_address)) == -1) {
    err("binding socket to %s server failed ! errno %d", ADDRESS, errno);
    exit(EXIT_FAILURE);
  }
  if (listen(socket_handle, 100) == -1) {
    err("listening to socket failed !");
    exit(EXIT_FAILURE);
  }
  int clsocket_handle = accept(socket_handle, 0, 0);
  if (clsocket_handle < 0) {
    err("could not accept client socket ! errno %d", errno);
    exit(EXIT_FAILURE);
  }
  ok("successfully connected to %s !", ADDRESS);
  struct pollfd fds[2] = {
      {0, POLLIN, 0},
      {socket_handle, POLLIN, 0},
  };
  for (;;) {
    char buffer[512] = {0};
    if (poll(fds, 2, 10000) == -1) {
      err("poll file descriptor error! %d", errno);
      exit(EXIT_FAILURE);
    }
    if (fds[0].revents & POLLIN) {
      if (read(0, buffer, 512) < 0) {
        err("could not read into the array buffer ! %d", errno);
        exit(EXIT_FAILURE);
      }
      if (send(clsocket_handle, buffer, 512, 0) < 0) {
        err("could not the buffersend to the socket %d", errno);
        exit(EXIT_FAILURE);
      }
    } else if (fds[1].revents & POLLIN) {
      if (recv(clsocket_handle, buffer, 255, 0) == 0) {
        return 0;
      }
      printf("%s\n", buffer);
    }
  }
}
