/*
 * funcionesCapaFS.h
 *
 *  Created on: 13/6/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "commons/collections/list.h"


#include "datosGlobales.h"


#include "../../Nuestras/src/laGranBiblioteca/datosGobalesGenerales.h"


#ifndef FUNCIONESCAPAFS_H_
#define FUNCIONESCAPAFS_H_

bool archivoExiste(char* path);

ENTRADA_DE_TABLA_GLOBAL_DE_PROCESO * encontrarElDeIgualPid(int pid);

ENTRADA_DE_TABLA_GLOBAL_DE_ARCHIVOS* encontrarElDeIgualPath(char* path);

void agregarATablaDeProceso(int df, char* flags, t_list* tablaProceso);

void agregarATablaGlobalDeArchivos(char* path,int aperturas);

void agregarATablaGlobalDeArchivosDeProcesos(int pid ,t_list * tablaProceso);

void* serializarEscribirMemoria(int size, int offset,char* path, char* buffer);

void* serializarPedidoFs(int size, int offset,char* path);

void finalizarPid(int pid, int exitCode);

void liberarEntradaDeTablaProceso(ENTRADA_DE_TABLA_DE_PROCESO* entrada);

void liberarEntradaTablaGlobalDeArchivos(ENTRADA_DE_TABLA_GLOBAL_DE_ARCHIVOS* entrada);



void liberarEntradaTablaGlobalDeArchivosDeProceso(ENTRADA_DE_TABLA_GLOBAL_DE_PROCESO*entrada);

bool liberarRecursosArchivo(int pid);


int borrarArchivoPermanente(t_archivo estructura);

int cerrarArchivoPermanente(t_archivo estructura);

int moverUnCursor(t_moverCursor estructura);

void abrirArchivoPermanente(bool existeArchivo, t_crearArchivo estructura, int socketCPU);

void leerEnUnArchivo(t_lectura estructura, int socketCPU);

int escribirEnUnArchivo(t_mensajeDeProceso msj);


#endif /* FUNCIONESCAPAFS_H_ */
