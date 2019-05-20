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

//Declaração do protótipo das funções utilizadas.
void preenche_pixels(FILE *, int **);
char *nome_arquivo(int, char, char *);
int *shuffle(int *);
int *normalizando(double **, int);
double *ilbp(int **, double **, int);
double *glcm(int **, double **, int);

//Início da função principal.
int main(int argc, char** argv){

  //Função utilizada para que o resultado seja aleatório sempre que o programa for rodado.
  srand(time(NULL));

  //Declaração de variáveis necessárias.
  FILE *fp;
  char training_data[35];
  int i = 0, a = 0, j = 0;
  int ASPHALT[50] = {0};
  int GRASS[50] = {0};
  double aceitacao = 0, falsa_aceitacao = 0, falsa_rejeicao = 0;
  double distance_grama = 0, distance_asfalto = 0;
  double** vectors = (double **)calloc(25, sizeof(double *));

  //O programa pode levar alguns minutos para terminar a execução.
  //Caso queira visualizar qual arquivo está sendo analisado no momento
  // e em qual parte está o programa, mude a variável "flow" para "true".
  bool flow = false;

  //Randomizando os vetores.
  shuffle(ASPHALT);
  shuffle(GRASS);

  if(flow)printf("==========TREINAMENTO ASFALTO==========\n");
  //Aqui são realizados todos os cálculos necessários para o treinamento com as imagens de asfalto.
  for(i = 0; i < 25; i++){

    //Pegando o nome de um arquivo diferente a cada iteração.
    nome_arquivo(ASPHALT[i], 'a', training_data);

    //Declaração do vetor que será preenchido com todos os valores de pixels da imagem selecionada.
    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(a = 0; a < 1025; a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    if(flow)printf("%s\n", training_data);

    //Abrindo o arquivo .txt da imagem.
    fp = fopen(training_data, "r");
    //Preenchendo o vetor pixels com os valores da imagem.
    preenche_pixels(fp, pixels);

    //Declarando um vetor para a imagem selecionada a cada iteração.
    //As funções que seguem realizam o cálculo do ILBP, do GLCM e normalizam o vetor resultado.
    *(vectors+i) = (double *)calloc(536, sizeof(double));
    ilbp(pixels, vectors, i);
    glcm(pixels, vectors, i);
    normalizando(vectors, i);

    //Liberando o espaço ocupado pelo vetor pixels e fechando o arquivo.
    for(a = 0; a < 1025; a++){
      free(*(pixels+a));
    }
    free(pixels);
    fclose(fp);
  }

  //Declaração e preenchimento do vetor final de treinamento de asfalto
  // que será a média dos 25 vetores obtidos no laço anterior.
  double* asfalto = calloc(536, sizeof(double));
  for(i = 0; i < 25; i++){
    for(j = 0; j < 536; j++){
      *(asfalto+j) += *(*(vectors+i)+j);
    }
  }
  for(i = 0; i < 536; i++){
    *(asfalto+i) = *(asfalto+i)/25;
  }

  //Liberando os 25 vetores de treinamento.
  for(a = 0; a < 25; a++){
    free(*(vectors+a));
  }

  if(flow)printf("==========TREINAMENTO GRAMA==========\n");
  //Aqui são realizados todos os cálculos necessários para o treinamento com as imagens de grama.
  for(i = 0; i < 25; i++){
    //O processo aqui será semelhante ao do treinamento com as imagens de asfalto,
    //com a diferença das imagens e vetor resultante.
    nome_arquivo(*(GRASS+i), 'g', training_data);

    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(a = 0; a < 1025; a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    if(flow)printf("%s\n", training_data);

    fp = fopen(training_data, "r");
    preenche_pixels(fp, pixels);

    *(vectors+i) = (double *)calloc(536, sizeof(double));
    ilbp(pixels, vectors, i);
    glcm(pixels, vectors, i);
    normalizando(vectors, i);

    for(a = 0; a < 1025; a++){
      free(*(pixels+a));
    }
    free(pixels);
    fclose(fp);
  }

  //Declaração e preenchimento do vetor final de treinamento de grama
  // que será a média dos 25 vetores obtidos no laço anterior.
  double* grama = calloc(536, sizeof(double));
  for(i = 0; i < 25; i++){
    for(j = 0; j < 536; j++){
        *(grama+j) += *(*(vectors+i)+j);
    }
  }
  for(i = 0; i < 536; i++){
    *(grama+i) /= 25;
  }

  for(a = 0; a < 25; a++){
    free(*(vectors+a));
  }

  if(flow)printf("==========TESTE ASFALTO==========\n");
  //Aqui são realizados todos os cálculos necessários para o teste com as imagens de asfalto.
  for(i = 0; i < 25; i++){

    //Zerando as variáveis de distância que são utilizadas para determinar qual é a imagem de teste.
    distance_asfalto = 0, distance_grama = 0;

    //Nas próximas linhas (até o fechamento do arquivo) o processo é igual ao treinamento.
    nome_arquivo(*(ASPHALT+(i+25)), 'a', training_data);

    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(a = 0; a < 1025; a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    if(flow)printf("%s\n", training_data);

    fp = fopen(training_data, "r");
    preenche_pixels(fp, pixels);

    *(vectors+i) = (double *)calloc(536, sizeof(double));
    ilbp(pixels, vectors, i);
    glcm(pixels, vectors, i);
    normalizando(vectors, i);

    for(a = 0; a < 1025; a++){
      free(*(pixels+a));
    }
    free(pixels);
    fclose(fp);

    //Aqui é calculada a distância euclidiana entre os vetores finais de treinamento
    // e o vetor final da imagem analisada no teste.
    for(a = 0; a < 536; a++){
      distance_asfalto += pow((*(*(vectors+i)+a)-*(asfalto+a)), 2);
      distance_grama += pow((*(*(vectors+i)+a)-*(grama+a)), 2);
    }
    distance_asfalto = sqrt(distance_asfalto);
    distance_grama = sqrt(distance_grama);

    //Comparando as distâncias, as variáveis correspondentes de acerto ou falsa aceitação
    // serão incrementadas.
    if(distance_asfalto < distance_grama){
      aceitacao++;
    }
    else if(distance_asfalto > distance_grama){
      falsa_aceitacao++;
    }
  }

  for(a = 0; a < 25; a++){
    free(*(vectors+a));
  }

  if(flow)printf("==========TESTE GRAMA==========\n");
  //Aqui são realizados todos os cálculos necessários para o teste com as imagens de asfalto.
  for(i = 0; i < 25; i++){
    //O processo aqui será semelhante ao do teste com as imagens de asfalto,
    //com a diferença das imagens e variáveis finais.
    distance_asfalto = 0, distance_grama = 0;

    nome_arquivo(*(GRASS+(i+25)), 'g', training_data);

    int** pixels = (int **)calloc(1025, sizeof(int *));
    for(a = 0; a < 1025; a++){
      *(pixels+a) = (int *)calloc(1025, sizeof(int));
    }

    if(flow)printf("%s\n", training_data);

    fp = fopen(training_data, "r");
    preenche_pixels(fp, pixels);

    *(vectors+i) = (double *)calloc(536, sizeof(double));
    ilbp(pixels, vectors, i);
    glcm(pixels, vectors, i);
    normalizando(vectors, i);

    for(a = 0; a < 1025; a++){
      free(*(pixels+a));
    }
    free(pixels);
    fclose(fp);

    //Cálculo de distância euclidiana.
    for(a = 0; a < 536; a++){
      distance_asfalto += pow((*(*(vectors+i)+a)-*(asfalto+a)), 2);
      distance_grama += pow((*(*(vectors+i)+a)-*(grama+a)), 2);
    }
    distance_asfalto = sqrt(distance_asfalto);
    distance_grama = sqrt(distance_grama);

    //Comparando as distâncias, as variáveis correspondentes de acerto ou falsa rejeição
    // serão incrementadas.
    if(distance_grama < distance_asfalto){
      aceitacao++;
    }
    else if(distance_grama > distance_asfalto){
      falsa_rejeicao++;
    }
  }

  //Multiplicando as variáveis finais por 2 para conseguir a porcentagem de cada uma.
  aceitacao *= 2.0;
  falsa_rejeicao *= 2.0;
  falsa_aceitacao *= 2.0;

  //Saída padrão no terminal, apresentando as taxas de acerto, falsa aceitação e falsa rejeição.
  printf("Acerto: %.1lf%%\nFalsa Aceitação: %.1lf%%\nFalsa Rejeição: %.1lf%%\n", aceitacao, falsa_aceitacao, falsa_rejeicao);

  //Liberando as últimas alocações feitas.
  for(a = 0; a < 25; a++){
    free(*(vectors+a));
  }
  free(vectors);
  free(asfalto);
  free(grama);
  return 0;
}

//Função realiza o embaralhamento dos vetores para randomizar os arquivos lidos.
int *shuffle(int *vetor){
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

//Função utiliza os valores embaralhados para retornar o nome do arquivo a ser lido.
char *nome_arquivo(int index, char c, char *training_data){
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
  return training_data;
}

//Função preenche o vetor recebido com os valores de cada pixel da imagem (.txt) selecionada.
void preenche_pixels(FILE *in, int **array){
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

//Função realiza a normalização do vetor recebido.
int *normalizando(double **ilbp_vec, int x){
  int max=0;
  int min=999999999;
  for(int i = 0; i < 536; i++){
    if(max < *(*(ilbp_vec+x)+i)){
      max = *(*(ilbp_vec+x)+i);
    }
    if(min > *(*(ilbp_vec+x)+i)){
      min = *(*(ilbp_vec+x)+i);
    }
  }
  for(int i = 0; i < 536; i++){
      *(*(ilbp_vec+x)+i) = (*(*(ilbp_vec+x)+i) - min)/(max - min);
  }
}

//Função calcula o ILBP.
double *ilbp(int **pixel, double **ilbp_vec, int x){
  double media = 0;
  int min, total, temp;
  int bit[9] = {0};
  int vetor[9] = {0};
  for(int i=1;i<1024;i++){
    for(int j=1;j<1024;j++){
      //Realiza o cálculo da média na vizinhança de 8 do pixel analisado.
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
      //Converte os valores para 1 ou 0, de acordo com a comparação com a média.
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
        //Converte o binário obtido para decimal.
        for(int a=8;a>=0;a--){
          total += bit[a] * pow(2,(8-a));
        }
        //Armazena o menor decimal obtido.
        if(total<=min){
          min = total;
        }
        //Faz um shift left no binário obtido.
        temp=bit[8];
        for(int k=8;k>0;k--){
          bit[k]=bit[k-1];
        }
        bit[0]=temp;
      }
      //Incrementa 1 na posição do menor valor obtido.
      *(*(ilbp_vec+x)+min) += 1;
    }
  }
}

//Função calcula o GLCM.
double *glcm(int **pixel, double **glcm_vec, int x){
  int i = 0, j = 0;
  int vizinho_cima[256][256] = {0}, vizinho_cima_esquerda[256][256] = {0},
  vizinho_cima_direita[256][256] = {0};
  int vizinho_direita[256][256] = {0}, vizinho_esquerda[256][256] = {0};
  int vizinho_baixo[256][256] = {0}, vizinho_baixo_esquerda[256][256] = {0},
  vizinho_baixo_direita[256][256] = {0};
  double energia[8] = {0}, contraste[8] = {0}, homogeneidade[8] = {0};

  //Cada um dos grupos de laços a seguir realiza os cálculos do GLCM para uma direção,
  // identificada pelo nome do vetor resultante.
  //As métricas obtidas (energia, contraste e homogeneidade) são armazenadas nos respectivos vetores.
  for(i = 1; i < 1025; i++){
    for(j = 1; j < 1025; j++){
      vizinho_cima_esquerda[*(*(pixel+i)+j)][*(*(pixel+(i-1))+(j-1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[0] += pow(vizinho_cima_esquerda[i][j],2);
      contraste[0] += pow(fabs(i-j),2)*vizinho_cima_esquerda[i][j];
      homogeneidade[0] += vizinho_cima_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 1; i < 1025; i++){
    for(j = 0; j < 1025; j++){
      vizinho_cima[*(*(pixel+i)+j)][*(*(pixel+(i-1))+j)] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[1] += pow(vizinho_cima[i][j],2);
      contraste[1] += pow(fabs(i-j),2)*vizinho_cima[i][j];
      homogeneidade[1] += vizinho_cima[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 1; i < 1025; i++){
    for(j = 0; j < 1024; j++){
      vizinho_cima_direita[*(*(pixel+i)+j)][*(*(pixel+(i-1))+(j+1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[2] += pow(vizinho_cima_direita[i][j],2);
      contraste[2] += pow(fabs(i-j),2)*vizinho_cima_direita[i][j];
      homogeneidade[2] += vizinho_cima_direita[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 0; i < 1025; i++){
    for(j = 1; j < 1025; j++){
      vizinho_esquerda[*(*(pixel+i)+j)][*(*(pixel+i)+(j-1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[3] += pow(vizinho_esquerda[i][j],2);
      contraste[3] += pow(fabs(i-j),2)*vizinho_esquerda[i][j];
      homogeneidade[3] += vizinho_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 0; i < 1025; i++){
    for(j = 0; j < 1024; j++){
      vizinho_direita[*(*(pixel+i)+j)][*(*(pixel+i)+(j+1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[4] += pow(vizinho_direita[i][j],2);
      contraste[4] += pow(fabs(i-j),2)*vizinho_direita[i][j];
      homogeneidade[4] += vizinho_direita[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 0; i < 1024; i++){
    for(j = 1; j < 1025; j++){
      vizinho_baixo_esquerda[*(*(pixel+i)+j)][*(*(pixel+(i+1))+(j-1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[5] += pow(vizinho_baixo_esquerda[i][j],2);
      contraste[5] += pow(fabs(i-j),2)*vizinho_baixo_esquerda[i][j];
      homogeneidade[5] += vizinho_baixo_esquerda[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 0; i < 1024; i++){
    for(j = 0; j < 1025; j++){
      vizinho_baixo[*(*(pixel+i)+j)][*(*(pixel+(i+1))+j)] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[6] += pow(vizinho_baixo[i][j],2);
      contraste[6] += pow(fabs(i-j),2)*vizinho_baixo[i][j];
      homogeneidade[6] += vizinho_baixo[i][j]/(1+(abs(i-j)));
    }
  }

  for(i = 0; i < 1024; i++){
    for(j = 0; j < 1024; j++){
      vizinho_baixo_direita[*(*(pixel+i)+j)][*(*(pixel+(i+1))+(j+1))] += 1;
    }
  }
  for(i = 0; i < 256; i++){
    for(j = 0; j < 256; j++){
      energia[7] += pow(vizinho_baixo_direita[i][j],2);
      contraste[7] += pow(fabs(i-j),2)*vizinho_baixo_direita[i][j];
      homogeneidade[7] += vizinho_baixo_direita[i][j]/(1+(abs(i-j)));
    }
  }

  //Realiza o preenchimento das posições do GLCM, resultando no vetor ILBP + GLCM.
  for(i = 512; i < 536; i++){
    if(i%3==0){
      *(*(glcm_vec+x)+i)=energia[(i-512)/3];
    }
    else if(i%3==1){
      *(*(glcm_vec+x)+i)=contraste[(i-512)/3];
    }
    else if(i%3==2){
      *(*(glcm_vec+x)+i)=homogeneidade[(i-512)/3];
    }
  }
}
