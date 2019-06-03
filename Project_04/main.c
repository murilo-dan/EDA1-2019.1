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
int globalTime = 480;
int pista1 = 480, pista2 = 480, pista3 = 480;
int decrease_fuel = 0;
int emergency = 0;

void display(struct Plane *);
int *shuffle(int *);
void deQueue(struct Fila *);
int emergencySwitch(struct Fila *, int *, int);
void management(struct Plane *, int *, int *, struct Fila *);
void removePlane(struct Plane *, struct Plane *, struct Fila *);

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
    printf("\nNVoos: %d\nNAproximações: %d\nNDecolagens: %d\n\nListagem de eventos:\n\n", NVoos, NAprox, NDec);

    for (; NVoos > 0;)
    {
        emergency = emergencySwitch(fila, &NVoos, emergency);
        printf("EMERGENY ALERT: %d\n", emergency);
        management(fila->front, &emergency, &NVoos, fila);
    }

    printf("Todos os pedidos atendidos.\n\n");

    return 0;
}

void removePlane(struct Plane *del, struct Plane *prev, struct Fila *fila)
{
    if (prev == NULL)
    {
        fila->front = del->next;
        free(del);
        return;
    }
    else if (del->next == NULL)
    {
        prev->next = NULL;
        free(del);
    }
    else
    {
        prev->next = del->next;
        free(del);
    }
}

void management(struct Plane *head, int *emergency, int *NVoos, struct Fila *fila)
{
    struct Plane *prev = NULL;
    while (head != NULL)
    {
        if (head->mode == 'A')
        {
            if (pista1 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 1\n\n", head->id, pista1 / 60, pista1 - (pista1 / 60) * 60);
                pista1 += 4 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                break;
            }
            else if (pista2 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 2\n\n", head->id, pista2 / 60, pista2 - (pista2 / 60) * 60);
                pista2 += 4 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                break;
            }
            else if ((*emergency) && pista3 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 3\n\n", head->id, pista3 / 60, pista3 - (pista3 / 60) * 60);
                pista3 += 4 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                (*emergency) = 0;
                break;
            }
        }
        else if (head->mode == 'D')
        {
            if (pista3 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 3\n\n", head->id, pista3 / 60, pista3 - (pista3 / 60) * 60);
                pista3 += 2 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                break;
            }
            else if (pista2 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 2\n\n", head->id, pista2 / 60, pista2 - (pista2 / 60) * 60);
                pista2 += 2 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                break;
            }
            else if (pista1 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 1\n\n", head->id, pista1 / 60, pista1 - (pista1 / 60) * 60);
                pista1 += 2 * timeUnit;
                removePlane(head, prev, fila);
                (*NVoos)--;
                break;
            }
        }
        prev = head;
        head = head->next;
    }
    if (head == NULL)
    {
        int min = 10000;
        if (min >= pista1)
        {
            min = pista1;
        }
        if (min >= pista2)
        {
            min = pista2;
        }
        pista3 = min;
    }

    if (pista1 <= globalTime)
    {
        globalTime = pista1;
    }
    else if (pista2 <= globalTime)
    {
        globalTime = pista2;
    }
    else if (pista3 <= globalTime)
    {
        globalTime = pista3;
    }
    else
    {
        int min = 10000;
        if (min >= pista1)
        {
            min = pista1;
        }
        if (min >= pista2)
        {
            min = pista2;
        }
        if (min >= pista3)
        {
            min = pista3;
        }
        globalTime = min;
    }
}

int emergencySwitch(struct Fila *fila, int *NVoos, int emergency)
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
        gasCheck++;
    }
    while (gasCheck > 3)
    {
        printf("ALERTA CRÍTICO, AERONAVE %s IRÁ CAIR\n\n", fila->front->id);
        gasCheck--;
        (*NVoos)--;
        deQueue(fila);
    }
    if (gasCheck == 3)
    {
        printf("ALERTA GERAL DE DESVIO DE AERONAVE\n\n");
        return 1;
    }
    return emergency;
}

void deQueue(struct Fila *fila)
{
    fila->front = fila->front->next;

    if (fila->front == NULL)
    {
        fila->rear = NULL;
    }
    return;
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
    for (int i = 0; i < 64; i++)
    {
        vetor[i] = i;
    }
    for (int i = 0; i < 64; i++)
    {
        int temp = vetor[i];
        int randomIndex = rand() % 64;
        vetor[i] = vetor[randomIndex];
        vetor[randomIndex] = temp;
    }
    return vetor;
}