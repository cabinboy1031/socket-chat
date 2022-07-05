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
  }

#define set_data(object, token, expression, data)\
  {                                              \
    json_object *obj = expression(data);         \
    json_object_object_add(object,token,obj);    \
  }

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
  json_object *object = json_object_new_object();

  set_data(object, "data", json_object_new_string, message.data);
  set_data(object, "username", json_object_new_string, message.username);
  set_data(object, "datetime", json_object_new_uint64, message.datetime);

  return json_object_to_json_string(object);
}
