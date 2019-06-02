#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

struct Fila
{
    struct Plane *front;
    struct Plane *rear;
};

struct Plane
{
    char id[6];
    int gas;
    char mode;
    struct Plane *next;
};

const int timeUnit = 5;
int auxTime = 0;
int globalTime = 480;
bool pista1 = false, pista2 = false, pista3 = false;
int pista1Usage = 0, pista2Usage = 0, pista3Usage = 0;

void removeAirplane(struct Plane *, struct Fila *);
void display(struct Plane *);
int *shuffle(int *);
int countQueue(struct Fila *);
void management(struct Plane *, struct Fila *);
void emergencySwitch(struct Fila *);
void timePassage(struct Fila *);

int main(int argc, char **argv)
{
    srand(time(NULL));

    struct Fila *fila = (struct Fila *)malloc(sizeof(struct Fila));
    fila->rear = fila->front = NULL;

    char *idVoos[64] = {"VG3001",
                        "JJ4404",
                        "LN7001",
                        "TG1501",
                        "GL7602",
                        "TT1010",
                        "AZ1009",
                        "AZ1008",
                        "AZ1010",
                        "TG1506",
                        "VG3002",
                        "JJ4402",
                        "GL7603",
                        "RL7880",
                        "AL0012",
                        "TT4544",
                        "TG1505",
                        "VG3003",
                        "JJ4403",
                        "JJ4401",
                        "LN7002",
                        "AZ1002",
                        "AZ1007",
                        "GL7604",
                        "AZ1006",
                        "TG1503",
                        "AZ1003",
                        "JJ4403",
                        "AZ1001",
                        "LN7003",
                        "AZ1004",
                        "TG1504",
                        "AZ1005",
                        "TG1502",
                        "GL7601",
                        "TT4500",
                        "RL7801",
                        "JJ4410",
                        "GL7607",
                        "AL0029",
                        "VV3390",
                        "VV3392",
                        "GF4681",
                        "GF4690",
                        "AZ1020",
                        "JJ4435",
                        "VG3010",
                        "LF0920",
                        "AZ1065",
                        "LF0978",
                        "RL7867",
                        "TT4502",
                        "GL7645",
                        "LF0932",
                        "JJ4434",
                        "TG1510",
                        "TT1020",
                        "AZ1098",
                        "BA2312",
                        "VG3030",
                        "BA2304",
                        "KL5609",
                        "KL5610",
                        "KL5611"};
    int NAprox = rand() % 23 + 10;
    int NDec = rand() % 23 + 10;
    int NVoos = NAprox + NDec;
    int aprox_aux = NAprox;
    int dec_aux = NDec;
    int random[64] = {0};
    shuffle(random);

    for (int i = 0; i < NVoos; i++)
    {
        struct Plane *newPlane = (struct Plane *)malloc(sizeof(struct Plane));

        strcpy(newPlane->id, idVoos[random[i]]);

        if (rand() % 2 == 0 && aprox_aux != 0)
        {
            newPlane->mode = 'A';
            aprox_aux--;
        }
        else if (dec_aux != 0)
        {
            newPlane->mode = 'D';
            dec_aux--;
        }
        else
        {
            newPlane->mode = 'A';
        }

        if (newPlane->mode == 'A')
        {
            newPlane->gas = rand() % 13;
        }
        else
        {
            newPlane->gas = -1;
        }
        if (fila->rear == NULL)
        {
            fila->rear = fila->front = newPlane;
            continue;
        }
        fila->rear->next = newPlane;
        fila->rear = newPlane;
    }
    printf("Aeroporto Internacional de Brasília\nHora inicial: %02d:%02d\nFila de pedidos:\nCódgigo de voo - Tipo - Prioridade\n", globalTime / 60, globalTime - (globalTime / 60) * 60);
    display(fila->front);
    printf("\nNVoos: %d\nNAproximações: %d\nNDecolagens: %d\n\nListagem de eventos:\n", NVoos, NAprox, NDec);
    while(NVoos!=0)
    {
      emergencySwitch(fila);
      management(fila->front, fila);
      timePassage(fila);
      NVoos = countQueue(fila) - NDec;
    }
    return 0;
}

int countQueue(struct Fila *fila)
{
    int contagem = 0;
    struct Plane *head = fila->front;
    while(head!=NULL)
    {
      contagem++;
      head = head->next;
    }
    return contagem;
}

void management(struct Plane *head, struct Fila *fila)
{
    while(head!=NULL)
    {
      if(head->gas == 0)
      {
        if(pista1 == false)
        {
          pista1 = true;
          fila->front = fila->front->next;
          printf("Código do voo: %s\nStatus: Aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: Pista 1\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60);
        }
        else if(pista2 == false)
        {
          pista2 = true;
          fila->front = fila->front->next;
          printf("Código do voo: %s\nStatus: Aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: Pista 2\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60);
        }
        else if(pista3 == false && pista2 == true && pista1 == true)
        {
          printf("ALERTA GERAL DE DESVIO DE AERONAVE\n");
          pista3 = true;
          fila->front = fila->front->next;
          printf("Código do voo: %s\nStatus: Aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: Pista 3\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60);
        }
      }
    /*  else if(head->mode == 'D')
      {
        if(pista3 == false)
        {
          pista3 = true;
          printf("Código do voo: %s\nStatus: Aeronave decolou\nHorário do início do procedimento:%02d:%02d\n%d\nNúmero da pista: Pista 3\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60, globalTime);
        }
        else if(pista2 == false)
        {
          pista2 = true;
          printf("Código do voo: %s\nStatus: Aeronave decolou\nHorário do início do procedimento:%02d:%02d\n%d\nNúmero da pista: Pista 2\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60, globalTime);
        }
        else if(pista1 == false)
        {
          pista1 = true;
          printf("Código do voo: %s\nStatus: Aeronave decolou\nHorário do início do procedimento:%02d:%02d\n%d\nNúmero da pista: Pista 1\n\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60, globalTime);
        }
        if(pista1 == true)
        {
          if(pista1Usage/timeUnit == 2)
          {
            pista1 = false;
            pista1Usage = 0;
          }
          else
          {
            pista1Usage += 5;
          }
        }
        if(pista2 == true)
        {
          if(pista2Usage/timeUnit == 2)
          {
            pista2 = false;
            pista2Usage = 0;
          }
          else
          {
            pista2Usage += 5;
          }
        }
        if(pista3 == true)
        {
          if(pista3Usage/timeUnit == 2)
          {
            pista3 = false;
            pista3Usage = 0;
          }
          else
          {
            pista3Usage += 5;
          }
        }
      }*/
      head = head->next;
    }
    if(pista1 == true)
    {
      pista1Usage += 5;
      if(pista1Usage == 20)
      {
        pista1 = false;
        printf("Pista 1 liberada as %02d:%02d!\n", globalTime / 60, globalTime - (globalTime / 60) * 60);
        pista1Usage = 0;
      }
    }
    if(pista2 == true)
    {
      pista2Usage += 5;
      if(pista2Usage == 20)
      {
        pista2 = false;
        printf("Pista 2 liberada as %02d:%02d!\n", globalTime / 60, globalTime - (globalTime / 60) * 60);
        pista2Usage = 0;
      }
    }
    if(pista3 == true)
    {
      pista3Usage += 5;
      if(pista3Usage == 20)
      {
        pista3 = false;
        printf("Pista 3 liberada as %02d:%02d!\n", globalTime / 60, globalTime - (globalTime / 60) * 60);
        pista3Usage = 0;
      }
    }
}

void emergencySwitch(struct Fila *fila)
{
    struct Plane *current = (struct Plane *)malloc(sizeof(struct Plane));
    struct Plane *prev, *temp, *temp2;
    current = fila->front;
    int gasCheck = 0;
    if (fila->front->gas == 0)
    {
        prev = current;
        current = current->next;
        gasCheck++;
    }
    while (current->next != NULL)
    {
        if (current->gas == 0)
        {
            prev->next = current->next;
            temp = fila->front;
            current->next = temp;
            fila->front = current;
            current = prev->next;
            gasCheck++;
            if(gasCheck==4)
            {
              gasCheck--;
              if(pista1==true && pista2==true && pista3==true)
              {
                temp2 = fila->front;
                removeAirplane(temp2, fila);
              }
            }
            continue;
        }
        prev = current;
        current = current->next;
    }
    if (fila->rear->gas == 0)
    {
        prev->next = NULL;
        temp = fila->front;
        current->next = temp;
        fila->front = current;
        fila->rear = prev;
    }
}

void timePassage(struct Fila *fila)
{
    struct Plane *head = fila->front;
    globalTime += timeUnit;
    auxTime++;
    while(head != NULL)
    {
      if(head->gas!=-1 && head->gas!=0 && auxTime==10)
      {
        head->gas--;
      }
      head = head->next;
    }
    if(auxTime==10)
    {
      auxTime = 0;
    }
}

void removeAirplane(struct Plane *head, struct Fila *fila)
{
    fila->front = fila->front->next;
    printf("ALERTA CRÍTICO, AERONAVE %s IRÁ CAIR ÁS %02d:%02d.\n",head->id, globalTime/60, globalTime - (globalTime / 60) * 60);
}

void display(struct Plane *head)
{
    while (head != NULL)
    {
        printf("     %s    -   %c  -    ", head->id, head->mode);
        if (head->gas == -1)
        {
            printf("N/A\n");
        }
        else
        {
            printf("%d\n", head->gas);
        }
        head = head->next;
    }
}

int *shuffle(int *vetor)
{
    for (int i = 1; i <= 50; i++)
    {
        vetor[i - 1] = i;
    }
    for (int i = 0; i < 50; i++)
    {
        int temp = vetor[i];
        int randomIndex = rand() % 50;
        vetor[i] = vetor[randomIndex];
        vetor[randomIndex] = temp;
    }
    return vetor;
}
