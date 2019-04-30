#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int *randomize_vector(int *);
char *get_training(int, char, char *);
void get_pixels(FILE *, int **);
float *ilbp(int **, float **, int);
float *glcm(int **, float **, int);
int *normalizando(float **, int);

int main(int argc, char** argv){
  srand(time(NULL));
  FILE *fp;
  char training_data[35];
  int ASPHALT[50]={0};
  int GRASS[50]={0};
  float** ilbp_vectors = (float **)calloc(25, sizeof(float *));
  float** glcm_vectors = (float **)calloc(25, sizeof(float *));
  randomize_vector(ASPHALT);
  randomize_vector(GRASS);

  //ASFALTO
  for(int i = 0;i < 25;i++){
    get_training(ASPHALT[i], 'a', training_data);
    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(int a = 0;a < 1025;a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    printf("%s\n", training_data);

    fp = fopen(training_data, "r");
    get_pixels(fp, pixels);
    *(ilbp_vectors+i) = (float *)calloc(512, sizeof(float));
    *(glcm_vectors+i) = (float *)calloc(24, sizeof(float));
    ilbp(pixels, ilbp_vectors, i);
    glcm(pixels, glcm_vectors, i);
    normalizando(ilbp_vectors, i);

    for(int a = 0;a < 1025;a++){
      free(pixels[a]);
    }
    free(pixels);
    fclose(fp);
  }

  float* ilbp_asfalto = calloc(512, sizeof(int));
  for(int i = 0; i < 25; i++){
    for(int j = 0; j < 512; j++){
      *(ilbp_asfalto+j) += *(*(ilbp_vectors+i)+j);
    }
  }
  for(int i = 0; i < 512; i++){
    *(ilbp_asfalto+i) /= 25;
  }
  for(int a = 0;a < 25;a++){
    free(ilbp_vectors[a]);
    free(glcm_vectors[a]);
  }

  //GRAMA
  for(int i = 0;i < 25;i++){
    get_training(*(GRASS+i), 'g', training_data);

    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(int a = 0;a < 1025;a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    printf("%s\n", training_data);

    fp = fopen(training_data, "r");
    get_pixels(fp, pixels);
    *(ilbp_vectors+i) = (float *)calloc(512, sizeof(float));
    *(glcm_vectors+i) = (float *)calloc(24, sizeof(float));
    ilbp(pixels, ilbp_vectors, i);
    glcm(pixels, glcm_vectors, i);
    normalizando(ilbp_vectors, i);

    for(int a = 0;a < 1025;a++){
      free(pixels[a]);
    }
    free(pixels);
    fclose(fp);
  }

  float* ilbp_grama = calloc(512, sizeof(float));
  for(int i = 0; i < 25; i++){
    for(int j = 0; j < 512; j++){
        *(ilbp_grama+j) += *(*(ilbp_vectors+i)+j);
    }
  }
  for(int i = 0; i < 512; i++){
    *(ilbp_grama+i) /= 25;
  }
  for(int a = 0;a < 25;a++){
    free(ilbp_vectors[a]);
    free(glcm_vectors[a]);
  }

  free(ilbp_vectors);
  free(glcm_vectors);
  free(ilbp_asfalto);
  free(ilbp_grama);
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
  char pixelber[3];
  sprintf(pixelber, "%02d", index);
  if(c == 'a'){
    strcpy(training_data, "DataSet/asphalt/asphalt_");
  }
  else{
    strcpy(training_data, "DataSet/grass/grass_");
  }
  strcat(training_data, pixelber);
  strcat(training_data, ".txt");
  return training_data;
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

int *normalizando(float **ilbp_vec, int x){
  int max=0;
  int min=1046530;
  for(int i = 0; i < 512; i++){
    if(max < *(*(ilbp_vec+x)+i)){
      max = *(*(ilbp_vec+x)+i);
    }
    if(min > *(*(ilbp_vec+x)+i) && *(*(ilbp_vec+x)+i) != 0){
      min = *(*(ilbp_vec+x)+i);
    }
  }
  for(int i = 0; i < 512; i++){
    if(*(*(ilbp_vec+x)+i) != 0){
      *(*(ilbp_vec+x)+i) = (*(*(ilbp_vec+x)+i) - min)/(max - min);
    }
  }
}

float *ilbp(int **pixel, float **ilbp_vec, int x){
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
      *(*(ilbp_vec+x)+min) += 1;
    }
  }
}

float *glcm(int **pixel, float **glcm_vec, int x){
  int vizinho_cima[256][256] = {0}, vizinho_cima_esquerda[256][256] = {0},
  vizinho_cima_direita[256][256] = {0};
  int vizinho_direita[256][256] = {0}, vizinho_esquerda[256][256] = {0};
  int vizinho_baixo[256][256] = {0}, vizinho_baixo_esquerda[256][256] = {0},
  vizinho_baixo_direita[256][256] = {0};
  float energia[8] = {0}, contraste[8] = {0}, homogeneidade[8] = {0};

  for(int i=1;i<1025;i++){
    for(int j=1;j<1025;j++){
      vizinho_cima_esquerda[*(*(pixel+i)+j)][*(*(pixel+(i-1))+(j-1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[0] += pow(vizinho_cima_esquerda[i][j],2);
      contraste[0] += pow((i-j),2)*vizinho_cima_esquerda[i][j];
      homogeneidade[0] += vizinho_cima_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=1;i<1025;i++){
    for(int j=0;j<1024;j++){
      vizinho_cima[*(*(pixel+i)+j)][*(*(pixel+(i-1))+j)]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[1] += pow(vizinho_cima[i][j],2);
      contraste[1] += pow((i-j),2)*vizinho_cima[i][j];
      homogeneidade[1] += vizinho_cima[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=1;i<1025;i++){
    for(int j=0;j<1024;j++){
      vizinho_cima_direita[*(*(pixel+i)+j)][*(*(pixel+(i-1))+(j+1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[2] += pow(vizinho_cima_direita[i][j],2);
      contraste[2] += pow((i-j),2)*vizinho_cima_direita[i][j];
      homogeneidade[2] += vizinho_cima_direita[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<1025;i++){
    for(int j=1;j<1025;j++){
      vizinho_esquerda[*(*(pixel+i)+j)][*(*(pixel+i)+(j-1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[3] += pow(vizinho_esquerda[i][j],2);
      contraste[3] += pow((i-j),2)*vizinho_esquerda[i][j];
      homogeneidade[3] += vizinho_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<1025;i++){
    for(int j=0;j<1024;j++){
      vizinho_direita[*(*(pixel+i)+j)][*(*(pixel+i)+(j+1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[4] += pow(vizinho_direita[i][j],2);
      contraste[4] += pow((i-j),2)*vizinho_direita[i][j];
      homogeneidade[4] += vizinho_direita[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<1024;i++){
    for(int j=1;j<1025;j++){
      vizinho_baixo_esquerda[*(*(pixel+i)+j)][*(*(pixel+(i+1))+(j-1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[5] += pow(vizinho_baixo_esquerda[i][j],2);
      contraste[5] += pow((i-j),2)*vizinho_baixo_esquerda[i][j];
      homogeneidade[5] += vizinho_baixo_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<1024;i++){
    for(int j=0;j<1025;j++){
      vizinho_baixo[*(*(pixel+i)+j)][*(*(pixel+(i+1))+j)]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[6] += pow(vizinho_baixo[i][j],2);
      contraste[6] += pow((i-j),2)*vizinho_baixo[i][j];
      homogeneidade[6] += vizinho_baixo[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<1024;i++){
    for(int j=0;j<1024;j++){
      vizinho_baixo_direita[*(*(pixel+i)+j)][*(*(pixel+(i+1))+(j+1))]++;
    }
  }
  for(int i=0;i<256;i++){
    for(int j=0;j<256;j++){
      energia[7] += pow(vizinho_baixo_direita[i][j],2);
      contraste[7] += pow((i-j),2)*vizinho_baixo_direita[i][j];
      homogeneidade[7] += vizinho_baixo_direita[i][j]/(1+(abs(i-j)));
    }
  }

  for(int i=0;i<24;i++){
    if(i%3==0){
      *(*(glcm_vec+x)+i)=energia[i/3];
    }
    else if(i%3==1){
      *(*(glcm_vec+x)+i)=contraste[i/3];
    }
    else if(i%3==2){
      *(*(glcm_vec+x)+i)=homogeneidade[i/3];
    }
  }
}
