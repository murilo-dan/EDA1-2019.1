/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

//Bibliotecas utilizadas.
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

double *shufflePesos(double *, int, int);
double *shuffleBias(double *, int);
int *shuffleTraining(int *);
void debug();
//Declaração do protótipo das funções utilizadas.

//Início da função principal.
int main(int argc, char *argv[])
{
  char *a = argv[1];
  int input = atoi(a);
  //Função utilizada para randomização de algumas variáveis.
  srand(time(NULL));
  FILE *fp;
  double minSquareError = 0;
  double erro[50] = {0};
  double gradienteEntrada[536], gradienteOculta[input], gradienteSaida = 0;
  const double learningRate = 0.5;
  int random[25] = {0};
  double bEntrada[536], bOculta[input], bSaida;
  double pesosEntrada[287296], pesosOculta[input * 536], pesosSaida[input];
  shuffleTraining(random);
  fp = fopen("features.txt", "r");
  int j = 0, aux = 0;
  double saidaEntrada[536], saidaOculta[input], camadaSaida = 0, n, euler = 2.718281828459045235360287;
  for (int epoca = 0; epoca < 1000 || minSquareError <= 0.2; epoca++)
  {
    while (aux < 25)
    {
      if (j == random[aux])
      {
        double *camadaEntrada = calloc(536, sizeof(double));
        for (int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(camadaEntrada + a));
        }
        if (aux == 0)
          shuffleBias(bEntrada, 536);
        for (int i = 0, k = 0; i < 287296; i += 536, k++)
        {
          if (aux == 0)
            shufflePesos(pesosEntrada, i, 536);
          for (int a = 0; a < 536; a++)
          {
            n += *(camadaEntrada + a) * pesosEntrada[i + a];
          }
          n += bEntrada[k];
          saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
          n = 0;
        }
        if (aux == 0)
          shuffleBias(bOculta, input);
        for (int i = 0, k = 0; i < input * 536; i += 536, k++)
        {
          if (aux == 0)
            shufflePesos(pesosOculta, i, 536);
          for (int a = 0; a < 536; a++)
          {
            n += saidaEntrada[a] * pesosOculta[i + a];
          }
          n += bOculta[k];
          saidaOculta[k] = (1 / (1 + (pow(euler, -n))));
          n = 0;
        }
        if (aux == 0)
        {
          for (int i = 0; i < input; i++)
          {
            pesosSaida[i] = rand() % 15000;
          }
          bSaida = rand() % 15000;
        }
        for (int a = 0; a < input; a++)
        {
          camadaSaida += saidaOculta[a] * pesosSaida[a];
        }
        camadaSaida += bSaida;
        camadaSaida = (1 / (1 + (pow(euler, -camadaSaida))));
        erro[aux] = 0 - camadaSaida;

        //BACK PROPAGATION
        gradienteSaida = (pow(euler, camadaSaida) / pow((pow(euler, camadaSaida) + 1), 2)) * erro[aux];
        double somatorioOculta = 0, somatorioEntrada = 0;
        for (int i = 0; i < input; i++)
        {
          somatorioOculta += gradienteSaida * pesosSaida[i];
        }
        for (int i = 0; i < input; i++)
        {
          gradienteOculta[i] = (pow(euler, saidaOculta[i]) / pow((pow(euler, saidaOculta[i]) + 1), 2)) * somatorioOculta;
        }
        for (int i = 0, h = 0; i < input; i++)
        {
          for (; h < input * 536; h++)
          {
            somatorioEntrada += gradienteOculta[i] * pesosOculta[h];
          }
        }
        for (int i = 0; i < 536; i++)
        {
          gradienteEntrada[i] = (pow(euler, saidaEntrada[i]) / pow((pow(euler, saidaEntrada[i]) + 1), 2)) * somatorioEntrada;
        }
        somatorioOculta = 0;
        somatorioEntrada = 0;
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

        for (int i = 0, g = 0; i < 287296; i += 536, g++)
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

        for (int i = 0; i < 50; i++)
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
        for (int i = 0, k = 0; i < 287296; i += 536, k++)
        {
          for (int a = 0; a < 536; a++)
          {
            n += *(camadaEntrada + a) * pesosEntrada[i + a];
          }
          n += bEntrada[k];
          saidaEntrada[k] = (1 / (1 + (pow(euler, -n))));
          n = 0;
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
        erro[aux + 25] = 1 - camadaSaida;

        //BACK PROPAGATION
        gradienteSaida = (pow(euler, camadaSaida) / pow((pow(euler, camadaSaida) + 1), 2)) * erro[aux + 25];
        for (int i = 0; i < input; i++)
        {
          somatorioOculta += gradienteSaida * pesosSaida[i];
        }
        for (int i = 0; i < input; i++)
        {
          gradienteOculta[i] = (pow(euler, saidaOculta[i]) / pow((pow(euler, saidaOculta[i]) + 1), 2)) * somatorioOculta;
        }
        for (int i = 0, h = 0; i < input; i++)
        {
          for (; h < input * 536; h++)
          {
            somatorioEntrada += gradienteOculta[i] * pesosOculta[h];
          }
        }
        for (int i = 0; i < 536; i++)
        {
          gradienteEntrada[i] = (pow(euler, saidaEntrada[i]) / pow((pow(euler, saidaEntrada[i]) + 1), 2)) * somatorioEntrada;
        }
        somatorioOculta = 0;
        somatorioEntrada = 0;
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

        for (int i = 0, g = 0; i < 287296; i += 536, g++)
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
        free(camadaEntrada);
        rewind(fp);
        j = 0;
        aux++;
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
    aux = 0;
    j = 0;
    shuffleTraining(random);
    for (int z = 0; z < 50; z++)
    {
      minSquareError += erro[z];
    }
    minSquareError *= minSquareError;
    minSquareError /= 50;
  }
  fclose(fp);
  return 0;
}

double *shufflePesos(double *pesos, int current, int limit)
{
  for (int i = 0; i < limit; i++)
  {
    pesos[current + i] = rand() % 15000;
  }
  return pesos;
}

double *shuffleBias(double *bias, int limit)
{
  for (int i = 0; i < limit; i++)
  {
    bias[i] = rand() % 15000;
  }
  return bias;
}

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

void debug()
{
  printf("TESTE\n");
  return;
}