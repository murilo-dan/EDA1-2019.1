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
//Declaração do protótipo das funções utilizadas.


//Início da função principal.
int main(int argc, char *argv[])
{
  char *a = argv[1];
  int input = atoi(a);
  //Função utilizada para randomização de algumas variáveis.
  srand(time(NULL));
  FILE *fp;
  double asphaltTraining[25];
  double grassTraining[25];
  double erro = 0;
  double gradienteEntrada[536], gradienteOculta[input], gradienteSaida = 0;
  double learningRate;
  int random[25] = {0};
  double camadaSaida = 0.0;
  double bEntrada[536], bOculta[input], bSaida;
  double pesosEntrada[287296], pesosOculta[input*2], pesosSaida[input];
  shuffleTraining(random);
  fp = fopen("features.txt", "r");
  int j = 0, aux = 0;
  double saidaAsphalt[536], saidaAsphaltOculta[input], saidaGrass[536], n, euler = 2.718281828459045235360287;
  for(int i = 0; i<25; i++){
    printf("%d\n",random[i]);
  }
  while(aux<25)
  {
    if(j == random[aux] && aux==0)
    {
      double* camadaEntrada = calloc(536, sizeof(double));
      for(int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada+a));
      }
      shuffleBias(bEntrada, 536);
      for(int i = 0; i<536; i++)
      {
      shufflePesos(pesosEntrada, i+536, 536);
        for(int a = 0; a<536; a++)
        {
          n += *(camadaEntrada+a) * pesosEntrada[a];
        }
        n += bEntrada[i];
        saidaAsphalt[i] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      shuffleBias(bOculta, input);
      for(int a = 0; a<input; a++)
      {
        shufflePesos(pesosOculta, a+input, input);
        for(int k = 0; k<536; k++)
        {
          n += saidaAsphalt[k] * pesosOculta[k];
        }
        n += bOculta[a];
        saidaAsphaltOculta[a] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      for(int i = 0;i < input;i++){
        pesosSaida[i] = rand()%15000;
      }
      bSaida = rand()%15000;
      for(int a = 0; a<input; a++)
      {
        camadaSaida += saidaAsphaltOculta[a] * pesosSaida[a];
      }
      camadaSaida += bSaida;
      camadaSaida = (1/(1+(pow(euler, -camadaSaida))));
      asphaltTraining[aux] = camadaSaida;
      erro = 0 - camadaSaida;
      //lendo 50 vezes para chegar no equivalente de grama
      /*for(int i = 1; i<50; i++)
      {
        double* temp = calloc(536, sizeof(double));
        for(int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(temp+a));
        }
        free(temp);
      }
      for(int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada+a));
      }
      for(int i = 0; i<536; i++)
      {
        shufflePesos(pesos);
        i = rand()%15000;
        for(int a = 0; a<536; a++)
        {
          n += *(camadaEntrada+a) * pesos[a];
        }
        n += b;
        printf("%d\n",i);
        saidaGrass[i] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      for(int a = 0; a<input; a++)
      {
        camadaOculta[a] = 0;
        for(int k = 0; k<536; k++)
        {
          camadaOculta[a] += saidaGrass[k] * pesos[a];
        }
        camadaOculta[a] += b;
        camadaOculta[a] = (1/(1+(pow(euler, -camadaOculta[a]))));
      }
      camadaSaida = 0.0;
      for(int a = 0; a<input; a++)
      {
        camadaSaida += camadaOculta[a] * pesos[a];
      }
      camadaSaida += b;
      camadaSaida = (1/(1+(pow(euler, -camadaSaida))));
      grassTraining[aux] = camadaSaida;
      erro = 1 - camadaSaida;*/
      j = 0;
      aux++;
      free(camadaEntrada);
    }
    else if(aux!=0)
    {
      for(int i=0; i<536; i++)
      {
        gradienteEntrada[i];
      }
      for(int i=0; i<input;i++)
      {
        gradienteOculta[i];
      }
      gradienteSaida = /*derivada de*/ camadaSaida * erro;
      for(int i=0; i<536; i++)
      {
        for(int j=0; j<536; j++)
        {
          pesosEntrada[j+i] = (pesosEntrada[j+i] + learningRate) * camadaSaida * *gradienteEntrada;
        }
        bEntrada[i] = (bEntrada[i] + learningRate) * *gradienteEntrada;
      }
      for(int i=0; i<input; i++)
      {
        for(int j=0; j<input; j++)
        {
          pesosOculta[j+i] = (pesosOculta[j+i] + learningRate) * saidaAsphalt[j] * *gradienteOculta;
        }
        bOculta[i] = (bOculta[i] + learningRate) * *gradienteOculta;
      }
      for(int i=0; i<input; i++)
      {
        pesosSaida[i] = (pesosSaida[i] + learningRate) * saidaAsphaltOculta[j] * gradienteSaida;
        bSaida = (bSaida + learningRate) * gradienteSaida;
      }
      double* camadaEntrada = calloc(536, sizeof(double));
      for(int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada+a));
      }
      for(int i = 0; i<536; i++)
      {
        for(int a = 0; a<536; a++)
        {
          n += *(camadaEntrada+a) * pesosEntrada[a];
        }
        n += bEntrada[i];
        saidaAsphalt[i] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      for(int a = 0; a<input; a++)
      {
        shufflePesos(pesosOculta, a+input, input);
        for(int k = 0; k<536; k++)
        {
          n += saidaAsphalt[k] * pesosOculta[k];
        }
        n += bOculta[a];
        saidaAsphaltOculta[a] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      for(int i = 0;i < input;i++){
        pesosSaida[i] = rand()%15000;
      }
      bSaida = rand()%15000;
      for(int a = 0; a<input; a++)
      {
        camadaSaida += saidaAsphaltOculta[a] * pesosSaida[a];
      }
      camadaSaida += bSaida;
      camadaSaida = (1/(1+(pow(euler, -camadaSaida))));
      asphaltTraining[aux] = camadaSaida;
      erro = 0 - camadaSaida;
      aux++;
    }
    else
    {
      double* temp = calloc(536, sizeof(double));
      for(int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(temp+a));
      }
      free(temp);
      j++;
    }
    rewind(fp);
  }
  fclose(fp);
  return 0;
}

double *shufflePesos(double *pesos, int current, int limit){
  for(int i = 0;i < limit;i++){
    pesos[current+i] = rand()%15000;
  }
  return pesos;
}

double *shuffleBias(double *bias, int limit){
  for(int i = 0;i < limit;i++){
    bias[i] = rand()%15000;
  }
  return bias;
}

int *shuffleTraining(int *vetor){
  for(int i = 1;i <= 50;i++){
    vetor[i-1] = i;
  }
  for(int i = 0;i < 50;i++){
    int temp = vetor[i];
    int randomIndex = rand()%50;
    vetor[i] = vetor[randomIndex];
    vetor[randomIndex] = temp;
  }
  return vetor;
}
