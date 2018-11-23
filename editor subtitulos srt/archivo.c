
#define _GNU_SOURCE   //GETLINE
#include <stdio.h>
#include <stdlib.h>    //free
#include <string.h>    //strcat


typedef struct{
  long indice;
  char* strindice;
  long start,end;
  char* strstartend;
  char* linea;
}registro_t;

long convertiraMillis(unsigned int HH,unsigned int MM,unsigned int SS,unsigned int mmm){
    return (HH*3600000)+(MM*60000)+(SS*1000)+mmm;
}

void convertirstrMillis(long arg1, long arg2, char* str){
  //      1h 3600000  ||  1mn 60000ms  ||  1seg 1000ms
    int HH=0,MM=0,SS=0;
    int HH2=0,MM2=0,SS2=0;

    while(arg1 >= 1000){
        if(arg1 >= 3600000){arg1=arg1-3600000; HH++;}
        else if(arg1 >= 60000){arg1=arg1-60000; MM++;if(MM>99){MM=0; HH++;}}
        else if(arg1 >= 1000){arg1=arg1-1000; SS++;if(SS>99){SS=0; MM++;}}
    }
    while(arg2 >= 1000){
        if(arg2 >= 3600000){arg2=arg2-3600000; HH2++;}
        else if(arg2 >= 60000){arg2=arg2-60000; MM2++;if(MM2>99){MM2=0; HH2++;}}
        else if(arg2 >= 1000){arg2=arg2-1000; SS2++;if(SS2>99){SS2=0; MM2++;}}
    }
    sprintf(str,"%.2d:%.2d:%.2d,%.3ld --> %.2d:%.2d:%.2d,%.3ld\n",HH, MM, SS, arg1, HH2, MM2, SS2, arg2);
}


registro_t leerRegistro(FILE* arch){
  static registro_t registro;

  char* indice=NULL; //getline
  char* startend=NULL; //getline
  char* linea=NULL; //getline
  char* lineaAux=NULL; //getline
  size_t tam=0;     //getline
  ssize_t res;      //getline

  char *ptrResto;   //strtol
  long num;         //strtol
  int res2;         //fscanf

  unsigned int HH,MM,SS,mmm,HH2,MM2,SS2,mmm2;

  res = getline(&indice, &tam, arch);
  if(res>1){ //LECTURA NORMAL RES = 4
    num = strtol(indice, &ptrResto, 10); //base 10
    registro.indice=num;
    registro.strindice=indice;

    getline(&startend, &tam, arch);
    res2=sscanf (startend,"%2u:%2u:%2u,%3u --> %2u:%2u:%2u,%3u\n", &HH, &MM, &SS, &mmm, &HH2, &MM2, &SS2, &mmm2);
    if(res2 == 8){
      registro.start=convertiraMillis(HH, MM, SS, mmm);
      registro.end=convertiraMillis(HH2,MM2,SS2,mmm2);
    }else{
      registro.start=-1;
      registro.end=-1;
    }

    registro.strstartend=startend;

    getline(&linea, &tam, arch);
    do{
      getline(&lineaAux, &tam, arch);
      linea=strcat(linea, lineaAux);
    }while(lineaAux[0]!=13);

    registro.linea=linea;
    free(lineaAux);
  }else{
    free(indice); //free getline fin archivo
    registro.indice=-1; //FIN ARCHIVO
  }

  return registro;
}

int lineaMayor36(char* linea){
  int i=0,cant=0,res=0;
  while(linea[i] != '\0'){
      if(linea[i] == '\n')cant=0;
      if(cant>36)res++;
      i++;cant++;
  }
  return res;
}

int cantlineas(char* linea){
  int i=0, cant=0;
  while(linea[i] != '\0'){
      if(linea[i] == '\n')cant++;
      i++;
  }
  cant--;
  return cant;
}

void validarArchivo(FILE* archivo){
  registro_t regis;
  int indice=1;  long actstart=0,antend=0;
  unsigned long tiempo=0,caracteres=0;

  regis=leerRegistro(archivo);
  while (regis.indice != -1){

    if(regis.end - regis.start < 1000)tiempo=tiempo+(regis.end - regis.start);
    else tiempo=0;
    caracteres=caracteres+strlen(regis.linea)-4;

    if(tiempo > 1000){
      if(caracteres > 25)printf("Error en indice %ld, demasiados caracteres en 1seg\n",regis.indice);
      tiempo=0; caracteres=0;
    }

    actstart=regis.start;
    if(actstart<antend)printf("Error en indice %ld, subtitulos solapados! con indice anterior\n",regis.indice);
    if(actstart - antend < 75)printf("Error en indice %ld, tiene menos de 75 milisegundos de separacion\n",regis.indice);
    antend=regis.end;

    if(regis.indice != indice){
        if(indice==1)printf("Error en indice %ld, los indices del archivo no comienzan en 1\n",regis.indice);
        printf("Error en indice %ld, no es consecutivo\n",regis.indice);
        indice=regis.indice;
    }

    if(regis.end - regis.start < 1000)printf("Error en indice %ld, dura menos de 1 seg\n",regis.indice);
    if(regis.end - regis.start > 7000)printf("Error en indice %ld, dura mas de 7 seg\n",regis.indice);
    if(lineaMayor36(regis.linea) > 0)printf("Error en indice %ld, tiene mas de 36 caracteres\n",regis.indice);
    if(cantlineas(regis.linea) > 2)printf("Error en indice: %ld, tiene mas de 2 lineas\n", regis.indice);;


    free(regis.strindice);
    free(regis.strstartend);
    free(regis.linea);
    regis=leerRegistro(archivo);
    indice++;
  }
  rewind(archivo);
}


void borrarLineaArchivo(FILE* arch_in,FILE* arch_out,long indice){
  registro_t regis;  int corrimiento=0;

  regis=leerRegistro(arch_in);
  while (regis.indice != -1){

    if(regis.indice != indice){
      if(corrimiento==1)sprintf(regis.strindice,"%lu\n", regis.indice-1);
      fputs(regis.strindice,arch_out);
      fputs(regis.strstartend,arch_out);
      fputs(regis.linea,arch_out);
    }else corrimiento=1;

    free(regis.strindice);
    free(regis.strstartend);
    free(regis.linea);

    regis=leerRegistro(arch_in);
  }

}

void agregarLineaArchivo(FILE* arch_in,FILE* arch_out ,long arg1,long arg2,char* linea){
  registro_t regis; int corrimiento=0; char straux[40];
  long indicetemp; char buff[20];

  regis=leerRegistro(arch_in);
  while (regis.indice != -1){

    if((regis.start > arg1)&&(corrimiento==0)){  //agrega al PRINCIPIO O AL MEDIO
      convertirstrMillis(arg1,arg2,straux);
      fputs(regis.strindice,arch_out);
      fputs(straux,arch_out);
      fputs(linea,arch_out);
      fputc('\n',arch_out);
      fputc(13,arch_out);
      fputc('\n',arch_out);

      corrimiento=1;
    }

    sprintf(regis.strindice,"%lu\n", regis.indice+corrimiento);
    fputs(regis.strindice,arch_out);
    fputs(regis.strstartend,arch_out);
    fputs(regis.linea,arch_out);


    indicetemp=regis.indice;

    free(regis.strindice);
    free(regis.strstartend);
    free(regis.linea);
    regis=leerRegistro(arch_in);
  }
  if(corrimiento == 0){   //agrega al FINAL
    convertirstrMillis(arg1,arg2,straux);
    sprintf(buff,"%lu\n", indicetemp+corrimiento+1);
    fputs(buff,arch_out);
    fputs(straux,arch_out);
    fputs(linea,arch_out);
    fputc('\n',arch_out);
    fputc(13,arch_out);
  }

}



void copiarArchivo(FILE* file_aux,FILE* file_out){
  registro_t regis;

  regis=leerRegistro(file_out);

  while (regis.indice != -1){

    fputs(regis.strindice,file_aux);
    fputs(regis.strstartend,file_aux);
    fputs(regis.linea,file_aux);

    free(regis.strindice);
    free(regis.strstartend);
    free(regis.linea);
    regis=leerRegistro(file_out);
  }

  rewind(file_aux);
  rewind(file_out);

}
