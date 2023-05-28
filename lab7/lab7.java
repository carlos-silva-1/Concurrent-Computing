import java.lang.Math;

class Consumidor extends Thread
{
	int id;
	Buffer monitor;
	
	public Consumidor( int id, Buffer monitor )
	{
		this.id = id;
		this.monitor = monitor;
	}
	
	public void run()
	{
		try
		{
			while( true )
			{
				this.monitor.popFront( id );
				sleep( 1 );
			}
		}
		catch( InterruptedException e ) {}
	}
}

class Produtor extends Thread
{
	int id;
	Buffer monitor;
	
	public Produtor( int id, Buffer monitor )
	{
		this.id = id;
		this.monitor = monitor;
	}
	
	public void run()
	{
		try
		{
			while( true )
			{
				//insere um valor aleatorio no intervalo [1, 10]
				this.monitor.pushBack( (int)Math.floor( Math.random() * 10 + 1 ), id );
				sleep( 1 );
			}
		}
		catch( InterruptedException e ) {}
	}
}

class ProdCons
{
	static final int TAM_BUFFER = 10;
	static final int N_CONSUMIDOR = 2; //numero de threads consumidoras
	static final int N_PRODUTOR = 3; //numero de threads produtoras

	public static void main ( String[] args )
	{
		Buffer buf = new Buffer( TAM_BUFFER );
		
		Produtor[] prod = new Produtor[ N_PRODUTOR ];
		Consumidor[] cons = new Consumidor[ N_CONSUMIDOR ];
		
		//inicia o log de saida
		System.out.println ( "import verificaProdCons" );
		System.out.println ( "pc = verificaProdCons.PC()" );
		System.out.println ( "pc.tamanho = " + TAM_BUFFER );
		System.out.println ( "pc.posicoesLivres = " + TAM_BUFFER );
		
		for( int i = 0; i < N_CONSUMIDOR; i++ )
		{
			cons[ i ] = new Consumidor( i, buf );
			cons[ i ].start(); 
		}
		for( int i = 0; i < N_PRODUTOR; i++ )
		{
			prod[ i ] = new Produtor( i, buf );
			prod[ i ].start();
		}
	}
}

