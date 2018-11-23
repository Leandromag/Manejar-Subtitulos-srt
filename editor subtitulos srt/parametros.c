/*
    Modulo encargado de rescatar los PARAMETROS
    y verificar ERRORES DE OPERACION.
*/

#define _GNU_SOURCE //STRTOL

#include <stdio.h>
#include <string.h> //STRLEN
#include "parametros.h"
#include "errores.h"
#include <stdlib.h>   //NULL

extern int Exit_Status;
extern int Error_Num;


void buscarErroresEnOperacion(operacion_t op){
  int modifica=0,lisnul=0; //operaciones sin modificacion de archivo



  if(op.lisop == NULL)lisnul=1;

  while(op.lisop != NULL){    //Revisa lista operaciones

    if(op.lisop->op.param != 'v')modifica=1;  //si tiene operaciones que modifican

    switch ( op.lisop->op.param ){ //PARAMETRO
      case 'i':
          for(int i=0; i< strlen(op.lisop->op.arg2); i++){  //si tiene algo distinto a un numero
            if((op.lisop->op.arg2[i] > '9') || (op.lisop->op.arg2[i] < '0')){Error_Num=7;Exit_Status=2;}
          }
      case 'b':
          for(int i=0; i< strlen(op.lisop->op.arg1); i++){  //si tiene algo distinto a un numero
            if((op.lisop->op.arg1[i] > '9') || (op.lisop->op.arg1[i] < '0')){Error_Num=7;Exit_Status=2;}
          }
      break;
    }

    op.lisop=op.lisop->sig;
  }

  if(op.file_in[0] == '\0'){Error_Num=2;Exit_Status=2;}   //FALTA PARAMETRO FILE_IN
  else if( (modifica == 1) && (op.file_out[0] == '\0') ){Error_Num=2;Exit_Status=2;} //MODIFICA Y FALTA FILE_OUT
      else if((modifica == 0)&&(op.file_out[0] != '\0')){Error_Num=6;Exit_Status=2;} //NO MODIFICA Y SE INDICA FILE_OUT
      else if(lisnul){Error_Num=5;Exit_Status=Exit_Failure;} //SIN OPERACIONES

}


void esParametroEspecial(char** argv, int argc, int i, char** bufftemp2, char** bufftemp3){

  int indice2=2; //ARG1 no lo tiene pegado    -i ARG1 ARG2 ARG3

  if( *(argv[i]+2) != '\0')indice2=1; //SI ARG1 esta pegado -iARG1 ARG2 ARG3

  if((i+indice2+1 < argc) && (*(argv[i+indice2]) != '-') && (*(argv[i+indice2+1]) != '-')){
    *bufftemp2=argv[i+indice2];  //ARG2
    *bufftemp3=argv[i+indice2+1];  //ARG3
  }else{Exit_Status=Exit_Failure;Error_Num=3;} //ERROR esta parametro pero Falta argumentos!

}

void rescatarParametro(operacion_t* op,char** argv,int i, int argc){  //PARAMETRO DE UN SOLO ARGUMENTO

    regis_t aux; char* bufftemp="\0"; char* bufftemp2="\0"; char* bufftemp3="\0";

    if( *(argv[i]+2) != '\0')bufftemp=argv[i]+2;           //LO TIENE PEGADO
    else if( ((i+1 < argc) && (*(argv[i+1]) != '-')) || (*(argv[i]+1) == 'v') ){
              if(  *(argv[i]+1) != 'v')bufftemp=argv[i+1];  //LO TIENE DESPUES
                        //  y si es -v no procesa ARG1

          }else{Exit_Status=Exit_Failure;Error_Num=3;} //ERROR esta parametro pero Falta ARG1



            switch ( *(argv[i]+1) ){
              case 'f':
                        if( (*op).file_in[0] == '\0')(*op).file_in=bufftemp;    //AGREGA FILE_IN
                        else{Exit_Status=Exit_Failure;Error_Num=4;}             //ERROR DE REPETICION
              break;
              case 'o':
                        if( (*op).file_out[0] == '\0')(*op).file_out=bufftemp;  //AGREGA FILE_OUT
                        else{Exit_Status=Exit_Failure;Error_Num=4;}             //ERROR DE REPE
              break;
              case 'i': esParametroEspecial(argv,argc,i,&bufftemp2,&bufftemp3);
              default:

                        if(Exit_Status == Exit_Success){
                          aux.param=*(argv[i]+1);   //'-b' '-v' '-i'
                          aux.arg1=bufftemp;
                          aux.arg2=bufftemp2;
                          aux.texto=bufftemp3;
                          agregar(aux, &((*op).lisop) );     //AGREGA LISTA OPERACIONES
                        }
            }

}

static int esParametroValido(char p){
    return ( (p=='f') || (p=='o') || (p=='b') || (p=='i') || (p=='v') );
}


operacion_t analisarParametros(int argc, char* argv[]){
    operacion_t op={"\0","\0",NULL};

    int i;
    if(argc < 2){Exit_Status=1;Error_Num=1; //FALTAN PARAMETROS
    }else{

        i=1;
        while( (i<argc) && (Exit_Status==Exit_Success) ){

            if((*(argv[i]) == '-') && (Exit_Status == Exit_Success)){ //ES PARAMETRO!
              if(esParametroValido( *(argv[i]+1) ))rescatarParametro( &op, argv , i, argc); //es valido
              else{Exit_Status=3;Error_Num=3;}//PARAMETRO INVALIDO -" "

            }

            i++;
        }
    }
    if(Exit_Status==Exit_Success)buscarErroresEnOperacion(op);
    if(Exit_Status!=Exit_Success)imprimirStatus();

    return op;
}
