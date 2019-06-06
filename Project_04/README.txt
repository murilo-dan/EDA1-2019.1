Estrutura de Dados 1 - Turma A/2019.1
Trabalho 04
Integrantes:
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200

Instruções para rodar o programa:

No Ubuntu, sistema operacional utilizado para desenvolver o código, vá até a pasta contendo o main.c pelo terminal.
Já na pasta, rode os seguintes comandos sem as aspas:

"gcc main.c"
"./a.out"

Observações:

O código utiliza o horário atual do sistema como "seed" para gerar a randomização do programa.
Assim sendo, rodar o código em rápida sucessão pode gerar saídas iguais, dado que o retorno da função
que utiliza o tempo do sistema não é modificado. 
É recomendado esperar cerca de 1 segundo entre cada execução do programa para evitar saídas iguais.

Não é recomendado alterar o valor da variável "globalTime". Apesar de o código tratar a passagem de dia,
ele não cobre todos os casos. O programa não dará erro, mas a utilização eficiente das pistas pode falhar.

Para as operações de aproximação/pouso, o horário inicial de procedimento foi considerado como o horário de aproximação.