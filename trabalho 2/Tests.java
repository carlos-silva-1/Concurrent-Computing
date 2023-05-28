class Tests
{
	static int TAM_VETOR;
	static int[] vetor;
	
	/*retorna true se cada elemento do vetor e menor ou igual ao proximo
  	  se nao, retorna false */
  	public static boolean isSorted( int[] vetor )
  	{
  		for( int i = 0; i < TAM_VETOR-1; i++ )
  			if( vetor[ i ] > vetor[ i+1 ] )
  				return false;
  		return true;
  	}
  	
  	/* inicializa o vetor de forma crescente da seguinte forma:
  	   elemento no indice 'i' tem valor 'i' */
  	public static void inicializaVetorOrdenado( int[] vetor )
  	{
  		for( int i = 0; i < TAM_VETOR; i++ )
  			vetor[ i ] = i;
  	}
  	
  	/* inicializa o vetor de forma decrescente da seguinte forma:
  	   elemento no indice 'i' tem valor -'i' */
  	public static void inicializaVetorNaoOrdenado( int[] vetor )
  	{
  		for( int i = 0; i < TAM_VETOR; i++ )
  			vetor[ i ] = -i;
  	}
  	
  	/* recebe dois indices e troca o elemento presente no indice1 
  	   pelo elemento no indice2, e vice-versa */
  	public static void troca( int indice1, int indice2, int[] vetor )
  	{
  		int temp = vetor[ indice1 ];
  		vetor[ indice1 ] = vetor[ indice2 ];
  		vetor[ indice2 ] = temp;
  	}
  	
  	/* recebe 3 indices do vetor e retorna o indice no qual o elemento
  	   medio esta presente.
  	   usada para encontrar um pivo com uma chance maior de estar 
  	   no meio do vetor apos a particao */
  	public static int indiceElementoMedio( int a, int b, int c )
  	{
  		if( vetor[a] > vetor[b] )  
    		{ 
        		if( vetor[b] > vetor[c] ) 
            			return b; 
        		else if( vetor[a] > vetor[c] ) 
            			return c; 
        		else
            			return a; 
    		} 
    		else 
    		{ 
        		if( vetor[a] > vetor[c] ) 
           			return a; 
        		else if( vetor[b] > vetor[c] ) 
            			return c; 
        		else
            			return b; 
    		}  
  	}
  	
  	/*particiona o intervalo [esq, dir] em torno do pivo
	  antes do pivo ficam todos os elementos menores que ele, e depois 
	  do pivo, todos os maiores.
	  retorna a posicao do pivo no vetor final */
  	public static int partition( int esq, int dir, int[] vetor ) 
	{
        	int i = esq, j = dir;
  
        	/* elemento medio de uma amostra do vetor escolhido como pivo */
        	int pivo = indiceElementoMedio( esq, (esq+dir)/2, dir );

        	/*troca o pivo com o elemento na posicao final do vetor */
        	troca( j, pivo, vetor );
        	j--; 
  
        	while (i <= j)
        	{
        		/* enquanto o ponteiro que percorre pela esquerda nao encontra um 
        	  	   elemento maior que o pivo, aponta para o elemento a esquerda ate
        	  	   encontrar um maior que o pivo a esquerda deste */
            		if (vetor[i] <= vetor[dir])
            		{
                		i++;
                		continue;
            		}
  
  			/* enquanto o ponteiro que percorre pela direita nao encontra um 
        	  	   elemento menor que o pivo, aponta para o elemento a direita ate
        	  	   encontrar um menor que o pivo a direita deste */
            		if (vetor[j] >= vetor[dir])
            		{
                		j--;
                		continue;
            		}

			/*somente troca um elemento maior que o pivo a esquerda 
			  por um menor que o pivo a direita */
  			troca( j, i, vetor );
            		j--;
            		i++;
       		}
  
        	/* poe o pivo na sua posicao correta, de modo que elementos menores
          	   estao a sua esquerda, enquanto maiores estao a direita */
        	troca( j+1, dir, vetor );
		
        	return j + 1;
    	}
    
    	/* checa se todos os elementos da particao esquerda de um vetor sao menores que o pivo */
    	public static boolean particaoEsquerdaValida( int[] vetor, int tamanho, int pivo )
    	{
    		for( int i = 0; i < tamanho; i++ )
    			if( vetor[ i ] > pivo )
    				return false;
    		return true;
    	}
    
    	/* checa se todos os elementos da particao direita de um vetor sao maior que o pivo */
    	public static boolean particaoDireitaValida( int[] vetor, int tamanho, int pivo )
    	{
    		for( int i = tamanho; i < TAM_VETOR; i++ )
    			if( vetor[ i ] < pivo )
    				return false;
    		return true;
    	}
  	
  	public static void main( String[] args )
  	{
  		TAM_VETOR = 10;
  		vetor = new int[ TAM_VETOR ];
  		
  		/* Teste 1: 'isSorted' retorna true se analisa um vetor ordenado */
  		inicializaVetorOrdenado( vetor );
  		assert isSorted( vetor ) == true : "ERRO --- 'isSorted' deveria retornar true para um vetor ordenado.";
  		/* Fim do Teste 1 */
  		
  		/* Teste 2: 'isSorted' retorna false se analisa um vetor nao ordenado */
  		inicializaVetorNaoOrdenado( vetor );
  		assert isSorted( vetor ) == false : "ERRO --- 'isSorted' deveria retornar false para um vetor nao ordenado.";
  		/* Fim do Teste 2 */
  		
  		/* Teste 3: se a troca de elementos e efetivada */
  		inicializaVetorOrdenado( vetor );
  		int temp1 = vetor[4], temp2 = vetor[8];
  		troca( 4, 8, vetor );
  			
  		assert temp1 == vetor[8] : "ERRO --- 'troca' deveria ter trocado os elementos dos indices dados.";
  		assert temp2 == vetor[4] : "ERRO --- 'troca' deveria ter trocado os elementos dos indices dados.";
  		assert temp1 != vetor[4] : "ERRO --- 'troca' deveria ter trocado os elementos dos indices dados.";
  		assert temp2 != vetor[8] : "ERRO --- 'troca' deveria ter trocado os elementos dos indices dados.";
  		/* Fim do Teste 3 */
  		
  		/* Teste 4: 'indiceElementoMedio' retorna o indice do elemento medio dentre 3 elementos */
  		inicializaVetorOrdenado( vetor );
  		int temp3 = 0, temp4 = 4, temp5 = 9;
  		int medio = indiceElementoMedio( temp3, temp4, temp5 );
  			
  		assert medio != temp3 : "ERRO --- 'indiceElementoMedio' deveria retornar o elemento com valor medio dentre os 3 parametros.";
  		assert medio == temp4 : "ERRO --- 'indiceElementoMedio' deveria retornar o elemento com valor medio dentre os 3 parametros.";
  		assert medio != temp5 : "ERRO --- 'indiceElementoMedio' deveria retornar o elemento com valor medio dentre os 3 parametros.";
  		/* Fim do Teste 4 */
  		
  		/* Teste 5: o subvetor esquerdo resultante do metodo 'partition' somente deve conter elementos menores do que o pivo,
  		   enquanto o direito somente elementos maiores do que o pivo */
  		inicializaVetorNaoOrdenado( vetor );
  		int indicePivo = partition( 0, TAM_VETOR-1, vetor );
  			
  		assert particaoEsquerdaValida( vetor, indicePivo, vetor[indicePivo] ) == true : "ERRO --- 'particao' deveria deixar o subvetor esquerdo somente contendo elementos menores do que o pivo.";
  		assert particaoDireitaValida( vetor, indicePivo, vetor[indicePivo] ) == true : "ERRO --- 'particao' deveria deixar o subvetor direito somente contendo elementos maiores do que o pivo.";
  		/* Fim do Teste 5 */
  		
  	}
}

