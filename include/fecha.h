#include <time.h>

typedef struct Fecha
{
  time_t tiempo;
  struct tm *tlocal;
  char   fecha_hora[128];
  char*  Obtener_fecha;
}Fecha;

