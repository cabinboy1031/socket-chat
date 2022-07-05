#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

typedef struct settings_s {
  int char_limit;
  struct sockaddr_in address;
} settings_s;

typedef struct connection_s {
  int fd;
  int sock;
  settings_s settings;
} connection_s;

/**
 * Returns data representing the server connection
 */
connection_s client_socket_connect(const char* url,const int port, settings_s settings);

connection_s server_socket_setup(int port, settings_s settings);
connection_s server_socket_connect(connection_s setup, settings_s settings);

void disconnect(connection_s conn);
