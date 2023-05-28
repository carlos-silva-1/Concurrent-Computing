//FUNCOES UTILIZADAS EM 'transposta.c'

//inicializacao feita com numeros aleatorios no intervalo [0, 99]
void inicializa_matriz(int* matriz, int tam)
{
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			matriz[i*tam + j] = rand()%100;
}

void transposta(int* matriz_A, int* matriz_B, int tam)
{ 
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			matriz_B[i*tam + j] = matriz_A[j*tam + i];
}

//FUNCOES AUXILIARES PARA TESTES

//checa, um elemento de cada vez, se as duas matrizes sao iguais
//retorna 1 se sao iguais
//retorna 0 se sao diferentes
int igual(int* matriz_A, int* matriz_B, int tam)
{
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			if(matriz_A[i*tam + j] != matriz_B[i*tam + j])
				return 0;
	
	return 1;
}

void copia_matriz(int* matriz_source, int* matriz_dest, int tam)
{
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			matriz_dest[i*tam + j] = matriz_source[i*tam + j];
}

void print_matriz(int* matriz, int tam)
{
	for(int i = 0; i < tam; i++)
	{
		for(int j = 0; j < tam; j++)
			printf("%d\t", matriz[i*tam + j]);
		printf("\n");
	}
}

