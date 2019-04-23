/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int fneuronio(float *, float *, float, int);

int main(int argc, char *argv[]){
  float ENTRADAS[MAX], PESOS[MAX], T;
  printf("Digite 10 valores de entrada:\n");
  for(int i=0;i<MAX;i++){
    scanf("%f", &ENTRADAS[i]);
  }
  printf("Digite 10 valores de peso:\n");
  for(int i=0;i<MAX;i++){
    scanf("%f", &PESOS[i]);
  }
  printf("Digite um valor para o limiar:\n");
  scanf("%f", &T);
  fneuronio(ENTRADAS, PESOS, T, MAX);
  if(ENTRADAS[0])
    printf("Neurônio ativado!\n");
  else
    printf("Neurônio inibido!\n");
  return 0;
}

int fneuronio(float *vet1, float *vet2, float lim, int total){
  float SOMAP = 0;
  for(int i=0;i<total;i++){
    SOMAP += *(vet1+i) * *(vet2+i);
  }
  if(SOMAP>lim)
    *vet1 = 1;
  else
    *vet1 = 0;
}
