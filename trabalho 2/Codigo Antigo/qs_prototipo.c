#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

void printVetor( int *vetor, int tamanho );
int isSorted( int *vetor );
void troca( int indice1, int indice2 );
int partition( int esq, int dir, int pivo );
void* quicksort( void *args );
void inicializaAleatoriamente( int *vetor, int tamanho );

int NTHREADS = 1;

//esq: primeiro indice do intervalo que uma thread particiona
//dir: ultimo indice do intervalo
typedef struct 
{
	int esq;
	int dir;
} Intervalo;

int *vetor; //vetor a ser ordenado
int TAM_VETOR = 10000000;

Intervalo *bolsaDeIntervalos; //vetor de intervalos a serem particionados
int NUM_MAX_INTERVALOS = 100000000;
int IN = 0, OUT = 0; //para indicar as posicoes de insercao e remocao, respectivamente

pthread_mutex_t mutex;
pthread_cond_t cond_depositar, cond_receber;

double ini, fim; //tomada de tempo

//verifica se o vetor esta ordenado
int isSorted( int *vetor )
{
	for( int i = 0; i < TAM_VETOR-1; i++ )
		if( vetor[ i ] > vetor[ i+1 ] )
			return 0;
	return 1;
}

//troca os elementos presentes nos indices dados
void troca( int indice1, int indice2 )
{
   int temp = vetor[ indice1 ];
   vetor[ indice1 ] = vetor[ indice2 ];
   vetor[ indice2 ] = temp;
}

//retorna o numero que nao for o maior nem o menor entre os tres, com o objetivo de 
//usa-lo como um pivo melhorado, ao inves de escolher um elemento aleatorio
int elementoMedio( int a, int b, int c )
{
	if (a > b)  
    	{ 
        	if (b > c) 
            		return b; 
        	else if (a > c) 
            		return c; 
        	else
            		return a; 
    	} 
    	else 
    	{ 
        	// Decided a is not greater than b. 
        	if (a > c) 
            		return a; 
        	else if (b > c) 
            		return c; 
        	else
            		return b; 
    	}  
}

//particiona o vetor em dois subvetores, um no qual todos os elementos 
//sao menores do que o pivo, e outro no qual todos sao maiores
int partition( int esq, int dir, int pivo )
{
	int leftPointer = esq -1;
	int rightPointer = dir;

	while(1)
	{
    		while( vetor[++leftPointer] < pivo ) {}
		
    		while( rightPointer > 0 && vetor[--rightPointer] > pivo ) {}

    		if( leftPointer >= rightPointer )
         		break;
      		else
         		troca( leftPointer,rightPointer );
   	}
   	
   	troca( leftPointer,dir );
   	
   	return leftPointer;
}

//imprime o vetor
void printVetor( int *vetor, int tamanho )
{
	for( int i = 0; i < tamanho; i++ )
	{
		printf( "[ " );
		printf( "%d ", vetor[ i ] );
		printf( "]" );
	}
	puts("");
}

//inicializa o vetor com numeros aleatorios
void inicializaAleatoriamente( int *vetor, int tamanho )
{
	srand( time( NULL ) );
	for( int i = 0; i < tamanho; i++ )
		vetor[ i ] = (rand()%1000) + 1;
}

//retorna o primeiro intervalo na bolsa de tarefas
Intervalo primeiroIntervaloDisponivel()
{
	Intervalo i;

	while( IN == OUT && bolsaDeIntervalos[ OUT ].dir == -1 ) 
	{
		/*
		//printf( "primeiroIntervaloDisponivel() bloqueada enquanto nao ha novos intervalos.\n"  );
		pthread_cond_wait( &cond_receber, &mutex );
		//printf( "primeiroIntervaloDisponivel() desbloqueada.\n" );
		*/
	}
	
	i = bolsaDeIntervalos[ OUT ];
	bolsaDeIntervalos[ OUT ].esq = -1;
	bolsaDeIntervalos[ OUT ].dir = -1;
	OUT = (OUT+1) % NUM_MAX_INTERVALOS;
	
	//pthread_cond_broadcast( &cond_depositar ); //sinaliza que ha ao menos uma posicao disponivel
	
	return i;
}

//insere intervalo delimitado pelos parametros esq e dir na bolsa de tarefas
void addIntervalo( int esq, int dir )
{
	while( IN == OUT && bolsaDeIntervalos[ OUT ].dir != -1 )
	{
		//printf( "addIntervalo() bloqueada enquanto nao ha posicoes livres para inserir um novo intervalo.\n" );
		//pthread_cond_wait( &cond_depositar, &mutex );
		//printf( "addIntervalo() desbloqueada.\n" );
	}

	Intervalo i;
	i.esq = esq;
	i.dir = dir;
	
	if( esq < dir )
	{
		bolsaDeIntervalos[ IN ] = i;
		IN = (IN+1) % NUM_MAX_INTERVALOS;
		//pthread_cond_broadcast( &cond_receber ); //sinaliza que ha ao menos um intervalo disponivel
	}
}

//funcao executada pelas threads
void* parallelQuicksort( void *args )
{
	while( 1 )
	{
		pthread_mutex_lock( &mutex );
		Intervalo intervalo = primeiroIntervaloDisponivel();
		pthread_mutex_unlock( &mutex );
		
		//int pivo = elementoMedio( vetor[intervalo.esq], vetor[intervalo.dir/2], vetor[intervalo.dir] );
		int pivo = vetor[ intervalo.dir ];
		int p = partition( intervalo.esq, intervalo.dir, pivo );
		numParticoes++;
		
		/*
		pthread_mutex_lock( &mutex );
		printVetor( vetor, TAM_VETOR );
		pthread_mutex_unlock( &mutex );
		*/
		
		pthread_mutex_lock( &mutex );
		addIntervalo( intervalo.esq, p-1 );
		addIntervalo( p+1, intervalo.dir );
		pthread_mutex_unlock( &mutex );
	}
}

int main( int argc, char *argv[] )
{
	pthread_t threads[ NTHREADS ];
	
	pthread_mutex_init( &mutex, NULL );
	pthread_cond_init( &cond_depositar, NULL );
  	pthread_cond_init( &cond_receber, NULL );
	
	vetor = ( int* ) calloc( sizeof( int ), TAM_VETOR );
	bolsaDeIntervalos = ( Intervalo* ) calloc( sizeof( Intervalo ), NUM_MAX_INTERVALOS );
	
	srand( time(NULL) );
	inicializaAleatoriamente( vetor, TAM_VETOR );
	
	//para indicar que os elementos estao vazios, sao inicializados com -1
	for( int i = 0; i < NUM_MAX_INTERVALOS; i++ )
	{
		bolsaDeIntervalos[ i ].esq = -1;
		bolsaDeIntervalos[ i ].dir = -1;
	}
	
	//para comecar o algoritmo, primeiro intervalo deve ser dado
	bolsaDeIntervalos[ 0 ].esq = 0;
	bolsaDeIntervalos[ 0 ].dir = TAM_VETOR-1;
	IN++;
	
	GET_TIME(ini);
	
	for( long int i = 0; i < NTHREADS; i++ )
	{
		pthread_create( &threads[ i ], NULL, parallelQuicksort, NULL );
	}
	
	for(int i = 0; i < NTHREADS; i++)
	{
    		pthread_join( threads[i], NULL );
	}
	
	GET_TIME(fim);
	printf("Tempo: %lf\n", fim-ini);
	
	//printVetor( vetor, TAM_VETOR );

	return 0;
}

