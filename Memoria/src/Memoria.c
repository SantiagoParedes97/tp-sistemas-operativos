/*
 ============================================================================
 Name        : FileSystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
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
#include "commons/config.h"
#include "laGranBiblioteca/sockets.h"
#include "laGranBiblioteca/config.h"
#define ID 2

int main(void) {

	printf("Inicializando Memoria.....\n\n");

	// ******* Declaración de la mayoria de las variables a utilizar

	socklen_t sin_size;

	struct sockaddr_storage their_addr; // Estructura que contiene la informacion de la conexion

	int listener, nuevoSocket, rta_handshake;
	int aceptados[] = {0,3};
	char ip[INET6_ADDRSTRLEN];

	char* mensajeRecibido= string_new();


	// ******* Configuracion de la Memoria a partir de un archivo

	printf("Configuracion Inicial: \n");
	configuracionInicial("/home/utnso/workspace/tp-2017-1c-While-1-recursar-grupo-/Memoria/memoria.config");
	imprimirConfiguracion();


	// ******* Conexiones obligatorias y necesarias

	listener = crearSocketYBindeo(configString("PUERTO")); // asignar el socket principal
	escuchar(listener); // poner a escuchar ese socket


	liberarConfiguracion();


	while (1) {
		sin_size = sizeof their_addr;

		if ((nuevoSocket = accept(listener, (struct sockaddr *) &their_addr, &sin_size)) == -1) {
			perror("Error en el Accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, getSin_Addr((struct sockaddr *) &their_addr), ip, sizeof ip); // para poder imprimir la ip del server

		printf("Conexion con %s\n", ip);

		if (!fork()) { // this is the child process
			close(listener); // child doesn't need the listener

			if ((rta_handshake = handshakeServidor(nuevoSocket, ID, aceptados)) == -1) {
				perror("Error con el handshake: -1");
				close(nuevoSocket);
			}

			printf("Conexión exitosa con el Server(%i)!!\n",rta_handshake);

			if(recibirMensaje(nuevoSocket,mensajeRecibido)==-1){
				perror("Error en el Reciv");
			}

			printf("Mensaje desde el Kernel: %s\n\n", mensajeRecibido);
			//free(mensajeRecibido);//OJO!!!!!ESTO HAY QUE MEJORARLO -- Comentario 2 esto esta omcentado tiera violacion de segmento
			close(nuevoSocket);
			exit(0);
		}
		close(nuevoSocket);  // parent doesn't need this


	}
	return EXIT_SUCCESS;
}
