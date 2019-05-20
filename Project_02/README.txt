Estrutura de Dados 1 - Turma A/2019.1
Trabalho 02
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

Certifique-se de que as pastas ("asphalt" e "grass") contendo os .txt das imagens estão dentro de uma pasta com o nome "DataSet".
A pasta "DataSet" deve estar no mesmo local que o main.c.

O programa leva alguns minutos para executar, variando de acordo com a capacidade da máquina.

Na linha 42 do código há uma variável booleana com o nome "flow", inicialmente setada para false.
Setar a variável para true fará com que o código imprima no terminal o nome de cada arquivo que está sendo lido,
além de informar qual parte do programa está sendo executada (treinamento ou teste).
