/*
 * funcionesConsolaKernel.c
 *
 *  Created on: 2/6/2017
 *      Author: utnso
 */

#include "funcionesConsolaKernel.h"


///---- CONSOLA DEL KERNEL -----////

//**Esta funcion anda
///***Esta funcion imprime todos los pids de sistema
void imprimirTodosLosProcesosEnColas(){

	void imprimir (PROCESOS * aviso)
	{
		printf("Pid: %d\n", aviso->pid);

	//	imprimirPCB(aviso->pcb);

		if(aviso->pcb->exitCode == 53)
			printf("Estado: en procesamiento\n");
		else
			printf("Estado: finalizado (%d)\n",aviso->pcb->exitCode);
	}

	list_iterate(avisos, imprimir);
}

//*** probar esta funcion - no anda, arreglar
///*** Esta funcion dada una cola te imprime todos los procesos que esta contenga
void imprimirProcesosdeCola(t_queue* unaCola)
{
	void imprimir(PCB_DATA * pcb){
		printf("Pid: %d\n",pcb->pid);
		//imprimirPCB(pcb);
	}

	int a=queue_size(unaCola);
	//printf("kiusaiz: %d\n",a);//,(char*)unaCola->elements->head->data);

	if(a>0 )
		list_iterate(unaCola->elements, imprimir);
	else
		printf("No hay elementos en esta cola.\n");
}

void imprimirArchivosAbiertosProceso(int pid){
	ENTRADA_DE_TABLA_GLOBAL_DE_PROCESO* entrada_a_imprimir = encontrarElDeIgualPid(pid);
	if (entrada_a_imprimir != NULL){
		int i;
		int tamanoTabla = list_size(entrada_a_imprimir->tablaProceso);
		if(tamanoTabla-3 > 0){
			for(i=3;i<tamanoTabla;i++){
				ENTRADA_DE_TABLA_DE_PROCESO* entrada_de_tabla_proceso= list_get(entrada_a_imprimir->tablaProceso,i);
				printf("Entrada: %d \n",i);
				printf("Flags: %s \n",entrada_de_tabla_proceso->flags);
				printf("Descriptor global: %d \n",entrada_de_tabla_proceso->globalFD);
				printf("Desplazamiento: %d \n",entrada_de_tabla_proceso->offset);
			}
		}else{
			printf("El proceso no contiene archivos abiertos \n");
		}
	}else{
		printf("No existe el pid \n");
	}

}
void imprimirTablaGlobaldeArchivos(){
	if(!list_is_empty(tablaGlobalDeArchivos)){
		int i;
		int tamanoTabla = list_size(tablaGlobalDeArchivos);
		for(i=0;i<tamanoTabla;i++){
			sem_wait(&mutex_tablaGlobalDeArchivos);
			ENTRADA_DE_TABLA_GLOBAL_DE_ARCHIVOS* entrada_a_imprimir = list_get(tablaGlobalDeArchivos,i);
			sem_post(&mutex_tablaGlobalDeArchivos);
			printf("Entrada: %d \n",i);
			printf("Path: %s \n",entrada_a_imprimir->path);
			printf("Cantidad de Aperturas: %d \n",entrada_a_imprimir->cantidad_aperturas);
	}
	}else{
		printf("No hay archivos en la tabla");
	}
}

void imprimirSemaforos(){
	void imprimir(t_semaforo* sem){
		printf("El semaforo %s vale: %d\n", sem->nombre,sem->valor);
	}
	list_iterate(listaDeSemaforos, imprimir);
}


void * consolaKernel()
{
	int opcion;
	printf("\nHola Bienvenido al Kernel!\n\n"
			"Aca esta el menu de todas las opciones que tiene para hacer:\n"
			"1- Obtener el listado de procesos del sistema de alguna cola.\n"
			"2- Obtener datos sobre un proceso.\n"
			"3- Obtener la tabla global de archivos.\n"
			"4- Modificar el grado de multiprogramación del sistema.\n"
			"5- Finalizar un proceso.\n"
			"6- Detener la planificación.\n"
			"7- Imprimir de nuevo el menu.\n"
			"Elija el numero de su opcion: ");
	sem_post(&sem_ConsolaKernelLenvantada);

	char buffer[256];
	fgets(buffer,256,stdin);
	opcion = atoi(buffer);

	// Esta entrando por el default

	while(1)
	{
		switch(opcion){
			case 1:{
				printf("\nSelecione la cola que quiere imprimir:\n"
						"1- Cola de New.\n"
						"2- Cola de Ready.\n"
						"3- Cola de Exec.\n"
						"4- Cola de Bloq.\n"
						"5- Cola de Finish.\n"
						"6- Todas las colas.\n"
						"Elija el numero de su opcion: ");
					fgets(buffer,256,stdin);
					opcion = atoi(buffer);

				switch(opcion){
					case 1:{
						printf("\nProcesos de la cola de New:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirProcesosdeCola(cola_New);
						sem_post(&mutex_listaProcesos);
					}break;
					case 2:{
						printf("\nProcesos de la cola de Ready:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirProcesosdeCola(cola_Ready);
						sem_post(&mutex_listaProcesos);

					}break;
					case 3:{
						printf("\nProcesos de la cola de Exec:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirProcesosdeCola(cola_Exec);
						sem_post(&mutex_listaProcesos);
					}break;
					case 4:{
						printf("\nProcesos de la cola de Bloq:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirProcesosdeCola(cola_Wait);
						sem_post(&mutex_listaProcesos);
					}break;
					case 5:{
						printf("\nProcesos de la cola de Finish:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirProcesosdeCola(cola_Finished);
						sem_post(&mutex_listaProcesos);
					}break;
					case 6:{
						printf("\nEstos son todos los procesos:\n");

						sem_wait(&mutex_listaProcesos);
							imprimirTodosLosProcesosEnColas();
						sem_post(&mutex_listaProcesos);
					}break;
					default:{
						printf("\nOpcion invalida! Intente nuevamente.\n");
					}break;
				}


			}break;
			case 2:{
			/*	La cantidad de rafagas ejecutadas.
				b. La cantidad de operaciones privilegiadas que ejecutó.
				c. Obtener la tabla de archivos abiertos por el proceso.
				d. La cantidad de páginas de Heap utilizadas
				i. Cantidad de acciones alocar realizadas en cantidad de operaciones y en
				bytes
				ii. Cantidad de acciones liberar realizadas en cantidad de operaciones y en
				bytes
				*/
				int pid;
				printf("\Ingrese pid:\n");

				fgets(buffer,256,stdin);
				pid = atoi(buffer);
				printf("\nSelecione opcion para pid:\n"
								"1- Cantidad de rafagas ejecutadas.\n"
								"2- Cantidad de operaciones privilegiadas que ejecuto\n"
								"3- Obtener tabla de archivos abiertos por procesos\n"
								"4- Cantidad de paginas de heap utilizadas\n" //?
								"5- Cantidad de acciones alocar realizadas en bytes y en operaciones\n"
								"6- Cantidad de acciones liberar realizadas en bytes y en operaciones\n\n"
								"Elija el numero de su opcion: ");

								fgets(buffer,256,stdin);
								opcion = atoi(buffer);
								sem_wait(&mutex_listaProcesos);
									bool busqueda(PROCESOS* proceso){
										return pid == proceso->pid;
									}
									PROCESOS* proceso =  (PROCESOS*)list_find(avisos, busqueda);
									PCB_DATA* pcb;
								if(proceso == NULL){
									opcion = -1;
								}
								else{
									pcb = proceso->pcb;
								}

								sem_post(&mutex_listaProcesos);
				switch (opcion) {
				case 1: {
					printf("La cantidad de rafagas ejecutadas es %d",pcb->cantDeRafagasEjecutadas);
				}
					break;
				case 2: {
						printf("La cantidad de rafagas privilegiadas que ejecuto es %d", pcb->cantDeInstPrivilegiadas);

				}
					break;
				case 3: {
					imprimirArchivosAbiertosProceso(pid);
				}
					break;
				case 4: {
					bool encontrarPorPid(filaEstadisticaDeHeap* fila){
						return fila->pid == pid;
					}
					sem_wait(&mutex_tabla_estadistica_de_heap);
					filaEstadisticaDeHeap* fila = list_find(tablaEstadisticaDeHeap,encontrarPorPid);
					if(fila == NULL){
						printf("Pid invalido!");
					}
					else{
						printf("El pid %d pidio %d paginas de heap durante su ejecucion",pid,fila->cantidadDePaginasHistoricasPedidas);
					}
					sem_post(&mutex_tabla_estadistica_de_heap);
				}
					break;
				case 5: {
					bool encontrarPorPid(filaEstadisticaDeHeap* fila){
					return fila->pid == pid;
					}
					sem_wait(&mutex_tabla_estadistica_de_heap);
					filaEstadisticaDeHeap* fila = list_find(tablaEstadisticaDeHeap,encontrarPorPid);
					if(fila == NULL){
						printf("Pid invalido!");
					}
					else{
						printf("El pid %d hizo %d operacion alocar y fueron %d bytes reservados ",pid,fila->tamanoAlocadoEnOperaciones,fila->tamanoAlocadoEnBytes);
					}
					sem_post(&mutex_tabla_estadistica_de_heap);
				}
					break;
				case 6: {
					bool encontrarPorPid(filaEstadisticaDeHeap* fila){
										return fila->pid == pid;
									}
					sem_wait(&mutex_tabla_estadistica_de_heap);
					filaEstadisticaDeHeap* fila = list_find(tablaEstadisticaDeHeap,encontrarPorPid);
					if(fila == NULL){
						printf("Pid invalido!");
					}
					else{
						printf("El pid %d hizo %d operacion liberar y fueron %d bytes reservados ",pid,fila->tamanoLiberadoEnOperaciones,fila->tamanoLiberadoEnBytes);
					}
					sem_post(&mutex_tabla_estadistica_de_heap);
				}
					break;
				case -1:{
					printf("\nPid invalido! Intente nuevamente.\n");
					break;
				}

				default: {
					printf("\nOpcion invalida! Intente nuevamente.\n");

					}
					break;
				}
				opcion = NULL;

			}break;
			// Aca termina el Case 2, el de tirar estadisticas de un pid dado

			case 3:{
				 imprimirTablaGlobaldeArchivos();
			}break;

			case 4:{
				int gradoNuevo;
				printf("\nIngrese nuevo Grado de multiprogramacion: ");
				char buffer[256];
				fgets(buffer,256,stdin);
				gradoNuevo = atoi(buffer);
				int i;
				if (gradoNuevo>=0){
					sem_wait(&mutex_gradoDeMultiprogramacion);
					numeroGradoDeMultiprogramacion+=gradoNuevo-getConfigInt("GRADO_MULTIPROG");
					sem_post(&mutex_gradoDeMultiprogramacion);
					if(gradoNuevo > getConfigInt("GRADO_MULTIPROG")){
						for(i=0;i<gradoNuevo-getConfigInt("GRADO_MULTIPROG");i++){
							sem_post(&gradoDeMultiprogramacion);

						}
					}
					setConfigInt("GRADO_MULTIPROG", gradoNuevo);
				}else{
					printf("Comando invalido! \n");
				}

				//probablemente tengamos qeu poner un semaforo para la variable global de grado multiprogramacion

			}break;

			case 5:{
				int pid;
				printf("\nIngrese pid del proceso a finalizar: ");

				fgets(buffer,256,stdin);
				pid = atoi(buffer);
				sem_wait(&mutex_listaProcesos);
				if(seDetuvoLaPlanificacion()){
					puts("No se pudo finalizar, ya que la planificacion esta detenida.");
					break;
				}
				if(proceso_Finalizar(pid,  finalizacionDesdeKenel)) //cambiar el numero del exit code, por el que sea el correcto
					printf("\nFinalizacion exitosa.\n");
				else
					printf("\nEl Pid %d es Incorrecto! Reeintente con un nuevo pid.\n",pid);
				sem_post(&mutex_listaProcesos);

			}break;
			case 6:{
				sem_wait(&mutex_detenerPlanificacion);
				finPorConsolaDelKernel=true;
				sem_post(&mutex_detenerPlanificacion);

				printf("\nPlanificacion detenida.\n");
			}break;

			case 7:

			break;


	/*		case 9:{
				imprimirSemaforos();
			}break;
		*/
			default:{
				printf("Comando invalido! \n");
			}break;
		}
			opcion = NULL;

		printf("\n\nAca esta el menu de todas las opciones que tiene para hacer:\n"
			"1- Obtener el listado de procesos del sistema de alguna cola.\n"
			"2- Obtener datos sobre un proceso.\n"
			"3- Obtener la tabla global de archivos.\n"
			"4- Modificar el grado de multiprogramación del sistema.\n"
			"5- Finalizar un proceso.\n"
			"6- Detener la planificación.\n"
			"7- Imprimir de nuevo el menu.\n\n"
			"Elija el numero de su opcion: ");
		fgets(buffer,256,stdin);
		opcion = atoi(buffer);
	}
}

///---- FIN CONSOLA KERNEL----////

