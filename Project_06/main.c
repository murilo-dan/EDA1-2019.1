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

double *shufflePesos(double *);
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
  double erro;
  int random[25] = {0};
  double b;
  double pesos[536];
  shuffleTraining(random);
  fp = fopen("features.txt", "r");
  int j = 0, aux = 0;
  double saidaAsphalt[536], saidaGrass[536], n, euler = 2.718281828459045235360287;
  for(int i = 0; i<25; i++){
    printf("%d\n",random[i]);
  }
  while(aux<25)
  {
    if(j == random[aux])
    {
      double* camadaEntrada = calloc(536, sizeof(double));
      for(int a = 0; a < 536; a++)
      {
        fscanf(fp, "%lf", &*(camadaEntrada+a));
      }
      for(int i = 0; i<536; i++)
      {
        shufflePesos(pesos);
        b = rand()%15000;
        for(int a = 0; a<536; a++)
        {
          n += *(camadaEntrada+a) * pesos[a];
        }
        n += b;
        saidaAsphalt[i] = (1/(1+(pow(euler, -n))));
        n = 0;
      }
      double camadaOculta[input];
      for(int a = 0; a<input; a++)
      {
        shufflePesos(pesos);
        b = rand()%15000;
        for(int k = 0; k<536; k++)
        {
          camadaOculta[a] += saidaAsphalt[k] * pesos[a];
        }
        camadaOculta[a] += b;
        camadaOculta[a] = (1/(1+(pow(euler, -camadaOculta[a]))));
      }
      double camadaSaida = 0.0;
      shufflePesos(pesos);
      b = rand()%15000;
      for(int a = 0; a<input; a++)
      {
        camadaSaida += camadaOculta[a] * pesos[a];
      }
      camadaSaida += b;
      camadaSaida = (1/(1+(pow(euler, -camadaSaida))));
      asphaltTraining[aux] = camadaSaida;
      printf("%lf\n",asphaltTraining[aux]);
      erro = 0 - camadaSaida;
      //lendo 50 vezes para chegar no equivalente de grama
      for(int i = 1; i<50; i++)
      {
        double* temp = calloc(536, sizeof(double));
        for(int a = 0; a < 536; a++)
        {
          fscanf(fp, "%lf", &*(temp+a));
        }
        free(temp);
      }/*
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

double *shufflePesos(double *pesos){
  for(int i = 0;i < 536;i++){
    pesos[i] = rand()%15000;
  }
  return pesos;
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
