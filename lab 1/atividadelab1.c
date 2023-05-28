/*
Autor: Carlos Eduardo Andrade da Silva
DRE:   115185141

O programa soma todos os elementos de um vetor por 1 usando 2 threads. Cada thread recebe uma struct com o vetor e dois indices, indicando com qual metade do vetor as threads vao lidar.
Exemplo:
Com um vetor com 20 elementos, uma thread somaria os elementos no intervalo [0,9], enquanto a outra lidaria com o intervalo [10,19].

Os elementos do vetor nao sao recebidos pelo usuario, sao gerados pseudoaleatoriamente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

typedef struct{
	int* vetor;
	int comeco, fim; //indices inicial e final de cada metade
} t_Args;

void* incrementa(void* arg){
	t_Args *args = (t_Args *)arg;
	
	for(int i = args->comeco; i < args->fim; i++)
		args->vetor[i] += 1;
	free(args);
	
	pthread_exit(NULL);
}

int main(void){
	int N; //numero de elementos no vetor
	int* vetor;
	pthread_t tid_sistema[NTHREADS]; //ID das threads no sistema
	int thread; //ID da thread na main
	t_Args* arg;
	
	puts("Insira um valor de 10 a 100:");
	scanf("%d", &N);
	if(N <= 10 || N >= 100){
		printf("ERRO: \nO valor de N deve estar no intervalo (10, 100).\n");
		exit(-1);
	}
	
	vetor = malloc(sizeof(int)*N);
	
	//inicializa o vetor com valores pseudoaleatorios
	for(int i = 0; i < N; i++)
		vetor[i] = rand()%10;
	for(int i = 0; i < N; i++)
		printf("%d\n", vetor[i]);	
	puts("");
	
	//cria as threads
	for(thread = 0; thread < NTHREADS; thread++){
		arg = malloc(sizeof(t_Args));
		if(arg == NULL){
			printf("ERRO: malloc()\n");
			exit(-1);
		}
		arg->vetor = vetor;
		arg->comeco = (thread == 0? 0 : N/2);
		arg->fim = (thread == 0? N/2 : N);
		
		if(pthread_create(&tid_sistema[thread], NULL, incrementa, (void*)arg)){
			printf("ERRO: pthread_create()\n");
			exit(-1);
		}
	}
	
	for(thread = 0; thread < NTHREADS; thread++){
		if(pthread_join(tid_sistema[thread], NULL)){
			printf("ERRO: pthread_join()\n");
			exit(-1); 
		} 
	}
  	
	puts("Vetor incrementado:");
	for(int i = 0; i < N; i++)
		printf("%d\n", vetor[i]);
	
	printf("--Thread principal terminou\n");
	pthread_exit(NULL);
	
}
