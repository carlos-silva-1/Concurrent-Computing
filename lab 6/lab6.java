class Vetor
{
	private int[] vet;
	private int tam; //tamanho do vetor

	//construtor
	public Vetor( int tam )
	{ 
		this.vet = new int[ tam ];
		this.tam = tam;
	}

	//inicializa todo o vetor para 'valor'
	public void inicializaVetor( int valor )
	{
		for( int i = 0; i < this.tam; i++ )
			vet[ i ] = valor;
	}

	public void imprimeVetor()
	{
		for( int i = 0; i < this.tam; i++ )
			System.out.printf( "%d ", vet[ i ] );
			
		System.out.println();
	}

	//retorna o tamanho do vetor
	public int getLength()
	{
		return this.tam;
	}
	
	//retorna o elemento no indice passado como argumento
	public int getElement( int indice )
	{
		return this.vet[ indice ];
	}
	
	//modifica o elemento no indice para o arg valor
	public void setElement( int indice, int valor )
	{
		this.vet[ indice ] = valor;
	}
}

//Soma os vetores A & B no vetor C 
class Somador extends Thread
{
	private int id; //identificador da thread
  
  	//construtor
	public Somador ( int tid )
	{ 
		this.id = tid; 
	}

	public void run()
	{
		System.out.println( "Thread " + this.id + " iniciou." );
		for( int i = id; i < somaVetor.TAM_VETOR; i += somaVetor.N_THREADS )
		{
			//C[ i ] = A[ i ] + B[ i ]
			somaVetor.C.setElement( i, somaVetor.A.getElement( i ) + somaVetor.B.getElement( i ) );
		}
		System.out.println( "Thread " + this.id + " terminou." ); 
	}
}

//classe da aplicacao
class somaVetor
{
	static final int N_THREADS = 4;
	static final int TAM_VETOR = 100;
	
	static Vetor A;
	static Vetor B;
	static Vetor C;

	public static void main ( String[] args )
	{
		//reserva espaço para um vetor de threads
		Thread[] threads = new Thread[ N_THREADS ];
		
		A = new Vetor( TAM_VETOR );
		B = new Vetor( TAM_VETOR );
		C = new Vetor( TAM_VETOR );
		
		A.inicializaVetor( 3215 );
		B.inicializaVetor( 2144 );
		C.inicializaVetor( 0 );
		
		//imprime os vetores A & B
		System.out.println( "Vetores a ser somados:" );
		System.out.println( "Vetor A:" );
		A.imprimeVetor();
		System.out.println( "Vetor B:" );
		B.imprimeVetor();
		
		//cria as threads da aplicacao
		for( int i = 0; i < threads.length; i++ )
		{
			threads[ i ] = new Somador( i );
		}

		//inicia as threads
		for( int i=0; i<threads.length; i++ )
		{
			threads[i].start();
		}

		//espera pelo termino de todas as threads
		for( int i=0; i<threads.length; i++ )
		{
			try { threads[i].join(); } 
			catch ( InterruptedException e ) { return; }
		}

		System.out.println( "Vetor resultado:" ); 
		C.imprimeVetor();
	}
}
