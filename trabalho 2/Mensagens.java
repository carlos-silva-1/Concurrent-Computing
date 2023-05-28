class Mensagens
{
	static String msg_chamada_incorreta = "=====   ERRO CHAMADA INCORRETA  ===== \n\nChamada correta para o programa: java Quicksort <tamanho do vetor> <numero de threads> \n\nO tamanho do vetor deve ser um inteiro positivo, assim como o numero de threads. \nCaso queira ver exemplos de chamadas em formato correto, digite: java Quicksort -exemplo\n\nCaso queira uma explicacao mais detalhada, digite: java Quicksort -ajuda\n";
	
	static String msg_exemplo = "===== EXEMPLOS DE CHAMADAS CORRETAS ===== \n\njava Quicksort 250000 4 \n\njava Quicksort 1000000 2 \n\nNo primeiro exemplo 250000 e o tamanho do vetor e 4 e o numero de threads. \nNo segundo exemplo 1000000 e o tamanho do vetor e 2 e o numero de threads.\n";
	
	static String msg_ajuda = "O programa ordena um vetor seguindo o algoritmo 'quicksort' de forma paralela, imprime o tempo de execucao levado, alem de confirmar se a ordenacao funcionou.\n\nPara ver como a chamada correta do programa e feita, digite: java Quicksort -exemplo \n\nCaso ainda haja duvidas sobre o algoritmo ou sua implementacao, leia a seguinte secao do relatorio: 1 - Algoritmo e Implementacao.";
	
	static String msg_arg_incorreto = "=====   ERRO ARGUMENTO INCORRETO  ===== \n\nUm dos argumentos passados ao programa nao e um numero. Ambos o primeiro argumento (tamanho do vetor) e o segundo argumento (numero de threads) devem ser inteiros.";
}
