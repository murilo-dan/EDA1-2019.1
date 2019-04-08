/*
Estrutura de Dados 1 - Turma A
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int fneuronio(double *, double *, double, int);

int main(int argc, char *argv[]){
  double ENTRADAS[MAX], PESOS[MAX], T;
  printf("Digite 10 valores de entrada:\n");
  for(int i=0;i<MAX;i++){
    scanf("%lf", &ENTRADAS[i]);
  }
  printf("Digite 10 valores de peso:\n");
  for(int i=0;i<MAX;i++){
    scanf("%lf", &PESOS[i]);
  }
  printf("Digite um valor para o limiar:\n");
  scanf("%lf", &T);
  if(fneuronio(ENTRADAS, PESOS, T, MAX)){
    printf("Neurônio ativado!\n");
  }
  else
    printf("Neurônio inibido!\n");
  return 0;
}

int fneuronio(double *vet1, double *vet2, double lim, int total){
  double SOMAP = 0;
  for(int i=0;i<total;i++){
    SOMAP += vet1[i] * vet2[i];
  }
  if(SOMAP>lim){
    return 1;
  }
    return 0;
}
