#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ulfius.h>
#include <jansson.h>

#include "../ulfius/example_callbacks/static_compressed_inmemory_website/static_compressed_inmemory_website_callback.h"

#define PUERTO 7777

int acumulador;
/**
 * Callback function for the web application on /helloworld url call
 */
int agregar_usuario(const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;
  (void) response;

  

  return U_CALLBACK_CONTINUE;
}

int imprimir_usuario(const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;
  char *temp = malloc(32);
  sprintf(temp, "acumulador: %d\n", acumulador);

  ulfius_set_string_body_response(response, 200, temp);

  free(temp);
  return U_CALLBACK_CONTINUE;
}

int no_encontrado(const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;
  char *temp = malloc(32);
  sprintf(temp, "No encontre\n");

  ulfius_set_string_body_response(response, 200, temp);

  free(temp);
  return U_CALLBACK_CONTINUE;
}
/**
 * main function
 */
int main() 
{
  struct _u_compressed_inmemory_website_config config;

  // Add mime types
  u_map_put(&config.mime_types, ".json", "application/json");
  // specify compressed mime types
  // u_add_mime_types_compressed(&config, "application/json");


  uint puerto = PUERTO; 

  struct _u_instance instancia_de_api;

  if (ulfius_init_instance(&instancia_de_api, puerto, NULL, NULL) != U_OK) 
  {
    fprintf(stderr, "Error al iniciar ulfius\n");
    return(EXIT_FAILURE);
  }

  ulfius_add_endpoint_by_val(&instancia_de_api, "POST", "/api/users", NULL, 0, &agregar_usuario, &config);
  ulfius_add_endpoint_by_val(&instancia_de_api, "GET", "/api/users", NULL, 0, &imprimir_usuario, &config);
  ulfius_set_default_endpoint(&instancia_de_api,&no_encontrado,NULL);

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
