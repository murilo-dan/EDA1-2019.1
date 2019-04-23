#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int* randomize_vector(int *);
char* get_training(int, char, char *);

int main(int argc, char** argv){
  srand(time(NULL));
  char* training_data = malloc(35 * sizeof(int));
  int* ASPHALT = malloc(50 * sizeof(int));
  int* GRASS = malloc(50 *sizeof(int));
  randomize_vector(ASPHALT);
  randomize_vector(GRASS);
  for(int i = 0;i < 25;i++){
    get_training(*(ASPHALT+i), 'a', training_data);
    printf("%s\n", training_data);
  }
  printf("\n\n");
  for(int i = 0;i < 25;i++){
    get_training(*(GRASS+i), 'g', training_data);
    printf("%s\n", training_data);
  }
  free(ASPHALT);
  free(GRASS);
  free(training_data);
  return 0;
}

int *randomize_vector(int *vetor){
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

char *get_training(int index, char c, char *training_data){
  char number[3];
  sprintf(number, "%d", index);
  if(c == 'a'){
    strcpy(training_data, "dataset/asphalt/asphalt_");
  }
  else{
    strcpy(training_data, "dataset/grass/grass_");
  }
  if(index < 10){
    strcat(training_data, "0");
  }
  strcat(training_data, number);
  strcat(training_data, ".txt");
}
