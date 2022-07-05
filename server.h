#pragma once
#include <stdio.h>
#include "./socket.h"


typedef struct server_s {
  char message_log[1024][10];
  connection_s data;
  connection_s connection[20];
  size_t connection_s;
} server_s;

server_s server_new(int port);
void server_stop(server_s server);

void server_listen(server_s* server);

char* server_read(server_s* server);
void server_send(server_s* server, char* message);
