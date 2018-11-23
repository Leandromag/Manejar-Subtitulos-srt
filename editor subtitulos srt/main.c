/*
    aplicación que manipula
    archivos de subtı́tulos en formato SubRip.

    autor:c.l.Piscitelli

    */
#include <stdio.h>
#include <stdlib.h> //NULL - STRTOL
#include "parametros.h"
#include "lista.h"
#include "archivo.h"
#include <unistd.h> //UNLINK

extern int Exit_Status;//var global


long rescatarArgumento(char* arg){

  char *ptrResto;
  long num;

  num = strtol( arg, &ptrResto, 10 );
  if(ptrResto > arg+19) printf("es invalida la conversion!\n\n");

  return num;
}


int main(int argc, char* argv[]){
  operacion_t operacion;
  long arg1,arg2; lista_t pun_ini;

  operacion=analisarParametros(argc,argv);//devuelve un struct de operacion

  if(Exit_Status==0){

    FILE* file_in=fopen(operacion.file_in,"r");
    FILE* file_in_pun=file_in;
    FILE* file_out=NULL;
    FILE* file_aux=NULL;

    if(file_in == NULL){
        printf("Error al intentar abrir archivo %s\n",operacion.file_in);
        Exit_Status=4;
    }else{

      if(operacion.file_out[0] != '\0'){
                file_out=fopen(operacion.file_out,"w+");
                file_aux=fopen("aux","w+");
              }

      pun_ini=operacion.lisop;


      while(operacion.lisop != NULL){

        if(operacion.lisop->op.arg1[0] != '\0')arg1=rescatarArgumento(operacion.lisop->op.arg1);
        if(operacion.lisop->op.arg2[0] != '\0')arg2=rescatarArgumento(operacion.lisop->op.arg2);


        switch (operacion.lisop->op.param){
          case 'v': validarArchivo(file_in);
          break;
          case 'b': borrarLineaArchivo(file_in,file_out,arg1);
          break;
          case 'i': agregarLineaArchivo(file_in,file_out,arg1,arg2,operacion.lisop->op.texto);
          break;
        }

        rewind(file_in);
        if(operacion.file_out[0] != '\0'){rewind(file_out);rewind(file_aux);}

        if((operacion.lisop->op.param == 'b') || (operacion.lisop->op.param == 'i')){
          fclose(file_aux);
          file_aux=fopen("aux","w+");       //ABRE AUX COMO ESCRITURA
          copiarArchivo(file_aux,file_out); //ESCRIBE AUX
          fclose(file_aux);
          file_aux=fopen("aux","r");        //ABRE AUX COMO LECTURA
          fclose(file_out);
          file_out=fopen(operacion.file_out,"w+"); //ABRE OUT VACIO
          file_in=file_aux;
        }

        operacion.lisop=operacion.lisop->sig;
      }
      operacion.lisop=pun_ini;
      file_in=file_in_pun;

      if(file_out != NULL){
        copiarArchivo(file_out,file_aux);
        fclose(file_out);
        fclose(file_aux);
        unlink("aux");  //borra AUX de mem fisica
      }
      fclose(file_in);

    }

  }


  eliminarLista(&operacion.lisop); //libera memoria - sin hacer nada tiene 3 allocs, 2 free
  return Exit_Status;
}
