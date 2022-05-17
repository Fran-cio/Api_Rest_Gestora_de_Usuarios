#include <stdio.h>
#include <ulfius.h>
#include <stdlib.h>
#include <string.h>

#include <jansson.h>

int acumulador;
/**
 * Callback function for the web application on /helloworld url call
 */
static int increment(const struct _u_request * request,
    struct _u_response * response, void * user_data) {
  (void) request;
  (void) user_data;

  json_t * json_body = NULL;

  json_int_t * acum = ((long long*)&acumulador);
  (*acum)++;

  long long status = 200;
  json_int_t *j_status = &status;
  
  json_body = json_object();
  json_object_set_new(json_body, "code", json_integer(*j_status));
  json_object_set_new(json_body, "description", json_integer(*acum));

  ulfius_add_header_to_response(response, ".json", "application/json");
  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);
  
  return U_CALLBACK_CONTINUE;
}

int value(const struct _u_request * request,
    struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;

  json_t * json_body = NULL;
  json_int_t * acum = ((long long*)&acumulador);

  long long status = 200;
  json_int_t *j_status = &status;
  
  json_body = json_object();
  json_object_set_new(json_body, "code", json_integer(*j_status));
  json_object_set_new(json_body, "description", json_integer(*acum));

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);
  
  return U_CALLBACK_CONTINUE;
}


int no_encontrado(const struct _u_request * request,
    struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;

  json_t * json_body = NULL;

  long long status = 404;
  json_int_t *j_status = &status;
  
  json_body = json_object();
  json_object_set_new(json_body, "code", json_integer(*j_status));
  json_object_set_new(json_body, "description", json_string("NOT FOUND"));

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);
  
  return U_CALLBACK_CONTINUE;
}


