#pragma once
#include "mpack/mpack.h"

typedef struct serialized_s {
  char *data;
  size_t size;
} serialized_s;

#include "message.h"
#include "socket.h"
