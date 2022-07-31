#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>

#include "net/server.h"
#include "net/client.h"
#include "net/message.h"

#include "ui/interface.h"

#define ARGV_RUN(run_str, run_func) if (!strcmp(argv[1], run_str)) { \
    run_func;                                                       \
  };

void client_run() {
  char* url = "127.0.0.1";
  client_s client = client_connect(url, 1234);

  // construct message
  time_t timer;
  message_s message = {.username = "mystic", .datetime = time(&timer)};
  read(0, message.data, 1024);

  // send message through client
  serialized_s message_j = message_mpack_serialize(message);
  client_send(&client, message_j);
  printf("Message sent!\n");

  //recieve message from client

  const char* message_c = client_read(&client);
  message_s message_m = message_mpack_deserialize(message_c);
  printf("%s", message_printf(message_m));

  close(client.connection.fd);
}

void server_run() {
  message_s message_c = {};

  server_s server = server_new(1234);
  while(strcmp(message_c.data, "/quit")){
    server_listen(&server);

    char *buffer = server_read(&server);
    message_c = message_mpack_deserialize(buffer);
    printf("%s", message_printf(message_c));

    time_t timer;
    message_s message = {.username = "server",
                         .datetime = time(&timer)};
    strcpy(message.data, message_c.data);
    serialized_s message_j = message_mpack_serialize(message);

    server_send(&server, message_j);
    server_close(&server, 0);
  }

  server_stop(server);
}

  int main(int argc, char *argv[]) {

    interface_setup();
    interface_full_refresh();
    sleep(3);
    interface_teardown();

    /*
    if(argc == 1){
      printf("Usage: ./chat [c | s]\n");
      return -1;
    }

    ARGV_RUN("c", client_run());
    ARGV_RUN("s", server_run());
     */
  }
