/*
Um programa concorrente que multiplica 2 matrizes quadradas e imprime o tempo levado.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matrizA;
float *matrizB;
float *saida;
int nthreads;

typedef struct{
	int id; //id da linha que a thread ira processar
	int dim; //dimensao das matrizes de entrada
} tArgs;

void* tarefa(void* arg){
	tArgs *args = (tArgs*) arg;
	for(int i = args->id; i < args->dim; i += nthreads)
		for(int j = 0; j < args->dim; j++)
			for(int k = 0; k < args->dim; k++)
				saida[i*(args->dim) + j] += matrizA[i*(args->dim) + k] * matrizB[k*(args->dim) + j];
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	int dim;
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta;
	
	GET_TIME(inicio);
	
	//leitura e avaliacao dos parametros de entrada
	if(argc < 3){
		printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if(nthreads > dim) nthreads = dim;
	
	//alocacao de memoria para as matrizes
	matrizA = (float*) malloc(sizeof(float) * dim * dim);
	if(matrizA == NULL){printf("ERRO--malloc\n"); return 2;}
	matrizB = (float*) malloc(sizeof(float) * dim * dim);
	if(matrizB == NULL){printf("ERRO--malloc\n"); return 2;}
	saida = (float*) calloc(sizeof(float), dim * dim);
	if(saida == NULL){printf("ERRO--malloc\n"); return 2;}
	
	//inicializacao das matrizes
	for(int i = 0; i < dim; i++)
		for(int j = 0; j < dim; j++){
			matrizA[i*dim + j] = 9;
			matrizB[i*dim + j] = 4;
		}
		
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de inicializacao: %lf\n", delta);
	
	
	
	//multiplicacao das matrizes
	GET_TIME(inicio);

	tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL){puts("ERRO--malloc"); return 2;}
	args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
	if(args == NULL){puts("ERRO--malloc"); return 2;}
	
	//criacao das threads
	for(int i = 0; i < nthreads; i++){
		(args+i)->id = i;
		(args+i)->dim = dim;
		if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
			puts("ERRO--pthread_create");
			return 3;
		}
	}
	
	//espera o termino das threads
	for(int i = 0; i < nthreads; i++){
		pthread_join(*(tid+i), NULL);
	}
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de multiplicacao: %lf\n", delta);
	
	
	
	//finalizacao
	GET_TIME(inicio);
	
	/*
	//exibicao dos resultados
	puts("Matriz de saida:");
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++)
			printf("%.lf ", saida[i*dim + j]);
		puts("");
	}
	*/
	
	free(matrizA);
	free(matrizB);
	free(saida);
	free(args);
	free(tid);
	
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo de finalizacao: \t%lf\n", delta);
	
	return 0;
}
