#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

#include <ulfius.h>
#include <jansson.h>
#include <yder.h>

#define PUERTO 7777
#define USER "admin"
#define PASSWORD "admin"


time_t tiempo;
struct tm *tlocal;
char fecha[128];

int check_injection(char *entrada)
{
  if (entrada == NULL) return 0;

  while (*entrada) {
    if (!isalnum(*entrada))
    {
      return 0;
    }
    entrada++;
  }

  return 1;
}

int agregar_usuario(const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  (void) user_data;
  json_t * json_body = NULL;
  long long status;

  if (!(request->auth_basic_user != NULL &&
        request->auth_basic_password != NULL && 
        0 == o_strcmp(request->auth_basic_user, USER) &&
        0 == o_strcmp(request->auth_basic_password, PASSWORD)))
  {
    status = 401;
    json_int_t *j_status = &status;

    json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description", json_string("Error authentication"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);



    return U_CALLBACK_UNAUTHORIZED;
  }
  json_t * json_request = ulfius_get_json_body_request(request, NULL);

  if (json_request == NULL) 
  {
    status = 500;
    json_int_t *j_status = &status;

    json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description", json_string("No se ingreso datos"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);
    json_decref(json_request);

    return U_CALLBACK_CONTINUE;
  }

  char *user = strdup(json_string_value(json_object_get(json_request,"username"))),
       *pass = strdup(json_string_value(json_object_get(json_request,"password")));

  if (!check_injection(user) || !check_injection(pass))
  {
    status = 500;
    json_int_t *j_status = &status;

    json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description", json_string("contrasenia debe tener letras o numeros"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);
    json_decref(json_request);



    free(user);
    free(pass);

    return U_CALLBACK_CONTINUE;
  }

  char *comando = malloc(128);
  int uid = (1000 + rand()%59000);
  sprintf(comando,"sudo useradd %s -p %s -u %d",user,pass,uid);

  system(comando);

  free(comando);

  strftime(fecha,128,"%d/%m/%y %H:%M:%S",tlocal);

  status = 200;

  json_body = json_object();

  json_object_set_new(json_body, "created_at", json_string(fecha));
  json_object_set_new(json_body, "user_name", json_string(user));
  json_object_set_new(json_body, "id", json_integer(uid));

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);


  struct _u_request incrementar ;
  ulfius_init_request(&incrementar);

  ulfius_set_request_properties(&incrementar,
      U_OPT_HTTP_VERB, "POST", U_OPT_HTTP_URL,
      "http://localhost:6666/contador/increment",
      U_OPT_CHECK_SERVER_CERTIFICATE, 0, U_OPT_NONE);


  strftime(fecha,128,"%d/%m/%y %H:%M:%S",tlocal);
  if(ulfius_send_http_request(&incrementar, NULL) != U_OK)
  {
    y_log_message(Y_LOG_LEVEL_ERROR,
        " %s | imprimir_usuario | Contador no disponible",fecha);
  }
  else
  {
    y_log_message(Y_LOG_LEVEL_DEBUG,
        " %s | imprimir_usuario | Contador Contador Incrementado desde: %s",
        request->client_address->sa_data);
  }

  free(user);
  free(pass);

  strftime(fecha,128,"%d/%m/%y %H:%M:%S",tlocal);
  y_log_message(Y_LOG_LEVEL_DEBUG, " %s | agregar_usuario | Usuario %d",fecha, uid);

  json_decref(json_request);

  return U_CALLBACK_CONTINUE;
}

int imprimir_usuario(const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  (void) request;
  (void) user_data;
  (void) response;

  (void) request;
  (void) user_data;

  json_t * json_body = NULL;

  long long status = 200;
  json_t * data = json_array();

  struct passwd *structura_pwd;

  setpwent();
  int cont =0;

  while ((structura_pwd = getpwent()) != NULL) 
  {
    json_t *pwd = json_object();

    json_object_set_new(pwd, "user_id", json_integer(structura_pwd->pw_gid));
    json_object_set_new(pwd, "username", json_string(structura_pwd->pw_name));

    json_array_append(data, pwd);

    cont++;
  }

  json_body = json_object();
  json_object_set_new(json_body, "data", data);

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);
  json_decref(data);

  endpwent();

  strftime(fecha,128,"%d/%m/%y %H:%M:%S",tlocal);
  y_log_message(Y_LOG_LEVEL_DEBUG, " %s | imprimir_usuario | Usuarios creados: %d",fecha, cont);

  return U_CALLBACK_CONTINUE;
}

static int no_encontrado(const struct _u_request * request, struct _u_response * response, void * user_data) 
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

  tiempo = time(0);
  tlocal = localtime(&tiempo);

  y_init_logs("log_user", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "./log/log_user.log",
      "main | Inicio Log de Usuario");

  struct _u_instance instancia_de_api;

  if (ulfius_init_instance(&instancia_de_api, puerto, NULL, NULL) != U_OK) 
  {
    fprintf(stderr, "Error al iniciar ulfius\n");
    return(EXIT_FAILURE);
  }

  ulfius_add_endpoint_by_val(&instancia_de_api, "POST", "/api/users", NULL, 0, &agregar_usuario, NULL);
  ulfius_add_endpoint_by_val(&instancia_de_api, "GET", "/api/users", NULL, 0, &imprimir_usuario, NULL);
  ulfius_set_default_endpoint(&instancia_de_api,&no_encontrado,NULL);

  if (ulfius_start_framework(&instancia_de_api) == U_OK) 
  {
    printf("Api hosteada en %d\nApreta enter para terminar\n", instancia_de_api.port);
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
