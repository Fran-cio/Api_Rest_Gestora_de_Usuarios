#include "./funciones/funciones_user.c"

#define PUERTO 7777
#define USUARIO "/api/users"

int main() 
{
  uint puerto = PUERTO; 

  y_init_logs("log_user", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, LOG,
      "main | Inicio Log de Usuario");

  struct _u_instance instancia_de_api;

  if (ulfius_init_instance(&instancia_de_api, puerto, NULL, NULL) != U_OK) 
  {
    fprintf(stderr, "Error al iniciar ulfius\n");
    return(EXIT_FAILURE);
  }

  ulfius_add_endpoint_by_val(&instancia_de_api, "POST", USUARIO, NULL, 0,
      &agregar_usuario, NULL);

  ulfius_add_endpoint_by_val(&instancia_de_api, "GET", USUARIO, NULL, 0,
      &imprimir_usuario, NULL);

  ulfius_set_default_endpoint(&instancia_de_api,&no_encontrado,NULL);

  if (ulfius_start_framework(&instancia_de_api) == U_OK) 
  {
    printf("Api hosteada en %d\nApreta enter para terminar\n",
        instancia_de_api.port);

    getchar();
  }
  else 
  {
    fprintf(stderr, "Error iniciando el framework\n");
  }

  printf("Fin de Ejecucion framework\n");

  ulfius_stop_framework(&instancia_de_api);
  ulfius_clean_instance(&instancia_de_api);

  y_close_logs();

  return 0;
}
