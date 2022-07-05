#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "./socket.h"
#include "./server.h"

#define ARGV_RUN(run_str, run_func) if (!strcmp(argv[1], run_str)) { \
    run_func;                                                       \
  };

void client_run() {
  settings_s settings;
  settings.char_limit = 1024;

  char *url = "127.0.0.1";
  connection_s client = client_socket_connect(url, 1234, settings);

  char *hello = "Hello from client!\n";
  char buffer[1024] = {0};
  send(client.sock, hello, strlen(hello), 0);
  printf("Message sent!\n");

  read(client.sock, buffer, 1024);
  printf("%s\n", buffer);

  close(client.fd);
}

void server_run() {
  server_s server = server_new(1234);
  server_listen(&server);

  char* buffer = server_read(&server);
  printf("%s", buffer);
  server_send(&server, "Hello world from server!");

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

