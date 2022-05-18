#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

#include <ulfius.h>
#include <jansson.h>
#include <yder.h>

#include "../../include/fecha.h"

#define USER "admin"
#define PASSWORD "admin"
#define LOG "./log/log_user.log"


Fecha fecha;
/*
 *
 */

int check_injection(char *entrada)
{
  if (entrada == NULL) return 0;

  while (*entrada)
  {
    int check = (*entrada == '%') || (*entrada == '|') || (*entrada == '>')
    || (*entrada == '<');
    if (check)
    {
      return 0;
    }
    entrada++;
  }

  return 1;
}

/*
 *
 */


char *obtener_fecha_hora()
{
  fecha.tiempo = time(0);
  fecha.tlocal = localtime(&fecha.tiempo);
  strftime(fecha.fecha_hora,128,"%d/%m/%y %H:%M:%S",fecha.tlocal);

  return fecha.fecha_hora;
}

/*
 *
 */

// int acreditacion(const struct _u_request *request, struct _u_response * response)
// {
//   if (!(request->auth_basic_user != NULL &&
//         request->auth_basic_password != NULL && 
//         0 == o_strcmp(request->auth_basic_user, USER) &&
//         0 == o_strcmp(request->auth_basic_password, PASSWORD)))
//   {
//     long long status = 401;
//     json_int_t *j_status = &status;
//
//     json_t *json_body = json_object();
//     json_object_set_new(json_body, "code", json_integer(*j_status));
//     json_object_set_new(json_body, "description",
//         json_string("Error authentication"));
//
//     ulfius_set_json_body_response(response, (uint)status, json_body);
//     json_decref(json_body);
//
//     return 0;
//   }
//
//   return 1;
// }
//
/*
 *
 */

json_t * obtener_req_json(const struct _u_request *request, struct _u_response * response)
{
  json_t * json_request = ulfius_get_json_body_request(request, NULL);

  if (json_request == NULL) 
  {
    long long status = 500;
    json_int_t *j_status = &status;

    json_t *json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description",
        json_string("No se ingreso datos"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);
    json_decref(json_request);

    return NULL;
  }
  return json_request;
}

/*
 *
 */

int verificar_injection(struct _u_response * response, char* user, char* pass)
{

  if (!check_injection(user) || !check_injection(pass))
  {
    long long status = 500;
    json_int_t *j_status = &status;

    json_t *json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description",
        json_string("contrasenia debe tener letras o numeros"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);

    free(user);
    free(pass);

    return 0;
  }

  return 1;
}

/*
 *
 */

int verificar_usuario_en_uso(char* user)
{
  if(getpwnam(user) != NULL)
    return 1;

  return 0;
}

/*
 *
 */

int nombre_en_uso(struct _u_response * response, char* user)
{

  if (verificar_usuario_en_uso(user))
  {
    long long status = 500;
    json_int_t *j_status = &status;

    json_t *json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description",
        json_string("Nombre de usuarion en uso"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);

    return 0;
  }

  return 1;
}

/*
 *
 */

int usuario_generado(struct _u_response * response, char* user)
{

  if (!verificar_usuario_en_uso(user))
  {
    long long status = 500;
    json_int_t *j_status = &status;

    json_t *json_body = json_object();
    json_object_set_new(json_body, "code", json_integer(*j_status));
    json_object_set_new(json_body, "description",
        json_string("Usuario no generado"));

    ulfius_set_json_body_response(response, (uint)status, json_body);
    json_decref(json_body);

    return 0;
  }

  return 1;
}

/*
 *
 */

int agregar_usuario(const struct _u_request * request,
    struct _u_response * response, void * user_data) 
{
  (void) user_data;
  json_t * json_body = NULL;
  long long status;
  //
  // if (!acreditacion(request, response))
  //   return U_CALLBACK_UNAUTHORIZED;
  //
  json_t * json_request= obtener_req_json(request, response);

  if (json_request == NULL) 
    return U_CALLBACK_CONTINUE;


  char *user = strdup(json_string_value(json_object_get(json_request,"username"))),
       *pass = strdup(json_string_value(json_object_get(json_request,"password")));

  if (!verificar_injection(response, user, pass))
        return U_CALLBACK_CONTINUE;

  if(!nombre_en_uso(response, user))
    return U_CALLBACK_CONTINUE;

  char *comando = malloc(128);
  uint uid;
  while(1)
  {
    uid = (uint)(1000 + rand()%59000);
    if (getpwuid(uid) == NULL) 
      break;
  }

  sprintf(comando,"sudo useradd %s -p %s -u %d",user,pass,uid);
  system(comando);
  free(comando);

  if(!usuario_generado(response, user))
    return U_CALLBACK_CONTINUE;

  status = 200;

  json_body = json_object();

  json_object_set_new(json_body, "created_at", json_string(obtener_fecha_hora()));
  json_object_set_new(json_body, "user_name", json_string(user));
  json_object_set_new(json_body, "id", json_integer(uid));

  ulfius_set_json_body_response(response, (uint)status, json_body);
  json_decref(json_body);

  struct _u_request incrementar ;
  ulfius_init_request(&incrementar);

  ulfius_set_request_properties(&incrementar,
      U_OPT_HTTP_VERB, "POST", U_OPT_HTTP_URL,
      "contadordeusuarios.com.ar/contador/increment",
      U_OPT_CHECK_SERVER_CERTIFICATE, 0,U_OPT_AUTH_BASIC_USER, request->auth_basic_user,
      U_OPT_AUTH_BASIC_PASSWORD,request->auth_basic_password, U_OPT_NONE);


  if(ulfius_send_http_request(&incrementar, NULL) != U_OK)
  {
    y_log_message(Y_LOG_LEVEL_ERROR,
        " %s | imprimir_usuario | Contador no disponible", obtener_fecha_hora());
  }
  // else
  // {
    // y_log_message(Y_LOG_LEVEL_DEBUG,
    //     " %s | imprimir_usuario | Contador Incrementado desde: %s",
    //     fecha, u_map_get(request -> map_header, "X-Forwarded-For"));
  // }

  free(user);
  free(pass);

  y_log_message(Y_LOG_LEVEL_DEBUG, " %s | agregar_usuario | Usuario %d",
      obtener_fecha_hora(), uid);

  json_decref(json_request);

  return U_CALLBACK_CONTINUE;
}

/*
 *
 */

int imprimir_usuario(const struct _u_request * request,
    struct _u_response * response, void * user_data) 
{
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

  y_log_message(Y_LOG_LEVEL_DEBUG,
      " %s | imprimir_usuario | Usuarios creados: %d", obtener_fecha_hora(), cont);

  return U_CALLBACK_CONTINUE;
}

/*
 *
 */

static int no_encontrado(const struct _u_request * request,
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


