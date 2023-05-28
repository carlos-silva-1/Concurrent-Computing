class Buffer
{
	private int[] buffer;
	private int front, back;
	
	/*	Construtor
		Para indicar que esta vazio, ambos os ponteiros apontam para -1 */
	public Buffer( int size )
	{
		this.buffer = new int[ size ];
		this.front = -1;
		this.back = -1;
	}
	
	public synchronized void printBuffer()
	{
		for( int i = 0; i < this.buffer.length; i++ )
			System.out.printf( "[ %d ]: %d\t", i, buffer[ i ] );
		System.out.println();
	}
	
	private synchronized boolean isFull()
	{
		if( this.front == this.back && this.front != -1 )
			return true;
		return false;
	}
	
	private synchronized boolean isEmpty()
	{
		if( this.front == -1 && this.back == -1 )
			return true;
		return false;
	}
	
	public synchronized void pushBack( int num, int id )
	{
		try
		{
			while( isFull() )
			{
				System.out.printf( "pc.bufferCheio()\n" );
				System.out.printf( "pc.produtorBloqueado(%d)\n", id );
				wait();
			}
			
			System.out.printf( "pc.produtorProduzindo(%d, %d)\n", id, num );
			
			if( isEmpty() ) //se for primeira insercao 'back' vai de -1 para 1
			{
				this.front = 0;
				this.back = 1;
				this.buffer[ this.front ] = num;
			}
			else
			{
				this.buffer[ this.back ] = num;
				this.back = (this.back+1)%this.buffer.length;
			}
			
			if( !isEmpty() )
				notifyAll();
		}
		catch( InterruptedException e ) {}
	}
	
	public synchronized void popFront( int id )
	{
		try
		{
			while( isEmpty() )
			{
				System.out.printf( "pc.bufferVazio()\n" );
				System.out.printf( "pc.consumidorBloqueado(%d)\n", id );
				wait();
			}
				
			int removido = this.buffer[ this.front ];
			System.out.printf( "pc.consumidorConsumindo(%d, %d)\n", id, removido );
			
			this.buffer[ this.front ] = 0;
			this.front = (this.front+1)%this.buffer.length;
			
			if( this.front == this.back ) //se consumido era ultimo elemento, indica que buffer esta vazio
			{
				this.front = -1;
				this.back = -1;
			}
			
			if( !isFull() )
				notifyAll();
		}
		catch( InterruptedException e ) {}
	}
}

