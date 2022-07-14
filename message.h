#include <time.h>

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
const char *message_mpack_serialize(message_s message);
