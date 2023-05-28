/*
O programa calcula a transposta de uma matriz quadrada de forma sequencial e paralela.

O programa pode receber pela linha de comando os seguintes argumentos:
- tamanho das matrizes e numero de threads;
- ajuda (opcional);
- exemplo (opcional).

Ex 1: transposta 2000 2 -> programa executa corretamente;
Ex 2: transposta -ajuda -> programa imprime uma mensagem de ajuda ao usuario;
Ex 3: transposta -exemplo -> programa imprime uma lista de exemplos de chamadas do programa.

Caso a chamada tenha sido feita corretamente, como saida o programa imprime o tempo levado pela 
execucao sequencial, pela paralela, e a aceleracao trazida pela execucao paralela.

Observacoes:
No tratamento de erros do programa, as strings utilizadas pela funcao 'erro()' estao definidas no arquivo 'mensagem.c', 
como 'msg_ajuda' ou 'msg_exemplo'.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include "timer.h"
#include "matriz.c"
#include "mensagem.c"

//numero de threads sendo usadas deve ser menor ou igual ao numero de CPUs da maquina, dado por N_MAX_THREADS
#define N_MAX_THREADS 8

void erro(const char* msg);
int is_pos_int(const char* arg);

int* matriz; //matriz que sera transposta
int* matriz_seq; //matriz transposta sequencialmente
int* matriz_par; //matriz transposta paralelamente
int tam; //tamanho da matriz
int nthreads; //numero de threads

//transposta realizada por cada thread
//recebe a ID da thread, que indica qual a sequencia de elementos que cada thread ira tratar
//ex: thread 0 trata dos elementos [0][0], [0][4], [0][8], ...], caso haja 4 threads
void* tarefa(void* arg)
{
	long int id = (long int) arg;
	
	for(int i = id; i < tam; i+= nthreads)
		for(int j = 0; j < tam; j++)
			matriz_par[i*tam + j] = matriz[j*tam + i];
			
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t* tid; //ID das threads no sistema
	double ini, fim, tempo_seq, tempo_par; //auxiliares para medir o tempo entre o inicio e fim da execucao

	//imprime a mensagem de ajuda e fecha o programa
	if(argv[1] != NULL && strcmp("-ajuda", argv[1]) == 0)
	{
		puts(msg_ajuda);
		return 1;
	}
	//imprime exemplos de chamada e fecha o programa
	else if(argv[1] != NULL && strcmp("-exemplo", argv[1]) == 0)
	{
		puts(msg_exemplo);
		return 2;
	}

	//checa se a chamada e seus argumentos sao validos
	if(argc < 3)
		erro(chamada_incorreta);

	if(is_pos_int(argv[1])) //checa se o tamanho da matriz dado é um inteiro positivo
		tam = atoi(argv[1]);
	else
		erro(arg_incorreto);

	if(is_pos_int(argv[2])) //checa se o numero de threads dado é um inteiro positivo
		nthreads = atoi(argv[2]);
	else
		erro(arg_incorreto);
		
	if(nthreads > N_MAX_THREADS) //se nthreads for maior que o numero de CPUs, ele é diminuido
		nthreads = N_MAX_THREADS;
	if(nthreads > tam * tam) //impede que threads acessem memoria fora dos limites da matriz, caso a matriz tenha numero de elementos menor que N_MAX_THREADS
		nthreads = tam * tam;

	//aloca memoria para as matrizes
	matriz = (int*) malloc(sizeof(int) * tam * tam);
	matriz_seq = (int*) malloc(sizeof(int) * tam * tam);
	matriz_par = (int*) malloc(sizeof(int) * tam * tam);

	if(matriz == NULL || matriz_seq == NULL || matriz_par == NULL)
		erro("Falha ao alocar memoria para as matrizes com a funcao malloc.");
	
	//inicializa matriz a ser transposta com numeros aleatorios
	srand(time(NULL));
	inicializa_matriz(matriz, tam);
	
	//FASE SEQUENCIAL
	
	//transposicao de forma sequencial
	GET_TIME(ini);
	transposta(matriz, matriz_seq, tam);
	GET_TIME(fim);
	tempo_seq = fim - ini;
	printf("Tempo sequencial: %lf\n", tempo_seq);
	
	//FASE PARALELA
	
	tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL)
		erro("Falha ao alocar memoria para 'tid'.");
		
	GET_TIME(ini);
	//cria as threads
	for(long int i = 0; i < nthreads; i++)
		if(pthread_create(tid+i, NULL, tarefa, (void*) i))
			erro("Falha na funcao pthread_create.");
			
	//espera o termino de todas as threads
	for(int i = 0; i < nthreads; i++)
		if(pthread_join(*(tid+i), NULL))
			erro("Falha na funcao pthread_join.");
	
	GET_TIME(fim);
	tempo_par = fim - ini;
	printf("Tempo paralelo: %lf\n", tempo_par);
	
	printf("Melhora em tempo: %lf\n", tempo_seq/tempo_par);
	
	free(matriz);
	free(matriz_seq);
	free(matriz_par);
	free(tid);
	
	return 0;
}


//FUNCOES AUXILIARES

//imprime mensagem de erro e fecha o programa
void erro(const char* msg)
{
	puts("");
	puts("Erro--");
	puts(msg);
	puts("");
	exit(-1);
}

//checa se o argumento passado é uma string de um inteiro positivo
//retorna 1 se for inteiro positivo
int is_pos_int(const char* arg)
{
	int len = strlen(arg);
	
	if(arg[0] == '-') //se o primeiro char do argumento for o sinal de menos, o numero é negativo
		return 0;
		
	if(arg[0] == '0' && len == 1) //o numero dado deve ser maior que 0
		return 0;
    	
	for(int i = 0; i < len; i++) //se qualquer caracter do argumento nao for um digito, retorna 0
		if (!isdigit(arg[i]))
			return 0;
			
	return 1;
}

