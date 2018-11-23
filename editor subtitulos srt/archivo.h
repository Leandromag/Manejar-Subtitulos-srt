#ifndef ARCHIVO_H
#define ARCHIVO_H

void validarArchivo(FILE*);
void borrarLineaArchivo(FILE*,FILE*,long);
void agregarLineaArchivo(FILE*,FILE*,long,long,char*);
void copiarArchivo(FILE*,FILE*);

#endif
