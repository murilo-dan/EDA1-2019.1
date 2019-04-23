/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

float *recebe_notas(float *, int);
void conta_notas(float *, int);
int percent_aprov(float *);

int main(int argc, char *argv[]){
  float NOTAS[MAX];
  float *APR;
  int metade = 0;
  printf("Digite as notas:\n");
  for(int i=0;i<MAX;i++){
    scanf("%f", &NOTAS[i]);
  }
  APR = recebe_notas(NOTAS, MAX);
  conta_notas(APR, MAX);
  printf("Número de aprovados: %.0f\nNúmero de reprovados: %.0f\n",*APR, *(APR+1));
  metade = percent_aprov(APR);
  printf("Percentual de aprovados: %.1f%%\nPercentual de reprovados: %.1f%%\n",*APR, *(APR+1));
  if(metade)
    printf("Mais da metade da turma foi aprovada.\n");
  return 0;
}

float *recebe_notas(float *APR, int elementos){
  for(int i=0;i<elementos;i++){
    if(*(APR+i)>=6.0)
      *(APR+i) = 1;
    else
      *(APR+i) = 0;
  }
  return APR;
}

void conta_notas(float *APR, int elementos){
  int aprovado=0, reprovado=0;
  for (int i=0;i<elementos;i++){
    if (*(APR+i)==1)
      aprovado++;
    else
      reprovado++;
  }
  *APR = aprovado;
  *(APR+1) = reprovado;
}

int percent_aprov(float *APR){
  *APR *= 10;
  *(APR+1) *= 10;
  if(*APR>50)
    return 1;
  return 0;
}
