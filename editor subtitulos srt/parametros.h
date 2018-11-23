#ifndef PARAMETROS_H
#define PARAMETROS_H

  #include "lista.h"
  typedef struct{
    char* file_in;
    char* file_out;
    lista_t lisop;     //lista de operaciones
  }operacion_t;


  operacion_t analisarParametros(int, char**);

#endif
