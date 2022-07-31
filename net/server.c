#include "./server.h"

server_s server_new(int port) {
  server_s server;
  server.data = server_socket_setup(1234,
                                    (settings_s){
                                      .char_limit = 2048,
                                      .max_clients = 32,
                                      .max_connections = 4,
                                      .server_name = "A test server"
                                    });
  server.client_s = 0;
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
  server->client[server->client_s].connection = server_socket_connect(server->data, settings);

  printf("Synchronizing settings with client...\n");
  serialized_s settings_c = settings_serialize(server->data.settings);
  send(server->client[server->client_s].connection.sock, settings_c.data, settings_c.size, 0);

  server->client_s++;
  printf("done.\n");

}

char* server_read(server_s* server){
  char buffer[server->data.settings.char_limit];
  memset(buffer, 0, server->data.settings.char_limit);

  read(server->client[0].connection.sock, buffer, server->data.settings.char_limit);
  char* result;
  asprintf(&result, "%s", buffer);

  return result;
}

void server_send(server_s* server,serialized_s message){
  for(int i = 0; i < server->client_s; i++){
    send(server->client[i].connection.sock, message.data, message.size, 0);
  }

  printf("Message sent!\n");
}

void server_close(server_s* server, int id){
  close(server->client[id].connection.fd);

  for(int i = id; i < server->client_s - 1; i++){
    server->client[i] = server->client[i + 1];
  }

  server->client_s--;
}
