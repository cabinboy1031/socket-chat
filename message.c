#include "message.h"

#include "mpack/mpack.h"
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

/**
   As the pattern stays the same throughout, use this for deserializing json data.
   sadly im not smart enough to completely abstract this away, so you will still have to decide on the type while writing the expression.
   Luckily getting a specific object type follows the same pattern as well:
   json_object_get_<type>(obj)
 */
#define get_json_data(data, token, expression)         \
  {                                               \
  json_object *obj;                               \
  json_object_object_get_ex(data, token, &obj);   \
  expression;                                     \
  }

#define set_json_data(object, token, expression, data)\
  {                                              \
    json_object *obj = expression(data);         \
    json_object_object_add(object,token,obj);    \
  }

message_s message_json_deserialize(const char* message){
  json_tokener *json = json_tokener_new();
  json_object *message_data = json_tokener_parse(message);

  message_s result;
  get_json_data(message_data, "data",
           strcpy(result.data, json_object_get_string(obj)));

  get_json_data(message_data, "username",
           strcpy(result.username, json_object_get_string(obj)));

  get_json_data(message_data, "datetime",
           result.datetime = (time_t) json_object_get_uint64(obj));

  json_tokener_free(json);
  return result;
}

const char* message_json_serialize(message_s message){
  json_object *object = json_object_new_object();

  set_json_data(object, "data", json_object_new_string, message.data);
  set_json_data(object, "username", json_object_new_string, message.username);
  set_json_data(object, "datetime", json_object_new_uint64, message.datetime);

  return json_object_to_json_string(object);
}

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
  strcpy(result.data,mpack_node_str(mpack_node_map_cstr(root, "data")));
  strcpy(result.username , mpack_node_str(mpack_node_map_cstr(root, "username")));
  result.datetime = mpack_node_uint(mpack_node_map_cstr(root, "datetime"));

  return result;
}

const char* message_mpack_serialize(message_s message){
  char *data;
  size_t size;
  mpack_writer_t writer;
  mpack_writer_init_growable(&writer, &data, &size);

  set_mpack_data(writer, "data", mpack_write_cstr, message.data);
  set_mpack_data(writer, "username", mpack_write_cstr, message.username);
  set_mpack_data(writer, "datetime", mpack_write_uint, message.datetime);
  mpack_complete_map(&writer);

  if(mpack_writer_destroy(&writer) != mpack_ok){
    fprintf(stdout, "an error occurred encoding the data! \n");
    return "";
  }
  return data;
}
