#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define L 3 //numero de threads leitoras
#define E 3 //numero de threads escritoras
#define TAM_VETOR 20

//variaveis do problema
int count_leitora = 0; //contador de threads lendo
int count_escritora = 0; //contador de threads escrevendo

int* vet; //vetor de inteiros

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_exec;

//entrada leitura
void InicLeit (int id)
{
	pthread_mutex_lock(&mutex);
	int soma = 0;
	
	printf("L[%d] quer ler.\n", id);
	
	//enquanto qualquer thread esta executando, nenhuma thread pode comecar a ler
	while(count_leitora > 0 || count_escritora > 0)
	{
		printf("L[%d] bloqueou. # de threads leitoras e escritoras executando: %d, e %d.\n", 
			id, count_leitora, count_escritora);
     	pthread_cond_wait(&cond_exec, &mutex);
     	printf("L[%d] desbloqueou.\n", id);
   	}
   	count_leitora++;
   	
   	for(int i = 0; i < TAM_VETOR; i++)
   	{
   		printf("[%d] ", vet[i]);
   		soma += vet[i];
   	}
   	printf("\nMedia encontrada pela thread %d: %lf.\n", id, soma/(double)TAM_VETOR);
   	
   	pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id)
{
	pthread_mutex_lock(&mutex);
	
   	printf("L[%d] terminou de ler.\n", id);
   	count_leitora--;
   	if(count_leitora == 0 && count_escritora == 0)
   		pthread_cond_broadcast(&cond_exec);
   		
   	pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id)
{
   	pthread_mutex_lock(&mutex);
   	
   	printf("E[%d] quer escrever.\n", id);
   
   	//enquanto qualquer thread esta executando, nenhuma thread pode comecar a escrever
   	while(count_leitora > 0 || count_escritora > 0)
   	{
     	printf("E[%d] bloqueou. # de threads leitoras e escritoras executando: %d, e %d.\n", 
     		id, count_leitora, count_escritora);
     	pthread_cond_wait(&cond_exec, &mutex);
     	printf("E[%d] desbloqueou.\n", id);
   	}
   	count_escritora++;
   	
   	/*primeira e ultima posicoes do vetor recebem a ID da thread como valor, 
   	  outras posicoes recebem 2 * ID da thread */
   	for(int i = 0; i < TAM_VETOR; i++)
   	{
   		if(i == 0 || i == TAM_VETOR - 1)
   			vet[i] = id;
   		else
   			vet[i] = 2*id;
   	}
   	
   	pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id)
{
   	pthread_mutex_lock(&mutex);
   
   	printf("E[%d] terminou de escrever.\n", id);
   	count_escritora--;
   	if(count_leitora == 0 && count_escritora == 0)
   		pthread_cond_broadcast(&cond_exec);

   	pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg)
{
	int *id = (int *) arg;
	
	while(1)
	{
		InicLeit(*id);
		printf("Leitora %d esta lendo.\n", *id);
		FimLeit(*id);
		sleep(1);
	} 
	
	free(arg);
	pthread_exit(NULL);
}

//thread escritora
void * escritor (void * arg)
{
  	int *id = (int *) arg;
  	
  	while(1)
  	{
    	InicEscr(*id);
    	printf("Escritora %d esta escrevendo.\n", *id);
    	FimEscr(*id);
    	sleep(1);
  	}
  	
  	free(arg);
  	pthread_exit(NULL);
}

//funcao principal
int main(void)
{
  	//identificadores das threads
  	pthread_t tid[L+E];
  	int id[L+E];

  	//inicializa as variaveis de sincronizacao
  	pthread_mutex_init(&mutex, NULL);
  	pthread_cond_init(&cond_exec, NULL);
  
  	vet = (int *) calloc(sizeof(int), TAM_VETOR);
  	if(vet == NULL)
  	{
  		printf("ERRO--\nFalha ao alocar memoria para o vetor.\n");
  		return 1;
  	}

  	//cria as threads leitoras
  	for(int i=0; i<L; i++)
  	{
    	id[i] = i+1;
    	if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i]))
    	{
    		printf("ERRO--\nFalha ao criar threads.\n");
    		exit(-1);
    	}
  	} 
  
  	//cria as threads escritoras
  	for(int i=0; i<E; i++)
  	{
    	id[i+L] = i+1;
    	if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L]))
    	{
    		printf("ERRO--\nFalha ao criar threads.\n");
    		exit(-1);
    	}
  	} 

  	pthread_exit(NULL);
  	return 0;
}
