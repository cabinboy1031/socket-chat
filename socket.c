#include "socket.h"

connection_s client_socket_connect(const char* url, const int port, settings_s settings){
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  inet_pton(sock, url, &serv_addr.sin_addr);

  int client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  return (connection_s){ .fd = client_fd, .sock = sock, .settings = settings };
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

