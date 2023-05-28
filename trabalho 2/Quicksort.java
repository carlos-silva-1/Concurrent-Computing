import java.io.*;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class Quicksort extends RecursiveTask<Integer>
{ 
	int esq, dir; //indices dos limites do intervalo sendo particionado
	
	static int[] vetor; //vetor a ser ordenado. caso seja uma chamada recursiva, subvetor do original
	static int TAM_VETOR; //tamanho do vetor. dado pelo usuario
	static int numThreads; //numero de threads. dado pelo usuario
	
	/* checa quantos processadores a maquina possui, e define esse numero como 
	   o maximo numero de threads que podem ser utilizadas */
	static final int NUM_MAX_THREADS = Runtime.getRuntime().availableProcessors();
	
	/* Construtor */
    	public Quicksort( int esq, int dir, int[] vetor )
    	{
        	this.vetor = vetor;
        	this.esq = esq;
        	this.dir = dir;
    	}
  	
  	/*retorna true se cada elemento do vetor e menor ou igual ao proximo
  	  se nao, retorna false */
  	public static boolean isSorted( int[] vetor )
  	{
  		for( int i = 0; i < TAM_VETOR-1; i++ )
  			if( vetor[ i ] > vetor[ i+1 ] )
  				return false;
  		return true;
  	}
  	
  	/* recebe dois indices e troca o elemento presente no indice1 
  	   pelo elemento no indice2, e vice-versa */
  	public void troca( int indice1, int indice2, int[] vetor )
  	{
  		int temp = vetor[ indice1 ];
  		vetor[ indice1 ] = vetor[ indice2 ];
  		vetor[ indice2 ] = temp;
  	}
  	
  	/* recebe 3 indices do vetor e retorna o indice no qual o elemento
  	   medio esta presente.
  	   usada para encontrar um pivo com uma chance maior de estar 
  	   no meio do vetor apos a particao */
  	public int indiceElementoMedio( int a, int b, int c )
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

    	/* particiona o intervalo [esq, dir] em torno do pivo
      	   antes do pivo ficam todos os elementos menores que ele, e depois 
      	   do pivo, todos os maiores.
      	   retorna a posicao do pivo no vetor final */
   	public int partition( int esq, int dir, int[] vetor ) 
    	{
        	int i = esq, j = dir;
  
        	/* elemento medio de uma amostra do vetor escolhido como pivo */
        	int pivo = indiceElementoMedio( esq, (esq+dir)/2, dir );

        	/* troca o pivo com o elemento na posicao final do vetor */
        	troca( j, pivo, vetor );
        	j--; 
  
        	while (i <= j)
        	{
        		/*enquanto o ponteiro que percorre pela esquerda nao encontra um 
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
  
  	/* particiona o vetor e recursivamente particiona um dos subvetores resultantes
  	   enquanto deixa a tarefa de particionar o outro subvetor na pool */
    	@Override
    	protected Integer compute()
    	{
    		/* se os ponteiros esquerdo e direito se cruzam, a particao ja esta pronta */
        	if( esq >= dir )
            		return null;

        	int p = partition( esq, dir, vetor );
  
        	/* divide o vetor em suas particoes esquerda [esq, p-] e direita [p+1, dir] */
        	Quicksort particaoEsquerda = new Quicksort( esq, p - 1, vetor );
        	Quicksort particaoDireita = new Quicksort( p + 1, dir, vetor );

        	/* particao do subvetor esquerdo do elemento pivo separado como 
           	   tarefa a ser executada na pool de threads */
        	particaoEsquerda.fork();
        
        	/* recursao no subvetor que nao foi enviado a pool de threads */
        	particaoDireita.compute();
  
        	/* espera o termino da particao do subvetor esquerdo */
        	particaoEsquerda.join();

        	return null;
    	}
    
    	/* inicializa cada elemento do vetor com um numero aleatorio na escala [1, 1000] */
   	public static void inicializaAleatoriamente( int[] vetor )
    	{
    		for( int i = 0; i < TAM_VETOR; i++ )
    			vetor[ i ] = (int)Math.floor( Math.random() * 1000 + 1 );
    	}
    
    	/* imprime o vetor, somente imprimindo 'new line' depois de imprimir o ultimo elemento */
    	public static void printVetor( int[] vetor )
    	{
    		for( int i = 0; i < TAM_VETOR; i++ )
            		System.out.print( vetor[i] + " " );
        	System.out.println();
    	}

    	public static void main( String args[] )
    	{
    		//caso pedido, imprime exemplos de chamadas corretas e termina o programa
    		if( args.length == 1 && args[0].equals("-exemplo") )
    		{
    			System.out.println( Mensagens.msg_exemplo );
    			return;
    		}
    		//caso pedido, imprime a mensagem auxiliar e termina o programa
    		if( args.length == 1 && args[0].equals("-ajuda") )
    		{
    			System.out.println( Mensagens.msg_ajuda );
    			return;
    		}
    	
    		try
    		{
    			TAM_VETOR = Integer.parseInt( args[0] );
    			numThreads = Integer.parseInt( args[1] );
    		}
    		/* excecao se argumentos passados na linha de comando nao sao inteiros */
    		catch( NumberFormatException e ) 
    		{
    			System.out.println( Mensagens.msg_arg_incorreto );
    			return;
    		}
    		/* excecao se nao foi passado o numero correto de argumentos */
    		catch( ArrayIndexOutOfBoundsException e ) 
    		{
    			System.out.println( Mensagens.msg_chamada_incorreta );
    			return;
    		}
    	
    		/* se o numero de threads dado pelo usuario foi maior do que a maquina suporta,
    	  	   modifica para o numero maximo de threads suportado pelo computador, ja que 
    	   	   um numero maior nao traria ganhos */
    		if( numThreads > NUM_MAX_THREADS )
    			numThreads = NUM_MAX_THREADS;
    		/* se o tamanho do vetor dado pelo usuario foi menor ou igual a 8192,
    	   	   modifica o numero de threads para 1, ja que para vetores pequenos o 
    	   	   overhead por usar multiplas threads causa mais demora */
    		if( TAM_VETOR <= 8192 )
    			numThreads = 1;

        	vetor = new int[ TAM_VETOR ];
        	inicializaAleatoriamente( vetor );

        	/* cria uma thread pool com 'numThreads' threads */
        	ForkJoinPool pool = new ForkJoinPool( numThreads );
  
  		long startTime = System.nanoTime(); //comeca a medir tempo de execucao
        	pool.invoke( new Quicksort(0, TAM_VETOR-1, vetor) ); //comeca o processo de ordenacao
        	long endTime = System.nanoTime(); //termina de medir tempo de execucao
        	System.out.println( endTime - startTime ); //imprime o tempo levado
        
        	if( isSorted( vetor ) ) //imprime se a ordenacao sucedeu
        		System.out.println( "Vetor foi ordenado com sucesso." );
    	}
} 
