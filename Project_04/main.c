#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/*

ARRUMAR A RANDOMIZAÇÃO DE COMBUSTÍVEL

*/

struct Fila
{
    struct Plane *inicio;
    struct Plane *fim;
};

struct Plane
{
    char id[6];
    int gas;
    char mode;
    struct Plane *next;
};

void display(struct Plane *);

int main(int argc, char **argv)
{
    
    srand(time(NULL));
    printf("%d\n", rand());printf("%d\n", rand());

    struct Fila *fila = (struct Fila *)malloc(sizeof(struct Fila));
    fila->inicio = fila->fim = NULL;

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
    int NVoos = rand() % 45 + 20;
    int NAprox = rand() % (NVoos - 9) + 10;
    int NDec;
    if (NAprox > 32)
        NAprox = 32;
    if ((NVoos - NAprox) < 10)
    {
        NDec = 10;
        NAprox = NVoos - 10;
    }
    else
    {
        NDec = NVoos - NAprox;
    }
    if (NDec > 32)
    {
        NDec = 32;
        NAprox = NVoos - NDec;
    }
    printf("Quantidade de\nVoos: %d - Aproximações: %d - Decolagens: %d\n", NVoos, NAprox, NDec);
    int fuel[64] = {0};
    for (int i = 0; i < 13; i++)
    {
        fuel[i] = i;
    }
    int type[64] = {0};
    for (int i = 0; i < 64; i++)
    {
        type[i] = i % 2;
    }

    int random[64] = {0};
    for (int i = 0; i < 64; i++)
    {
        random[i] = (i);
    }
    for (int i = 0; i < 64; i++)
    {
        int temp = random[i];
        int randomIndex = rand() % 64;
        random[i] = random[randomIndex];
        random[randomIndex] = temp;
    }

    for (int i = 0; i < NVoos; i++)
    {
        struct Plane *newPlane = (struct Plane *)malloc(sizeof(struct Plane));
        strcpy(newPlane->id, idVoos[random[i]]);
        if (type[random[i]] == 0 && NAprox != 0)
        {
            newPlane->mode = 'A';
            NAprox--;
        }
        else if (NDec != 0)
        {
            newPlane->mode = 'D';
            NDec--;
        }
        else
        {
            newPlane->mode = 'A';
        }
        if (newPlane->mode == 'A')
        {
            if (random[i] > 12)
            {
                newPlane->gas = fuel[random[i] % 13];
            }
            else
            {
                newPlane->gas = fuel[random[i]];
            }
        }
        else
        {
            newPlane->gas = -1;
        }
        if (fila->inicio == NULL)
        {
            fila->inicio = fila->fim = newPlane;
            continue;
        }
        fila->inicio->next = newPlane;
        fila->inicio = newPlane;
    }
    display(fila->fim);
    return 0;
}

void display(struct Plane *head)
{
    if (head == NULL)
    {
        printf("Final da fila.\n");
        return;
    }
    else
    {
        printf("ID: %s - Tipo: %c - Combustível: %d\n", head->id, head->mode, head->gas);
        display(head->next);
    }
}