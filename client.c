#include "client.h"

client_s client_connect(const char* url, int port){
  printf("Beginning connection to server...\n");
  connection_s conn = client_socket_connect(url, 1234);
  printf("done.\n");

  printf("Synchronizing settings with the server...\n");
  char buffer[1024];
  read(conn.sock, buffer, 1024);
  printf("%s\n", buffer);
  settings_s server_s = settings_deserialize(buffer);
  server_s.address = conn.settings.address;

  conn.settings = server_s;

  client_s client = {.connection = conn};
  printf("done.\n");

  settings_printf(client.connection.settings);
  printf("done.\n\n");

  return client;
}

void client_send(client_s* client, serialized_s message){
  send(client->connection.sock, message.data, message.size, 0);
}

const char* client_read(client_s* client){
  char buffer[client->connection.settings.char_limit];
  memset(buffer, 0, client->connection.settings.char_limit);

  read(client->connection.sock, buffer,
       client->connection.settings.char_limit);
  char *result;
  asprintf(&result, "%s", buffer);

  return result;
}
