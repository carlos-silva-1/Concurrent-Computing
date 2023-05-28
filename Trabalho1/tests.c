#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include "matriz.c"

#define TAM 2000
#define NTHREADS 2

int* transposta_paralela;
int* matriz_paralela_original;

int is_pos_int(const char* arg);

//realiza a transposta em cada thread
void* tarefa(void* arg)
{
	long int id = (long int) arg;
	
	for(int i = id; i < TAM; i+= NTHREADS)
		for(int j = 0; j < TAM; j++)
			transposta_paralela[i*TAM + j] = matriz_paralela_original[j*TAM + i];
			
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int* matriz_original_1;
	int* matriz_original_2;
	int* matriz_transposta_1;
	int* matriz_transposta_2;

	//teste - se a funcao 'is_pos_int' reconhece somente inteiros positivos
	assert(1 == is_pos_int("1")); 			//retorna 1 - inteiro de 1 digito
	assert(1 == is_pos_int("16187465")); 	//retorna 1 - inteiro de multiplos digito
	assert(0 == is_pos_int("0")); 			//retorna 0 - nao e positivo
	assert(0 == is_pos_int("-1")); 			//retorna 0 - nao e positivo
	assert(0 == is_pos_int("1.5")); 		//retorna 0 - nao e inteiro
	assert(0 == is_pos_int("a")); 			//retorna 0 - char
	assert(0 == is_pos_int("string")); 		//retorna 0 - string
	
	
	
	
	
	//testes - se a funcao 'transposta()' realiza a transposicao corretamente
	//checa se a transposta da transposta de uma matriz é igual a matriz
	matriz_original_1 = (int*) malloc(sizeof(int) * TAM * TAM);
	matriz_transposta_1 = (int*) malloc(sizeof(int) * TAM * TAM);
	matriz_transposta_2 = (int*) malloc(sizeof(int) * TAM * TAM);
	if(matriz_original_1 == NULL || matriz_transposta_1 == NULL || matriz_transposta_2 == NULL)
	{
		puts("Falha ao alocar memoria para as matrizes originais ou transpostas.");
		exit(-1);
	}
	
	srand(time(NULL));
	inicializa_matriz(matriz_original_1, TAM);
	transposta(matriz_original_1, matriz_transposta_1, TAM); //matriz_transposta_1 = transposta da original
	transposta(matriz_transposta_1, matriz_transposta_2, TAM); //matriz_transposta_2 = transposta da transposta da original
	assert(1 == igual(matriz_original_1, matriz_transposta_2, TAM));
	
	free(matriz_original_1);
	free(matriz_transposta_1);
	free(matriz_transposta_2);
	
	
	
	
	
	//teste - se forma sequencial e paralela resultam na mesma saida
	//sequencial
	matriz_original_1 = (int*) malloc(sizeof(int) * TAM * TAM);
	matriz_transposta_1 = (int*) malloc(sizeof(int) * TAM * TAM);
	if(matriz_original_1 == NULL || matriz_transposta_1 == NULL)
	{
		puts("Falha ao alocar memoria para as matrizes originais ou transpostas.");
		exit(-1);
	}
	
	inicializa_matriz(matriz_original_1, TAM);
	transposta(matriz_original_1, matriz_transposta_1, TAM); //matriz_transposta_1 = matriz transposta pela forma sequencial
	
	//paralela
	//aloca memoria
	pthread_t* tid = (pthread_t*) malloc(sizeof(pthread_t) * NTHREADS);
	matriz_paralela_original = (int*) malloc(sizeof(int) * TAM * TAM);
	transposta_paralela = (int*) malloc(sizeof(int) * TAM * TAM);
	if(tid == NULL || matriz_paralela_original == NULL || transposta_paralela == NULL)
	{
		puts("Falha ao alocar memoria para o vetor tid ou para a matriz transposta_paralela.");
		exit(-1);
	}
	
	copia_matriz(matriz_original_1, matriz_paralela_original, TAM); //matriz_paralela_original = matriz_original
	
	//cria as threads
	for(long int i = 0; i < NTHREADS; i++)
		if(pthread_create(tid+i, NULL, tarefa, (void*) i))
		{
			printf("Falha ao criar a thread #%ld.", i);
			exit(-1);
		}
		
	//espera o termino da execucao de cada thread
	for(int i = 0; i < NTHREADS; i++)
		if(pthread_join(*(tid+i), NULL))
		{
			printf("Falha ao unir a thread #%d.", i);
			exit(-1);
		}
	
	//checa se as transpostas sequencial e paralela sao iguais
	assert(1 == igual(matriz_transposta_1, transposta_paralela, TAM));
	
	//teste - se duas matrizes diferentes sao reconhecidas como diferentes
	//modifica o primeiro elemento
	if(transposta_paralela[0] == 0) transposta_paralela[0] = 1;
	else transposta_paralela[0] = 0;
	
	assert(0 == igual(matriz_transposta_1, transposta_paralela, TAM));
	
	free(matriz_original_1);
	free(matriz_transposta_1);
	free(tid);
	free(matriz_paralela_original);
	free(transposta_paralela);
	
	
	
	
	
	puts("Todos os testes passaram sem falhas.");

	return 0;
}

int is_pos_int(const char* arg)
{
	int len = strlen(arg);
	
	if(arg[0] == '-') //se o primeiro char do argumento for o sinal de menos, o numero eh negativo
		return 0;
		
	if(arg[0] == '0' && len == 1) //o numero dado deve ser maior que 0
		return 0;
    	
	for(int i = 0; i < len; i++) //se qualquer caracter do argumento nao for um digito, retorna 0
		if (!isdigit(arg[i]))
			return 0;
			
	return 1;
}
