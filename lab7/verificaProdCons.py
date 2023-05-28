#implementa o mesmo buffer do original como vetor e todas as operacoes realizadas no original sao aplicadas nesse.
#assim pode-se checar se os elementos sendo consumidos sao os elementos corretos
class PC:
	def __init__( self ):
		self.cheio = False
		self.vazio = True
		self.tamanho = 0
		self.posicoesLivres = 0
		self.buffer = []

	# um produtor é bloquado quando o buffer ja esta cheio
	def produtorBloqueado( self,id ):
		'''Recebe o id do produtor. Verifica se a decisao de bloqueio esta correta.'''
		if( self.cheio == False ):
			print( "ERRO: produtor " + str(id) + " bloqueado quando buffer nao esta cheio!" )

	# um consumidor é bloquado quando o buffer esta vazio
	def consumidorBloqueado( self,id ): 
		'''Recebe o id do consumidor. Verifica se a decisao de bloqueio esta correta.'''
		if( self.vazio == False ):
			print( "ERRO: consumidor " + str(id) + " bloqueado quando buffer nao esta vazio!" )

	# se o buffer ja esta cheio, produtor deveria ter sido bloqueado
	# se o buffer nao esta cheio, atualiza o estado das variaveis para refletir a insercao
	def produtorProduzindo( self,id,numInserido ):
		'''Recebe o id do produtor, verifica se pode produzir e registra que produziu.'''
		if( self.cheio == True ):
			print( "ERRO: produtor " + str(id) + " esta produzindo quando buffer ja esta cheio!" )
		self.buffer.append( numInserido )
		self.posicoesLivres -= 1
		if( self.posicoesLivres == 0 ):
			self.cheio = True
		self.vazio = False

	# se o buffer ja esta vazio, consumidor deveria ter sido bloqueado
	# se o buffer nao esta vazio, atualiza o estado das variaveis para refletir a remocao
	def consumidorConsumindo( self,id,numRemovido ):
		'''Recebe o id do consumidor, verifica se pode consumir e registra que consumiu.'''
		if( self.vazio == True ):
			print("ERRO: consumidor " + str(id) + " esta consumindo quando buffer ja esta vazio!")
		removed = self.buffer.pop( 0 )
		if( numRemovido != removed ):
			print("ERRO: elemento removido deveria ter sido " + str(numRemovido) + " mas foi " + str(removed) )
		self.posicoesLivres += 1
		if( self.posicoesLivres == self.tamanho ):
			self.vazio = True
		self.cheio = False
		
	def bufferCheio( self ):
		'''Chamada se todas as posicoes do buffer estao ocupadas.'''
		if( self.posicoesLivres > 0 ):
			print("ERRO: buffer nao esta cheio!")
		
	def bufferVazio( self ):
		'''Chamada se todas as posicoes do buffer estao vazias.'''
		if( self.posicoesLivres < self.tamanho ):
			print("ERRO: buffer nao esta vazio!")

