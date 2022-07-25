#include "socket.h"
#include "serialization.h"

#define set_mpack_data(writer, token, expression, data)                        \
  {                                                                            \
    mpack_write_cstr(&writer, token);                                          \
    expression(&writer, data);                                                 \
  }

settings_s settings_deserialize(const char *data){
  mpack_tree_t tree;
  mpack_tree_init_data(&tree, data, strlen(data));
  mpack_tree_parse(&tree);
  mpack_node_t root = mpack_tree_root(&tree);

  settings_s settings;
  settings.char_limit = mpack_node_u64(mpack_node_map_cstr(root, "byte_limit"));
  settings.max_clients = mpack_node_u64(mpack_node_map_cstr(root, "max_clients"));
  settings.max_connections = mpack_node_u64(mpack_node_map_cstr(root, "max_connections"));
  mpack_node_copy_cstr(mpack_node_map_cstr(root, "name"), settings.server_name,
                       sizeof(settings.server_name));
  settings_printf(settings);

  return settings;
}

serialized_s settings_serialize(settings_s settings){
  char* data;
  size_t size;
  mpack_writer_t writer;
  mpack_writer_init_growable(&writer, &data, &size);

  mpack_start_map(&writer, 4);
  set_mpack_data(writer, "name", mpack_write_cstr, settings.server_name);
  set_mpack_data(writer, "byte_limit", mpack_write_uint, settings.char_limit);
  set_mpack_data(writer, "max_clients", mpack_write_uint, settings.max_clients);
  set_mpack_data(writer, "max_connections", mpack_write_uint, settings.max_connections);
  mpack_finish_map(&writer);

  if (mpack_writer_destroy(&writer) != mpack_ok) {
    fprintf(stdout, "an error occurred encoding the data! \n");
    return (serialized_s) {.data = "", .size = 0};
  }

  return (serialized_s){.data = data, .size = size};
}

void settings_printf(settings_s data){
  printf("Server name: %s\n"
         "\tMax Clients: %d\n"
         "\tMax Connections: %d\n"
         "\tByte Limit: %d\n",
         data.server_name, data.max_clients, data.max_connections,
         data.char_limit);
}

connection_s client_socket_connect(const char* url, const int port){
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  inet_pton(sock, url, &serv_addr.sin_addr);

  printf("Attempting to connect...\n");
  int client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  return (connection_s){
    .fd = client_fd,
    .sock = sock,
    .settings = {.address = serv_addr}
  };
}

connection_s server_socket_setup(int port, settings_s settings){
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in address;
  int opt = 1;

  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
             sizeof(opt));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(1234);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));


  return (connection_s){server_fd, 0, settings};
}

connection_s server_socket_connect(connection_s setup, settings_s settings){
  listen(setup.fd, 3);

  int addrlen = sizeof(settings.address);
  int new_socket = accept(setup.fd, (struct sockaddr*)&settings.address, (socklen_t*)&addrlen);

  connection_s result = {setup.fd, new_socket, setup.settings};
  return result;
}

void disconnect(connection_s conn){
  close(conn.fd);
}

void server_teardown(){
  
}

