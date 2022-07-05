#include "message.h"

#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

/**
   As the pattern stays the same throughout, use this for deserializing json data.
   sadly im not smart enough to completely abstract this away, so you will still have to decide on the type while writing the expression.
   Luckily getting a specific object type follows the same pattern as well:
   json_object_get_<type>(obj)
 */
#define get_data(data, token, expression)         \
  {                                               \
  json_object *obj;                               \
  json_object_object_get_ex(data, token, &obj);   \
  expression;                                     \
  }                                               \

message_s message_json_deserialize(const char* message){
  json_tokener *json = json_tokener_new();
  json_object *message_data = json_tokener_parse(message);

  message_s result;
  get_data(message_data, "data",
           strcpy(result.data, json_object_get_string(obj)));

  get_data(message_data, "username",
           strcpy(result.username, json_object_get_string(obj)));

  get_data(message_data, "datetime",
           result.datetime = (time_t) json_object_get_uint64(obj));

  json_tokener_free(json);
  return result;
}

const char* message_json_serialize(message_s message){
  json_object *obj = json_object_new_object();

  json_object *obj_data = json_object_new_string(message.data);
  json_object_object_add(obj, "data", obj_data);

  json_object *obj_username = json_object_new_string(message.username);
  json_object_object_add(obj, "username", obj_username);

  json_object *obj_datetime = json_object_new_uint64(message.datetime);
  json_object_object_add(obj, "datetime", obj_datetime);

  return json_object_to_json_string(obj);
}
