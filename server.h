#pragma once
#include <stdio.h>
#include "./socket.h"

typedef struct client_s{
  connection_s connection;
  //TODO
  // room_s* rooms; // used to limit bandwidth usage by choosing where to send messages
  // size_t room_size;
  // int user_id; // used to prevent spoofing
} client_s;

typedef struct server_s {
  char message_log[2048][10];
  connection_s data;
  client_s client[20];
  fd_set read_fd;
  size_t connection_s;
} server_s;

server_s server_new(int port);
void server_close(server_s* server, int id);
void server_stop(server_s server);

void server_listen(server_s* server);

char* server_read(server_s* server);
void server_send(server_s* server, const char* message);
