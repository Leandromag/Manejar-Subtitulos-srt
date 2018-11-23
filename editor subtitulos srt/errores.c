#include <stdio.h>
#include "errores.h"

int Exit_Status=Exit_Success;  //GLOBAL
int Error_Num=0;               //GLOBAL cualquier modulo puede indicar error;

static char* Errores[8]={    //LOCAL
  "SIN ERRORES",
  "ERROR 1: Faltan parametros",
  "ERROR 2: Faltan parametros obligatorios",
  "ERROR 3: Parametro/s invalido/s",
  "ERROR 4: Parametro/s obligatorio/s repetido/s",
  "ERROR 5: No hay operacion/es",
  "ERROR 6: No es necesario indicar salida",
  "ERROR 7: El argumento debe ser un numero entero sin signo"
};


void imprimirAyuda(){
  printf("\n\n---------------------------------AYUDA-----------------------------------\n\n"
        "Modo de empleo: [PROGRAMA] <[ARCHIVO_IN] [ARCHIVO_OUT] [OPERACIONES]... >\n\n"
        "*puede no respetar el orden de los parametros.\n"
        "si se realizan [OPERACIONES] de modificacion en el [ARCHIVO_IN]\n"
        "se debera indicar el archivo de salida: [ARCHIVO_OUT],\n"
        "caso contrario, no es necesario.\n\n"
        "\tparametros validos:\n\n"
        "\t-f      archivo de entrada                obligatorio\n"
        "\t-o      archivo de salida                 obligatorio si se modifica el archivo\n\n"
        "\t-v      verifica el archivo               [no modifica]\n"
        "\t-b      borra una linea del archivo       [modifica]\n"
        "\t-i      ingresa una linea al archivo      [modifica]\n\n\n"
        "Estado de salida:\n"
        "0  si todo fue bien\n"
        "1  si se paso parametros -f o -o repetidos\n"
        "2  si no se ingreso parametro/s obligatorio/s\n"
        "3  si hubo parametro/s invalido/s\n"
        "\n-------------------------------------------------------------------------\n\n");
}

void imprimirStatus(){
    printf("\n%s\n", Errores[Error_Num]);
    if(Error_Num!=Exit_Success)imprimirAyuda();
}
