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
  settings.char_limit = 2048;

  server->connection[server->connection_s] = server_socket_connect(server->data, settings);
  server->connection_s++;
  printf("done.\n");
}

char* server_read(server_s* server){
  char buffer[server->connection->settings.char_limit];
  memset(buffer, 0, server->connection->settings.char_limit);

  read(server->connection[0].sock, buffer, server->connection->settings.char_limit);
  char* result;
  asprintf(&result, "%s", buffer);

  return result;
}

void server_send(server_s* server,const char* message){
  for(int i = 0; i < server->connection_s; i++){
    send(server->connection[i].sock, message, strlen(message), 0);
  }
  printf("Message sent!\n");
}

void server_close(server_s* server, int id){
  close(server->connection[id].fd);

  for(int i = id; i < server->connection_s - 1; i++){
    server->connection[i] = server->connection[i + 1];
  }

  server->connection_s--;
}
