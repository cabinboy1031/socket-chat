#pragma once
#include <stdio.h>
#include "./socket.h"
#include "./client.h"

typedef struct server_s {
  char message_log[2048][10];
  connection_s data; // master connection
  client_s client[20]; // connected clients
  size_t client_s; // size of client array
  fd_set read_fd; // set of fds that clients are connected to
} server_s;

server_s server_new(int port);
void server_close(server_s* server, int id);
void server_stop(server_s server);

void server_listen(server_s* server);

char* server_read(server_s* server);
void server_send(server_s* server, serialized_s message);
