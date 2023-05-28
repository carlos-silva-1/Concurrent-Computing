#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NLEITORES  2
#define NESCRITORES  2
#define TAM_VETOR 100

int *vetor;

sem_t em_e, em_l, escr, leit; //semaforos
int e = 0, l = 0; //globais
int posicaoParaEscrita = 0;

//a impressao nao esta dentro de um lock para simular varias threads leitoras podendo ler ao mesmo tempo,
//porem pode ser que a impressao seja interrompida
void le( int id )
{
	printf( "Thread %d comeca sua impressao.\n", id );
	printf( "[ " );
	for( int i = 0; i < TAM_VETOR; i++ )
		printf( "%d ", vetor[ i ] );
	printf( "]\n" );
}

void escreve( int id, int elemento )
{
	printf( "Thread %d insere %d.\n", id, elemento );
	vetor[ posicaoParaEscrita ] = elemento;
	posicaoParaEscrita = (posicaoParaEscrita+1) % TAM_VETOR;
}

void* leitor( void* args )
{
	int *id = ( int* ) args;
	
	while( 1 )
	{
		sem_wait( &leit );
		sem_wait( &em_l );
		l++;
		if( l == 1 )
			sem_wait( &escr );
		sem_post( &em_l );
		sem_post( &leit );
		
		le( *id );
		
		sem_wait( &em_l );
		l--;
		if( l == 0 )
			sem_post( &escr );
		sem_post( &em_l );
	}
	
	pthread_exit( NULL );
}

void* escritor( void* args )
{
	int *id	= ( int* ) args;
	
	while( 1 )
	{
		sem_wait( &em_e );
		e++;
		if( e == 1 )
			sem_wait( &leit );
		sem_post( &em_e );
		
		sem_wait( &escr );
		escreve( *id, (rand()+1)%10 );
		sem_post( &escr );
		
		sem_wait( &em_e );
		e--;
		if( e == 0 )
			sem_post( &leit );
		sem_post(&em_e);
	}
	
	pthread_exit( NULL );
}

int main( int argc, char *argv[] )
{	
	pthread_t tid[ NLEITORES+NESCRITORES ];
	int *id[ NLEITORES+NESCRITORES ];
	
	vetor = ( int* ) calloc( sizeof( int ), TAM_VETOR );
	
	srand( time( NULL ) );
	
	//aloca espaco para os IDs das threads
  	for( int i = 0; i < NLEITORES+NESCRITORES; i++)
  	{
    	id[ i ] = malloc( sizeof(int) );
    	if( id[ i ] == NULL )
    		exit( -1 );
    	*id[ i ] = i+1;
  	}
	
	//inicia os semaforos
  	sem_init( &em_e, 0, 1 );
  	sem_init( &em_l, 0, 1 );
  	sem_init( &escr, 0, 1 );
  	sem_init( &leit, 0, 1 );
  	
  	//cria as threads escritoras
  	for( int i = 0; i < NESCRITORES; i++ )
  	{
    	if( pthread_create( &tid[ i ], NULL, escritor, (void *) id[ i ]) )
    		exit(-1);
  	}
  	
  	//cria as threads leitoras
  	for( int i = 0; i < NLEITORES; i++ )
  	{
    	if( pthread_create( &tid[ i+NESCRITORES ], NULL, leitor, (void *) id[ i+NESCRITORES ] ) )
    		exit(-1);
  	} 
  	
  	pthread_exit(NULL);

	return 0;
}

