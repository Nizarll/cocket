# Cocket

cocket is a c socket training project.
includes error handling for each of the socket event.
to establish a socket buffer between a client and a server (or between two processes) we instanciate a socket on our client process bind it to an ip and a port.
the client socket sends a handshake request to the server and the server process accepts it.
## Example

[client.c](https://)
```c
#define PORT 3000
#define ADDRESS "127.0.0.1"

typedef int Handle;
typedef struct sockaddr_in Address;

int main() {
  Handle socket_handle = socket(AF_INET, SOCK_STREAM, 0);
  Address serv_address = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr(ADDRESS),
      .sin_port = htons(PORT),
  };
    exit(EXIT_FAILURE);
  }
  ok("socket successfully created !");
  if (connect(socket_handle, (struct sockaddr *)&serv_address,
              sizeof(serv_address)) < 0) {
    err("Connection to %s server failed !", ADDRESS);
    exit(EXIT_FAILURE);
  }
}
```
[server.c](https://)
```c
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
}
```
