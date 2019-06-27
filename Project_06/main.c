/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

//Bibliotecas utilizadas.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//Declaração do protótipo das funções utilizadas.
double *shuffleValues(double *, int);
int *shuffleTraining(int *);
int *getTest(int *, int *);

//Setar essa variável para true fará com que o programa imprima a época e o erro médio quadrático calculado.
//Utilizada para visualizar o andamento do programa.
bool flow = false;

//Início da função principal.
int main(int argc, char *argv[])
{
  //Lê o valor inserido na linha de comando.
  char *a = argv[1];
  int input = atoi(a);

  //Função utilizada para randomização de algumas variáveis.
  srand(time(NULL));

  FILE *fp;

  //Constante de taxa de aprendizagem e constante de Euler.
  const double learningRate = .5, euler = exp(1.0);

  //Definição das variáveis e arrays utilizados.

  //Variáveis da taxa de acerto, taxa de falsa rejeição e taxa de falsa aceitação.
  double acerto = 0, falsa_rejeicao = 0, falsa_aceitacao = 0;

  //Arrays que armazenarão os indíces das linhas do "features.txt" referentes as imagens de treino e teste,
  //além de variáveis auxiliares.
  int train[25] = {0}, test[25] = {0}, j = 1, aux = 0;

  //Array que armazenará todos os erros calculados em uma época e variável que armazenará o Erro Médio Quadrático ao final
  //de uma época. É inicializado com 1 de forma que passe a verificação de parada na primeira iteração.
  double erro[50] = {0}, minSquareError = 1;

  //Arrays que armazenarão os valores dos gradientes. Cada neurônio possui um gradiente, de forma que cada camada
  //terá o número de gradientes igual ao número de neurônios.
  double gradienteEntrada[536], gradienteOculta[input], gradienteSaida;

  //Arrays que armazenarão os bias. Cada neurônio possui um bia, de forma que cada camada
  //terá o número de bias igual ao número de neurônios.
  double bEntrada[536], bOculta[input], bSaida;

  //Arrays que armazenarão os pesos. O número de pesos de cada neurônio é igual ao número de valores que o neurônio recebe.
  //A camada de entrada possui 536 neurônios e cada um recebe 536 valores. Assim sendo, o número total de pesos na camada
  //de entrada é igual a 536 neurônios * 536 pesos por neurônio. Os primeiros 536 valores são referentes aos pesos do
  //primeiro neurônio, os próximos 536 valores são referentes aos pesos do segundo neurônio e assim por diante.
  //O mesmo para as outras camadas.
  double pesosEntrada[536 * 536], pesosOculta[input * 536], pesosSaida[input];

  //Arrays que armazenarão as saídas. Cada neurônio gera uma única saída, de forma que cada camada
  //terá o número de saídas igual ao número de neurônios.
  double saidaEntrada[536], saidaOculta[input], camadaSaida = 0;

  //Variáveis auxiliares.
  double n = 0, somatorioOculta = 0, somatorioEntrada = 0;

  //O array "train" é preenchido com 25 valores aleatórios que variam de 1 a 50.
  shuffleTraining(train);

  //Preenche o array "test" com os valores de 1 a 50 que não foram utilizados no array "train".
  getTest(train, test);

  //Preenche os array com valores aleatórios variando entre 0 e 1.
  shuffleValues(bEntrada, 536);
  shuffleValues(bOculta, input);
  shuffleValues(pesosEntrada, 536 * 536);
  shuffleValues(pesosOculta, input * 536);

  //Preenche o vetor "pesosSaida" e a variável bSaida com valores aleatórios variando entre 0 e 1.
  for (int i = 0; i < input; i++)
  {
    pesosSaida[i] = rand() / (double)RAND_MAX;
  }
  bSaida = rand() / (double)RAND_MAX;

  //Abre o arquivo "features.txt" para leitura.
  fp = fopen("features.txt", "r");

  if (flow)
    printf("==========TREINAMENTO==========\n");
  //Início do laço de treinamento.
  for (int epoca = 0;; epoca++)
  {
    //Condição de parada.
    if (minSquareError <= 0.2 || epoca > 1000)
    {
      break;
    }

    //Laço de leitura de cada "imagem".
    for (aux = 0; aux < 25;)
    {
      //A variávle "j" é utilizada para certificar que o programa utilizará somente as imagens que foram sorteadas
      //para o treinamento.
      if (j == train[aux])
      {
        //Aloca o array que armazenará os valores de entrada (os valores da linha escolhida aleatoriamente).
        double *camadaEntrada = calloc(536, sizeof(double));
        //Armazena os valores no array.
        for (int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(camadaEntrada + a));
        }

        //FEED FORWARD - ASFALTO

        //Calcula a saída dos neurônios da Camada de Entrada.
        //As três variáveis utilizadas nos laços garantem que os cálculos utilizam somente os valores adequados para cada neurônio.
        for (int i = 0, k = 0; i < (536 * 536); i += 536, k++)
        {
          for (int a = 0; a < 536; a++)
          {
            //Somatório dos valores de entrada multiplicados pelos pesos de um neurônio.
            n += (*(camadaEntrada + a)) * pesosEntrada[i + a];
          }
          //Somando o bias ao resultado do somatório.
          n += bEntrada[k];
          //Passa o somatório+bias pela função de ativação (função logística) e armazena a saída no array "saidaEntrada".
          saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
          n = 0.0;
        }

        //Calcula a saída dos neurônios da Camada Oculta.
        for (int i = 0, k = 0; i < input * 536; i += 536, k++)
        {
          for (int a = 0; a < 536; a++)
          {
            n += saidaEntrada[a] * pesosOculta[i + a];
          }
          n += bOculta[k];
          saidaOculta[k] = (1 / (1 + (pow(euler, -n))));
          n = 0.0;
        }

        //Calcula a saída do único neurônio da Camada de Saída.
        for (int a = 0; a < input; a++)
        {
          camadaSaida += saidaOculta[a] * pesosSaida[a];
        }
        camadaSaida += bSaida;
        camadaSaida = (1 / (1 + (pow(euler, -camadaSaida))));

        //Armazena o erro no array "erro". Como o vetor de entrada representa uma imagem de asfalto,
        //o valor esperado é 0. Portanto, o erro é dado por 0 - saída.
        erro[aux] = 0.0 - camadaSaida;

        //BACK PROPAGATION - ASFALTO

        //Calcula o gradiente de saída utilizando o erro calculado.
        //O gradiente de saída é dado pela derivada da função logística * erro calculado.
        gradienteSaida = (pow(euler, camadaSaida) / pow(pow(euler, camadaSaida) + 1, 2)) * erro[aux];

        //Calcula o gradiente de todos os neurônios da Camada Oculta.
        for (int k = 0; k < input; k++)
        {
          //Para o primeiro neurônio da Camada Oculta, soma o gradiente da saída com o primeiro peso do primeiro neurônio
          //da Camada de Saída e assim por diante. Como a Camada de Saída só possui um neurônio, não é necessário utilizar um laço.
          somatorioOculta += gradienteSaida * pesosSaida[k];
          //O gradiente de cada neurônio da Camada Oculta é dado pela derivada da funçãologística * somatório calculado acima.
          gradienteOculta[k] = (pow(euler, saidaOculta[k]) / pow(pow(euler, saidaOculta[k]) + 1, 2)) * somatorioOculta;
          //Zera o somatório para a próxima iteração.
          somatorioOculta = 0;
        }

        //Calcula o gradiente de todos os neurônios da Camada de Entrada.
        for (int i = 0; i < 536; i++)
        {
          for (int h = 0, b = i; h < input; h++, b += 536)
          {
            //Para o primeiro neurônio da Camada de Entrada, soma o gradiente de cada neurônio da Camada Oculta com o primeiro
            //peso de cada neurônio da Camada Oculta e assim por diante.
            somatorioEntrada += gradienteOculta[h] * pesosOculta[b];
          }
          gradienteEntrada[i] = (pow(euler, saidaEntrada[i]) / pow(pow(euler, saidaEntrada[i]) + 1, 2)) * somatorioEntrada;
          somatorioEntrada = 0;
        }

        //Reajuste dos pesos e bias da Camada de Saída.
        for (int i = 0; i < input; i++)
        {
          //Cada peso é dado por: próprio peso + taxa de aprendizagem * valor referente ao peso (saída da camada anterior) * gradiente.
          pesosSaida[i] = pesosSaida[i] + learningRate * saidaOculta[i] * gradienteSaida;
        }
        //Cada bias é dado por: próprio bias + taxa de aprendizagem * gradiente.
        bSaida = bSaida + learningRate * gradienteSaida;

        //Reajuste dos pesos e bias para a Camada Oculta.
        for (int i = 0, g = 0; i < input * 536; i += 536, g++)
        {
          for (int h = 0; h < 536; h++)
          {
            pesosOculta[i + h] = pesosOculta[i + h] + learningRate * saidaEntrada[h] * gradienteOculta[g];
          }
        }
        for (int i = 0; i < input; i++)
        {
          bOculta[i] = bOculta[i] + learningRate * gradienteOculta[i];
        }

        //Reajuste dos pesos e bias para a Camada de Entrada.
        for (int i = 0, g = 0; i < (536 * 536); i += 536, g++)
        {
          for (int h = 0; h < 536; h++)
          {
            //Como a Camada de Entrada não possui camada anterior, os valores utilizados são os valores de entrada do vetor que
            //representa a imagem.
            pesosEntrada[i + h] = pesosEntrada[i + h] + learningRate * camadaEntrada[h] * gradienteEntrada[g];
          }
        }
        for (int i = 0; i < 536; i++)
        {
          bEntrada[i] = bEntrada[i] + learningRate * gradienteEntrada[i];
        }

        //FEED FORWARD - GRAMA

        //Para pegar os valores da próxima imagem que será de grama, são lidas e descartadas 49 linhas do arquivo "features.txt".
        //Esse processo é realizado devido a estrutura do arquivo: as primeiras 50 linhas são de asfalto, enquanto as 50 útlimas
        //são de grama. Por exemplo: caso a primeira imagem sorteada de asfalto seja a imagem/linha 17. A imagem de grama que será
        //usada, portanto, será a imagem/linha 67. Assim sendo, são descartadas 49 linhas, deixando o ponteiro de leitura do arquivo
        //no início da linha 67, que será lida logo após.
        for (int i = 1; i < 50; i++)
        {
          double *temp = calloc(536, sizeof(double));
          for (int a = 0; a < 536; a++)
          {
            fscanf(fp, "%lf", &*(temp + a));
          }
          free(temp);
        }

        //As operações realizadas no Feed Forward para imagem de grama são semelhantes as de asfalto, com exceção do cálculo do erro.
        for (int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(camadaEntrada + a));
        }
        for (int i = 0, k = 0; i < (536 * 536); i += 536, k++)
        {
          for (int a = 0; a < 536; a++)
          {
            n += (*(camadaEntrada + a)) * pesosEntrada[i + a];
          }
          n += bEntrada[k];
          saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
          n = 0.0;
        }
        for (int i = 0, k = 0; i < input * 536; i += 536, k++)
        {
          for (int a = 0; a < 536; a++)
          {
            n += saidaEntrada[a] * pesosOculta[i + a];
          }
          n += bOculta[k];
          saidaOculta[k] = (1 / (1 + (pow(euler, -n))));
          n = 0;
        }
        for (int a = 0; a < input; a++)
        {
          camadaSaida += saidaOculta[a] * pesosSaida[a];
        }
        camadaSaida += bSaida;
        camadaSaida = (1 / (1 + (pow(euler, -camadaSaida))));
        //Armazena o erro no array "erro". Como o vetor de entrada representa uma imagem de grama,
        //o valor esperado é 1. Portanto, o erro é dado por 1 - saída. Note que o índice utilizado é "aux+25".
        //Fazendo isso, é garantido que um erro não sobrescreverá outro.
        //Ao final de uma época, os primeiros 25 valores serão referentes aos erros do asfalto, enquanto o restante
        //será referente ao erro da grama.
        erro[aux + 25] = 1.0 - camadaSaida;

        //BACK PROPAGATION - GRAMA

        //As operações realizadas no Back Propagation para imagem de grama são semelhantes as de asfalto. Note que o erro passado
        //para o gradiente de saída é o mesmo que acabou de ser calculado, utilizando o mesmo índice.
        gradienteSaida = (pow(euler, camadaSaida) / pow(pow(euler, camadaSaida) + 1, 2)) * erro[aux + 25];

        for (int k = 0; k < input; k++)
        {
          somatorioOculta += gradienteSaida * pesosSaida[k];
          gradienteOculta[k] = (pow(euler, saidaOculta[k]) / pow(pow(euler, saidaOculta[k]) + 1, 2)) * somatorioOculta;
          somatorioOculta = 0;
        }

        for (int i = 0; i < 536; i++)
        {
          for (int h = 0, b = i; h < input; h++, b += 536)
          {
            somatorioEntrada += gradienteOculta[h] * pesosOculta[b];
          }
          gradienteEntrada[i] = (pow(euler, saidaEntrada[i]) / pow(pow(euler, saidaEntrada[i]) + 1, 2)) * somatorioEntrada;
          somatorioEntrada = 0;
        }

        for (int i = 0; i < input; i++)
        {
          pesosSaida[i] = pesosSaida[i] + learningRate * saidaOculta[i] * gradienteSaida;
        }
        bSaida = bSaida + learningRate * gradienteSaida;

        for (int i = 0, g = 0; i < input * 536; i += 536, g++)
        {
          for (int h = 0; h < 536; h++)
          {
            pesosOculta[i + h] = pesosOculta[i + h] + learningRate * saidaEntrada[h] * gradienteOculta[g];
          }
        }
        for (int i = 0; i < input; i++)
        {
          bOculta[i] = bOculta[i] + learningRate * gradienteOculta[i];
        }

        for (int i = 0, g = 0; i < (536 * 536); i += 536, g++)
        {
          for (int h = 0; h < 536; h++)
          {
            pesosEntrada[i + h] = pesosEntrada[i + h] + learningRate * camadaEntrada[h] * gradienteEntrada[g];
          }
        }
        for (int i = 0; i < 536; i++)
        {
          bEntrada[i] = bEntrada[i] + learningRate * gradienteEntrada[i];
        }

        //Libera a memória alocada para o array "camadaEntrada".
        free(camadaEntrada);
        //Retorna o ponteiro de leitura do arquivo para o começo do arquivo.
        rewind(fp);
        //Incrementa "aux" e reseta "j";
        aux++;
        j = 1;
      }
      //Caso a linha atual não seja a linha que representa a imagem escolhida, o programa descarta a linha atual e pula para a
      //próxima, incrementando o "j".
      else
      {
        double *temp = calloc(536, sizeof(double));
        for (int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(temp + a));
        }
        free(temp);
        j++;
      }
    }
    //Reseta "aux" e "j" para a próxima época.
    aux = 0;
    j = 1;

    //Cálculo do Erro Médio Quadrático que é utilizado como critério de parada.
    for (int z = 0; z < 50; z++)
    {
      minSquareError += erro[z] * erro[z];
    }
    minSquareError = minSquareError / 50;
    if (flow)
    {
      printf("Época: %d\nErro Médio Quadrático: %.5lf\n\n", epoca, minSquareError);
    }
  }

  if (flow)
    printf("==========TESTE==========\n");
  //Início do laço de testes.
  //Os cálculos realizados na fase de testes são os mesmos que são realizados na fase de treinamento com duas diferenças.
  //Primeiro, na fase de testes não é feito Back Propagation.
  //Segundo, de acordo com a saída de cada imagem, é incrementado a variável correspondente: acerto, falsa_aceitação ou falsa_rejeição.
  for (j = 1, aux = 0; aux < 25;)
  {
    if (j == test[aux])
    {
      //FEED FORWARD - ASFALTO

      double *camadaEntrada = calloc(536, sizeof(double));
      for (int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada + a));
      }
      for (int i = 0, k = 0; i < (536 * 536); i += 536, k++)
      {
        for (int a = 0; a < 536; a++)
        {
          n += (*(camadaEntrada + a)) * pesosEntrada[i + a];
        }
        n += bEntrada[k];
        saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
        n = 0.0;
      }
      for (int i = 0, k = 0; i < input * 536; i += 536, k++)
      {
        for (int a = 0; a < 536; a++)
        {
          n += saidaEntrada[a] * pesosOculta[i + a];
        }
        n += bOculta[k];
        saidaOculta[k] = (1 / (1 + (pow(euler, -n))));
        n = 0;
      }
      for (int a = 0; a < input; a++)
      {
        camadaSaida += saidaOculta[a] * pesosSaida[a];
      }
      camadaSaida += bSaida;
      camadaSaida = (1 / (1 + (pow(euler, -camadaSaida))));
      //Sabemos que a linha lida representa uma imagem de asfalto. Como o valor ideal para asfalto é 0, verifica-se se a saída
      //é menor ou igual a 0.5 (margem de erro). Caso positivo, é incrementada a taxa de acerto.
      //Caso negativo, o sistema classificou uma imagem de asfalto como grama. Dessa forma, é incrementada a taxa de falsa aceitação.
      if (camadaSaida <= 0.5)
      {
        acerto++;
      }
      else
      {
        falsa_aceitacao++;
      }

      //FEED FORWARD - GRAMA

      for (int i = 1; i < 50; i++)
      {
        double *temp = calloc(536, sizeof(double));
        for (int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(temp + a));
        }
        free(temp);
      }
      for (int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada + a));
      }
      for (int i = 0, k = 0; i < (536 * 536); i += 536, k++)
      {
        for (int a = 0; a < 536; a++)
        {
          n += (*(camadaEntrada + a)) * pesosEntrada[i + a];
        }
        n += bEntrada[k];
        saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
        n = 0.0;
      }
      for (int i = 0, k = 0; i < input * 536; i += 536, k++)
      {
        for (int a = 0; a < 536; a++)
        {
          n += saidaEntrada[a] * pesosOculta[i + a];
        }
        n += bOculta[k];
        saidaOculta[k] = (1 / (1 + (pow(euler, -n))));
        n = 0;
      }
      for (int a = 0; a < input; a++)
      {
        camadaSaida += saidaOculta[a] * pesosSaida[a];
      }
      camadaSaida += bSaida;
      camadaSaida = (1 / (1 + (pow(euler, -camadaSaida))));
      //Sabemos que a linha lida representa uma imagem de grama. Como o valor ideal para grama é 1, verifica-se se a saída
      //é maior ou igual a 0.5 (margem de erro). Caso positivo, é incrementada a taxa de acerto.
      //Caso negativo, o sistema classificou uma imagem de grama como asfalto. Dessa forma, é incrementada a taxa de falsa rejeição.
      if (camadaSaida > 0.5)
      {
        acerto++;
      }
      else
      {
        falsa_rejeicao++;
      }

      free(camadaEntrada);
      rewind(fp);
      aux++;
      j = 1;
    }
    else
    {
      double *temp = calloc(536, sizeof(double));
      for (int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(temp + a));
      }
      free(temp);
      j++;
    }
  }

  //Multiplicando as taxas para chegar a porcentagem.
  acerto *= 2.0;
  falsa_rejeicao *= 2.0;
  falsa_aceitacao *= 2.0;
  //Impressão dos resultados.
  printf("Taxa de acerto: %.1lf%%\nTaxa de falsa rejeição: %.1lf%%\nTaxa de falsa aceitação: %.1lf%%\n", acerto, falsa_rejeicao, falsa_aceitacao);
  //Fecha o arquivo.
  fclose(fp);
  //Finaliza o programa.
  return 0;
}

//Função que gera valores aleatórios entre 0 e 1 e armazena no array fornecido.
double *shuffleValues(double *pesos, int limit)
{
  for (int i = 0; i < limit; i++)
  {
    pesos[i] = rand() / (double)RAND_MAX;
  }
  return pesos;
}

//Função que gera 25 valores aleatórios de 1 a 50 e armazena no vetor fornecido.
int *shuffleTraining(int *vetor)
{
  for (int i = 1; i <= 50; i++)
  {
    vetor[i - 1] = i;
  }
  for (int i = 0; i < 50; i++)
  {
    int temp = vetor[i];
    int randomIndex = rand() % 50;
    vetor[i] = vetor[randomIndex];
    vetor[randomIndex] = temp;
  }
  return vetor;
}

//Função que gera os valores de 1 a 50 que não foram utilizados no primeiro vetor e armazena no segundo vetor.
int *getTest(int *train, int *test)
{
  for (int i = 1, g = 0; i <= 50; i++)
  {
    for (int h = 0; h < 25; h++)
    {
      if (train[h] == i)
      {
        i++;
        h = 0;
        continue;
      }
      else if (h == 24)
      {
        *(test + g) = i;
        g++;
        break;
      }
    }
  }
  return test;
}