#include "message.h"

#include "serialization.h"

#include <stdio.h>
#include <string.h>

#define set_mpack_data(writer, token, expression, data)                        \
  {                                                                            \
    mpack_write_cstr(&writer, token);                                          \
    expression(&writer, data);                                                 \
  }

message_s message_mpack_deserialize(const char* data){
  mpack_tree_t tree;
  mpack_tree_init_data(&tree, data, strlen(data));
  mpack_tree_parse(&tree);
  mpack_node_t root = mpack_tree_root(&tree);

  message_s result;
  mpack_node_copy_cstr(mpack_node_map_cstr(root,"data"), result.data, sizeof(result.data));
  mpack_node_copy_cstr(mpack_node_map_cstr(root,"username"), result.username, 20);
  result.datetime = mpack_node_uint(mpack_node_map_cstr(root, "datetime"));

  return result;
}

serialized_s message_mpack_serialize(message_s message){
  char *data;
  size_t size;
  mpack_writer_t writer;
  mpack_writer_init_growable(&writer, &data, &size);

  mpack_start_map(&writer, 3);
  set_mpack_data(writer, "data", mpack_write_cstr, message.data);
  set_mpack_data(writer, "username", mpack_write_cstr, message.username);
  set_mpack_data(writer, "datetime", mpack_write_uint, message.datetime);
  mpack_finish_map(&writer);

  if(mpack_writer_destroy(&writer) != mpack_ok){
    fprintf(stdout, "an error occurred encoding the data! \n");
    return (serialized_s){.data = "", .size = 0};
  }

  return (serialized_s) {.data = data, .size = size};
}
