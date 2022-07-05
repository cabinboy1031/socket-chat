#include "./server.h"

server_s server_new(int port) {
  settings_s settings;
  settings.char_limit = 1024;

  server_s server;
  server.data = server_socket_setup(1234, settings);
  server.connection_s = 0;
  printf("Server is online.\n");

  return server;
}

void server_stop(server_s server){
  close(server.data.sock);
  shutdown(server.data.fd, SHUT_RDWR);
}

void server_listen(server_s *server) {
  printf("Waiting for connection...\n");
  settings_s settings;
  settings.char_limit = 1024;

  server->connection[server->connection_s] = server_socket_connect(server->data, settings);
  server->connection_s++;
  printf("done.\n");
}

char* server_read(server_s* server){
  char buffer[1024] = {0};
  read(server->connection[0].sock, buffer, 1024);
  char* result;
  asprintf(&result, "%s", buffer);

  return result;
}

void server_send(server_s* server, char* message){
  for(int i = 0; i < server->connection_s; i++){
    send(server->connection[i].sock, message, strlen(message), 0);
  }
  printf("Message sent!\n");
}
