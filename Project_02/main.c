#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int* randomize_vector(int *);
char* get_training(int, char, char *);
void get_pixels(FILE *, int **);

int main(int argc, char** argv){
  srand(time(NULL));
  FILE *fp;
  char* training_data = calloc(35, sizeof(int));
  int* ASPHALT = calloc(50, sizeof(int));
  int* GRASS = calloc(50, sizeof(int));
  randomize_vector(ASPHALT);
  randomize_vector(GRASS);
  for(int i = 0;i < 25;i++){
    get_training(*(ASPHALT+i), 'a', training_data);
    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(int a = 0;a < 1025;a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }
    fp = fopen(training_data, "r");
    get_pixels(fp, pixels);
    for(int a = 0;a < 1025;a++){
      free(pixels[a]);
    }
    free(pixels);
    fclose(fp);
  }
  for(int i = 0;i < 25;i++){
    get_training(*(GRASS+i), 'g', training_data);
    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(int a = 0;a < 1025;a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }
    fp = fopen(training_data, "r");
    get_pixels(fp, pixels);
    for(int a = 0;a < 1025;a++){
      free(pixels[a]);
    }
    free(pixels);
    fclose(fp);
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
  sprintf(number, "%02d", index);
  if(c == 'a'){
    strcpy(training_data, "DataSet/asphalt/asphalt_");
  }
  else{
    strcpy(training_data, "DataSet/grass/grass_");
  }
  strcat(training_data, number);
  strcat(training_data, ".txt");
}

void get_pixels(FILE *in, int **array){
  int c;
  for(int i = 0;i<1025;i++){
    for(int j = 0;j<1025;j++){
      fscanf(in, "%d", *(array+i)+j);
      if((c=fgetc(in))!=';'){
        ungetc(c,in);
      }
    }
  }
}
