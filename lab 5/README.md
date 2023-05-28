# CompConc-lab.5

O código 'prior_leit' implementa a solução sem prioridades;
'prior_escr' implementa a solução com prioridade para threads escritoras;
e 'prior_igual' implementa a solução com prioridades iguais para ambos os tipos de threads.

Para checar se os resultados esperados ocorrem, ou seja, se ocorre inânição como previsto, os códigos originais foram modificados para imprimir o número total de 
execuções de cada tipo de thread. Dessa forma pode ser checado se o código com prioridade para threads escritoras, por exemplo, executa mais vezes esse tipo de thread do que threads leitoras. 
Estes códigos modificados estão na pasta 'resultados'.
