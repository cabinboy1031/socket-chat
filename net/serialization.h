#pragma once
#include "../mpack/mpack.h"

typedef struct serialized_s {
  char *data;
  size_t size;
} serialized_s;

#define set_mpack_data(writer, token, expression, data)                        \
  {                                                                            \
    mpack_write_cstr(&writer, token);                                          \
    expression(&writer, data);                                                 \
  }

#include "message.h"
#include "socket.h"
