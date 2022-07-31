#pragma once
#include "socket.h"

typedef struct client_s {
  connection_s connection;
  // TODO
  // room_s* rooms; // used to limit bandwidth usage by choosing where to send messages
  // size_t room_size;
  // int user_id; // used to prevent spoofing
} client_s;

client_s client_connect(const char* url, int port);
void client_send(client_s* client, serialized_s message);
const char* client_read(client_s* client);
