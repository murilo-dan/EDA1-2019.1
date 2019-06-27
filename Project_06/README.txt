Estrutura de Dados 1 - Turma A/2019.1
Trabalho 06
Integrantes:
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200

Instruções para rodar o programa:

No Ubuntu, sistema operacional utilizado para desenvolver o código, vá até a pasta contendo o main.c pelo terminal.
Já na pasta, rode os seguintes comandos:

"gcc main.c -lm"
"./a.out x"

Observações:

A variável "x" no comando de execução do código é obrigatória e deve ser maior que zero e define a quantidade de neurônios 
da camada oculta. Utilizar valores mais altos implica em um maior número de operações e, possivelmente, maior tempo de execução.

A flag -lm é utilizada para compilar a biblioteca "math.h", necessária para o programa.

O arquivo "features.txt" deve ficar na mesma pasta que o main.c e contém todos os vetores que serão utilizados no treinamento
e teste da rede neural.
Esse arquivo foi gerado ao realizar algumas modificações e executar o Projeto 2, desenvolvido pelos mesmos autores deste projeto 
e disponível no GitHub "https://github.com/murilo-dan/EDA1-2019.1/tree/master/Project_02".
Dentro do arquivo, as linhas de 1 a 50 são os vetores de asfalto, enquanto as linhas de 51 a 100 são os vetores de grama.
Cada linha representa um vetor de 536, onde os primeiros 512 valores são referentes ao ILBP de cada imagem,
enquanto os últimos 24 valores são referentes ao GLCM.
Todos os vetores estão normalizados.

Na linha 21 do código há uma variável booleana com o nome "flow", inicialmente setada para false.
Setar a variável para true fará com que o código imprima no terminal a época atual e cada erro médio quadrático calculado,
além de informar qual parte do programa está sendo executada (treinamento ou teste).