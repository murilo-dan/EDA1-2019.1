#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

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
int globalTime = 480;

void display(struct Plane *);
int *shuffle(int *);
void emergencySwitch();

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

    struct Plane *current = (struct Plane *)malloc(sizeof(struct Plane));
    struct Plane *prev, *temp;
    current = fila->front;

    if (fila->front->gas == 0)
    {
        prev = current;
        current = current->next;
    }

    while (current->next != NULL)
    {
        if (current->gas == 0)
        {
            prev->next = current->next;
            temp = fila->front;
            current->next = temp;
            fila->front = current;
            prev = prev->next;
            if (prev->next->next == NULL)
            {
                break;
            }
            current = prev->next->next;
            continue;
        }
        prev = current;
        current = current->next;
    }
    display(fila->front);
    return 0;
}

void emergencySwitch()
{
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