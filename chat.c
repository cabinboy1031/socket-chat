#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>

#include "./socket.h"
#include "./server.h"
#include "./message.h"

#define ARGV_RUN(run_str, run_func) if (!strcmp(argv[1], run_str)) { \
    run_func;                                                       \
  };

void client_run() {
  time_t timer;
  message_s message = {
    .data = "Hello from client!\n",
    .username = "mystic",
    .datetime = time(&timer)
  };
  const char *message_j = message_json_serialize(message);

  settings_s settings;
  settings.char_limit = 2048;
  char *url = "127.0.0.1";
  connection_s client = client_socket_connect(url, 1234, settings);

  send(client.sock, message_j, strlen(message_j), 0);
  printf("Message sent!\n");

  char buffer[2048] = {0};
  read(client.sock, buffer, 2048);
  message_s message_c = message_json_deserialize(buffer);
  printf("[%d:%d] %s: %s\n",
         localtime(&message_c.datetime)->tm_hour,
         localtime(&message_c.datetime)->tm_min,
         message_c.username,
         message_c.data);

  close(client.fd);
}

void server_run() {
  time_t timer;
  message_s message = {
    .data = "Hello from server!",
    .username = "server",
    .datetime = time(&timer)
  };
  const char *message_j = message_json_serialize(message);

  server_s server = server_new(1234);
  server_listen(&server);

  char* buffer = server_read(&server);
  message_s message_c = message_json_deserialize(buffer);
  printf("[%d:%d] %s: %s\n", localtime(&message_c.datetime)->tm_hour,
         localtime(&message_c.datetime)->tm_min, message_c.username,
         message_c.data);
  server_send(&server, message_j);

  server_stop(server);
}

int main(int argc, char* argv[]){
  /*
  WINDOW *mainwin = initscr();

  initscr(); cbreak(); noecho();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  mvaddstr(13,33, "Hello, World!");
  refresh();
  sleep(3);

  delwin(mainwin);
  endwin();
  refresh();
  */

  if(argc != 2){
    printf("Usage: ./chat [client | server]\n");
    return -1;
  }
  ARGV_RUN("client", client_run());
  ARGV_RUN("server", server_run());
}

