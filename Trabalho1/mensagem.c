char* msg_ajuda = "O programa calcula a transposta de uma matriz quadrada de forma sequencial e paralela, e imprime o tempo de exeucao de cada forma com o objetivo de medir o ganho de desmpenho.\n\nA chamada do programa é feita da seguinte forma: transposta <tamanho da matriz> <numero de threads>. \nO tamanho da matriz deve ser um inteiro positivo, assim como o numero de threads.\nCaso deseje ver exemplos de chamadas para o programa, digite: transposta -exemplo\n\nCaso ainda haja duvidas sobre o algoritmo ou sua implementacao, leia a seguinte secao do relatorio: 1 - Problema e Algoritmo";

char* msg_exemplo = "Exemplos de chamadas corretas:\ntransposta 1000 1\ntransposta 1000 4\ntransposta 8000 3\ntransposta 1 99\n\nExemplos de chamadas incorretas:\ntransposta\ntransposta 1000\ntransposta 1000 2 4\ntransposta 2000 4.5\n";

char* chamada_incorreta = "Chamada correta para o programa: transposta <tamanho da matriz> <numero de threads> \nCaso queira ver exemplos de chamadas em formato correto, digite: transposta -exemplo\nCaso queira uma explicacao mais detalhada, digite: transposta -ajuda";

char* arg_incorreto = "Argumentos devem ser numeros inteiros positivos. \nChamada correta para o programa: transposta <tamanho da matriz> <numero de threads> \nCaso queira ver exemplos de chamadas em formato correto, digite: transposta -exemplo\nCaso queira uma explicacao mais detalhada, digite: transposta -ajuda";

