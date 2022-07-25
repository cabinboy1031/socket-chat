#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "serialization.h"
/**
   General connection settings.
   Given the nature of these settings, they MUST be modified
   and set by the server before connections begin.
   They SHALL NOT be modified by the client.

   Items marked with [SERVER] do not need to be sent over the network for the client to know.
   However, this may be useful information to the client.

   char_limit: the maximum byte limit for messages.
   max_clients [SERVER]: the maximum number of clients that one server can handle at a time.
   max_connections [SERVER]: the maximum number of connections that a server should try and resolve
 */
typedef struct settings_s {
  char server_name[32];
  int char_limit;
  int max_clients;
  int max_connections;
  struct sockaddr_in address;
} settings_s;

settings_s settings_deserialize(const char* data);
serialized_s settings_serialize(settings_s settings);
void settings_printf(settings_s data);

typedef struct connection_s {
  int fd;
  int sock;
  settings_s settings;
} connection_s;

/**
 * Creates a client connection to the specified URL.
 */
connection_s client_socket_connect(const char* url,const int port);

connection_s server_socket_setup(int port, settings_s settings);
connection_s server_socket_connect(connection_s setup, settings_s settings);

void disconnect(connection_s conn);
