#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long int ntermos; //numero de termos da serie
long int nthreads; //numero de threads

void* tarefa(void* arg)
{
	long int id = (long int) arg;
	double* somaLocal; //variavel local da soma de elementos
	long int tamBloco = ntermos/nthreads; //tamanho do bloco da thread
	long int ini = id * tamBloco; //termo inicial na thread
	long int fim; //termo final (nao processado) na thread
	
	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal == NULL){
		fprintf(stderr, "Erro-malloc\n");
		exit(1);
	}
	*somaLocal = 0;
	
	if(id == nthreads-1) fim = ntermos;
	else fim = ini + tamBloco; // trata o resto se houver
	
	for(long int i = fim-1; i >= ini; i--)
		*somaLocal += 1.0/(1+2*i) * pow(-1, i);

	pthread_exit((void *)somaLocal);
}

int main(int argc, char* argv[])
{
	double somaSeq = 0;
	double somaConc = 0;
	double ini, fim; //tomada de tempo
	pthread_t *tid; //id das threads no sistema
	double* retorno; //valor retornado pelas threads
	
	//recebe e valida a entrada
	if(argc < 3)
	{
		fprintf(stderr, "Digite: %s <numero de termos> <numero de threads>\n", argv[0]);
		return 1;
	}
	ntermos = atoll(argv[1]);
	nthreads = atoi(argv[2]);
		
	//soma sequencial dos termos
	GET_TIME(ini);
	for(long int i = 0; i< ntermos; i++)
		somaSeq += 1.0/(1+2*i) * pow(-1, i);
	somaSeq *= 4;
	GET_TIME(fim);
	printf("Tempo sequencial: %lf\n", fim-ini);
	
	//soma concorrente dos termos
	GET_TIME(ini);
	tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL)
	{
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	//criar as threads
	for(long int i = 0; i < nthreads; i++)
	{
		if(pthread_create(tid+i, NULL, tarefa, (void*) i))
		{
			fprintf(stderr, "ERRO--pthread_create\n");
			return 3;
		}
	}
	//aguardar o termino das threads
	for(long int i = nthreads-1; i >= 0; i--)
	{
		if(pthread_join(*(tid+i), (void**) &retorno))
		{
			fprintf(stderr, "ERRO--pthread_join\n");
			return 3;
		}
		//soma global
		somaConc += *retorno;
		free(retorno);
	}
	somaConc *= 4;
	GET_TIME(fim);
	printf("Tempo concorrente: %lf\n", fim-ini);
	
	//exibir os resultados
	printf("M_PI: \t\t%.15lf\n", M_PI);
	printf("Soma seq: \t%.15lf\n", somaSeq);
	printf("Soma conc: \t%.15lf\n", somaConc);
	
	//libera as areas de memoria alocadas
	free(tid);
	
	return 0;
}
