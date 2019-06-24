Estrutura de Dados 1 - Turma A/2019.1
Trabalho 06
Integrantes:
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200

Instruções para rodar o programa:

No Ubuntu, sistema operacional utilizado para desenvolver o código, vá até a pasta contendo o main.c pelo terminal.
Já na pasta, rode os seguintes comandos:

"gcc main.c -lm"
"./a.out"

Observações:

A flag -lm é utilizada para compilar a biblioteca "math.h", necessária para o programa.

O arquivo "features.txt" deve ficar na mesma pasta que o main.c e contém todos os vetores que serão utilizados no treinamento
e teste da rede neural.
Dentro do arquivo, as linhas de 1 a 50 são os vetores de asfalto, enquanto as linhas de 51 a 100 são os vetores de grama.
Cada linha representa um vetor de 536, onde os primeiros 512 valores são referentes ao ILBP de cada imagem,
enquanto os últimos 24 valores são referentes ao GLCM.
Todos os vetores estão normalizados.

O programa leva alguns minutos para executar, variando de acordo com a capacidade da máquina.