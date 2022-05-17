#include "./funciones/funciones_contador.c"

#define PUERTO 6666
#define INCREMENT "/contador/increment"
#define VALUE "/contador/value"

int main()
{
  uint puerto = PUERTO; 

  struct _u_instance instancia_de_api;

  if (ulfius_init_instance(&instancia_de_api, puerto, NULL, NULL) != U_OK) 
  {
    fprintf(stderr, "Error al iniciar ulfius\n");
    return(EXIT_FAILURE);
  }

  ulfius_add_endpoint_by_val(&instancia_de_api, "POST", INCREMENT, NULL, 0,
      &increment, NULL);

  ulfius_add_endpoint_by_val(&instancia_de_api, "GET", VALUE, NULL, 0, &value,
      NULL);

  ulfius_set_default_endpoint(&instancia_de_api,&no_encontrado,NULL);

  if (ulfius_start_framework(&instancia_de_api) == U_OK) 
  {
    printf("Api hosteada en %d\nApreta enter para terminar",
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

  return 0;
}
