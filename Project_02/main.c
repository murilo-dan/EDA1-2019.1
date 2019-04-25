#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int *randomize_vector(int *);
char *get_training(int, char, char *);
void get_pixels(FILE *, int **);
int *ilbp(int **);

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
    ilbp(pixels);
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
    ilbp(pixels);
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

int *ilbp(int **pixel){
  float media = 0;
  int min, total, temp;
  int bit[9] = {0};
  int vetor[9] = {0};
  for(int i=1;i<1024;i++){
    for(int j=1;j<1024;j++){
      min = 512;
      vetor[0] = *(*(pixel+(i-1))+(j-1));
      vetor[1] = *(*(pixel+(i-1))+j);
      vetor[2] = *(*(pixel+(i-1))+(j+1));
      vetor[3] = *(*(pixel+i)+(j-1));
      vetor[4] = *(*(pixel+i)+j);
      vetor[5] = *(*(pixel+i)+(j+1));
      vetor[6] = *(*(pixel+(i+1))+(j-1));
      vetor[7] = *(*(pixel+(i+1))+j);
      vetor[8] = *(*(pixel+(i+1))+(j+1));
      media = (vetor[0]+vetor[1]+vetor[2]+vetor[3]+vetor[4]+vetor[5]+vetor[6]+vetor[7]+vetor[8])/9.0;
      for(int a=0;a<9;a++){
        if(vetor[a]>=media){
          bit[a]=1;
        }
        else{
          bit[a]=0;
        }
      }
      for(int b=0;b<9;b++){
        total = 0;
        for(int a=8;a>=0;a--){
          total += bit[a] * pow(2,(8-a));
        }
        if(total<=min){
          min = total;
        }
        temp=bit[8];
        for(int k=8;k>0;k--){
          bit[k]=bit[k-1];
        }
        bit[0]=temp;
      }
    }
  }
}
