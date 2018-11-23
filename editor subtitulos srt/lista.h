#ifndef LISTA_H
#define LISTA_H

#ifndef NULL
#define NULL (0)
#endif

typedef struct reg{
  char param;
  char* arg1;
  char* arg2;
  char* texto;
}regis_t;


typedef struct nodo{
  regis_t op;
  struct nodo* sig;
}nodo_t;

typedef nodo_t* lista_t;

int agregar(regis_t, lista_t*);
int esVacia(lista_t);
unsigned int length(lista_t);
void eliminarLista(lista_t*);

#endif
