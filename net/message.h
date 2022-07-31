#pragma once
#include <time.h>
#include "serialization.h"

typedef struct message_s {
  char data[1024];
  char username[20];
  time_t datetime;
}message_s;

/**
   deserialize MessagePack data to message structure
 */
message_s message_mpack_deserialize(const char* message);

/**
   serialize message structure to MessagePack protocol
 */
serialized_s message_mpack_serialize(message_s message);

char* message_printf(message_s message);
