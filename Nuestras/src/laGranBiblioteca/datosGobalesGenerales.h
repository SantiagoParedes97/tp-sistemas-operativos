/*
 * datosGobalesGenerales.h
 *
 *  Created on: 18/5/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "commons/collections/list.h"
#include "commons/collections/queue.h"
#include "commons/collections/dictionary.h"
#include "ProcessControlBlock.h"

#ifndef DATOSGOBALESGENERALES_H_
#define DATOSGOBALESGENERALES_H_

enum id_Modulos{
	Kernel = 0,
	CPU = 1,
	Memoria = 2,
	Consola = 3,
	FileSystem = 4
};

typedef struct{
 int id;
 t_direccion direccion;
 t_valor_variable valor;
}__attribute__((packed)) t_escrituraMemoria;

typedef struct{
 int id;
 t_direccion direccion;
}__attribute__((packed)) t_pedidoMemoria;
enum tipos_de_Acciones{
	//***Todas las acciones del Kernel al enviar
		envioPCB = 4,
		envioDelPidEnSeco = 1,
		envioCantidadPaginas = 2,
		envioPaginaMemoria = 3,

	//***Todas las acciones del CPU
		asignarValor = 101,
		pedirValor = 102,

	//***Todas las acciones del Memoria
		enviarTamanoPaginas = 200,
		lineaDeCodigo = 201,
		RespuestaBooleanaDeMemoria = 202,

	//***Todas las acciones de Consola
		envioScriptAnsisop = 301,
		finalizarCiertoScript = 302,
		desconectarConsola = 303,

	//***Todas las acciones de FileSystem
		algoharaelFS = 403
};


#endif /* DATOSGOBALESGENERALES_H_ */
