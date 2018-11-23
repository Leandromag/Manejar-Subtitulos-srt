#include "lista.h"
#include <stdlib.h>
#include <string.h> //STRCPY

static lista_t ult;

int agregar(regis_t reg, lista_t* lis){ //AGREGAR FINAL
  lista_t aux;

  aux= malloc(sizeof(nodo_t));

  aux->op.param = reg.param;
  aux->op.arg1=reg.arg1;
  aux->op.arg2=reg.arg2;
  aux->op.texto=reg.texto;

  aux->sig = NULL;

  if(*lis == NULL)*lis=aux;
  else ult->sig=aux;
  ult=aux;

  return 0;
}

int esVacia(lista_t lis){
  return (lis == NULL);
}

unsigned int length(lista_t lis){
  if (esVacia(lis)) return 0;
  else return 1 + length(lis->sig);
}

void eliminarLista(lista_t* lis){
  lista_t aux;
  while(*lis != NULL){
    aux=*lis;
    *lis=(*lis)->sig;
    free(aux);
  }

}
