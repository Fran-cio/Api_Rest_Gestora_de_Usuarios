#include <stdio.h>
#include <ulfius.h>
#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#include "../ulfius/example_callbacks/static_compressed_inmemory_website/static_compressed_inmemory_website_callback.h"

#define PUERTO 6666

int acumulador;
/**
 * Callback function for the web application on /helloworld url call
 */
static int increment(const struct _u_request * request, struct _u_response * response, void * user_data) {
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

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);
  
  return U_CALLBACK_CONTINUE;
}

int value(const struct _u_request * request, struct _u_response * response, void * user_data) 
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


int no_encontrado(const struct _u_request * request, struct _u_response * response, void * user_data) 
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

int main()
{

  uint puerto = PUERTO; 

  struct _u_compressed_inmemory_website_config config;
  // Add mime types
  // u_map_put(&config.mime_types, ".json", "application/json");
  // specify compressed mime types
  // u_add_mime_types_compressed(&config, "application/json");


  struct _u_instance instancia_de_api;

  if (ulfius_init_instance(&instancia_de_api, puerto, NULL, NULL) != U_OK) 
  {
    fprintf(stderr, "Error al iniciar ulfius\n");
    return(EXIT_FAILURE);
  }

  ulfius_add_endpoint_by_val(&instancia_de_api, "POST", "/contador/increment", NULL, 0, &increment, &config);
  ulfius_add_endpoint_by_val(&instancia_de_api, "GET", "/contador/value", NULL, 0, &value, &config);
  ulfius_set_default_endpoint(&instancia_de_api,&no_encontrado,&config);

  if (ulfius_start_framework(&instancia_de_api) == U_OK) 
  {
    printf("Api hosteada en %d\nApreta enter para terminar", instancia_de_api.port);
    getchar();
  }
  else 
  {
    fprintf(stderr, "Error iniciando el framework\n");
  }

  printf("Fin de Ejecucion framework\n");

  ulfius_stop_framework(&instancia_de_api);
  ulfius_clean_instance(&instancia_de_api);

  return 0;
}
