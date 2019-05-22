#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Elemento
{
    char nome[102];
    char telefone[12];
    char endereco[102];
    unsigned int cep;
    char nascimento[13];
    struct Contato *proximo;
    struct Contato *anterior;
} Contato;
Contato *lista = NULL;

void inserirRegistro(Contato **);
void removerRegistrosEspecificos();
void imprimeRegistrosEspecificos();
void imprimeRegistrosAlfabeticamente(Contato *);
void insertionSort(Contato *, int);
void atualizarTxt(Contato *);

int main()
{
    int escolha = 0;
    do
    {
        printf("Funções Disponíveis:\n");
        printf("1. Inserir novo registro na agenda.\n");
        printf("2. Remover registros com certa string no nome.\n");
        printf("3. Visualizar registros com certa string no nome.\n");
        printf("4. Visualizar registros em ordem alfabética de acordo com o nome.\n");
        printf("5. Sair.\n");
        scanf("%d", &escolha);
        getchar();
        switch (escolha)
        {
        case 1:
            inserirRegistro(&lista);
            break;
        case 2: //removerRegistrosEspecificos(); break;
        case 3: imprimeRegistrosEspecificos(); break;
        case 4:
            imprimeRegistrosAlfabeticamente(lista);
            break;
        case 5:
            atualizarTxt(lista);
            break;
        default:
            printf("Entrada inválida.\n");
            break;
        }
    } while (escolha != 5);
    return 0;
}
void inserirRegistro(Contato **inicio)
{
    bool leap = false;
    int i;
    char confirma;
    int numeroRegistros = 1;
    Contato *novoContato;
    Contato *fim = *inicio;
    novoContato = (Contato *)malloc(sizeof(Contato));
    printf("Registre um novo contato.\nInsira o nome: ");
    for (i = 0; i < strlen(novoContato->nome) - 1;)
    {
        fgets(novoContato->nome, sizeof(novoContato->nome), stdin);
        for (i = 0; i < strlen(novoContato->nome) - 1; i++)
        {
            if (isdigit(novoContato->nome[i]))
            {
                i = 0;
                printf("Entrada inválida, certifique-se de utilizar somente caracteres válidos.\nDigite novamente: ");
                break;
            }
        }
    }
    printf("Insira o telefone (xxxxx-xxxx): ");
    for (i = 0; i < strlen(novoContato->telefone) - 1;)
    {
        fgets(novoContato->telefone, sizeof(novoContato->telefone), stdin);
        if (strlen(novoContato->telefone) - 1 < 9)
        {
            i = 0;
            printf("Entrada inválida, certifique-se de digitar um número no formato xxxxx-xxxx.\nDigite novamente: ");
            continue;
        }
        for (i = 0; i < strlen(novoContato->telefone) - 1; i++)
        {
            if (isdigit(novoContato->telefone[i]))
            {
                continue;
            }
            else
            {
                if (novoContato->telefone[i] == '-' && i == 5)
                    continue;
                else
                {
                    printf("Entrada inválida, certifique-se de digitar um número no formato xxxxx-xxxx.\n");
                    break;
                }
            }
        }
        if (novoContato->telefone[5] != '-' && i == strlen(novoContato->telefone) - 1)
        {
            for (; i > 4; i--)
            {
                novoContato->telefone[i + 1] = novoContato->telefone[i];
            }
            novoContato->telefone[i + 1] = '-';
            break;
        }
    }
    printf("Insira o endereço: ");
    fgets(novoContato->endereco, sizeof(novoContato->endereco), stdin);
    printf("Insira o CEP (somente dígitos): ");
    scanf("%d", &novoContato->cep);
    getchar();
    printf("Insira a data de nascimento no formato dd/mm/aaaa: ");
    for (i = 0; i < strlen(novoContato->nascimento) - 1;)
    {
        fgets(novoContato->nascimento, sizeof(novoContato->nascimento), stdin);
        if (strlen(novoContato->nascimento) - 1 < 8)
        {
            i = 0;
            printf("Entrada inválida, certifique-se de digitar uma data no formato dd/mm/aaaa.\nDigite novamente: ");
            continue;
        }
        for (i = 0; i < strlen(novoContato->nascimento) - 1; i++)
        {
            if (isdigit(novoContato->nascimento[i]))
            {
                continue;
            }
            else
            {
                if (novoContato->nascimento[i] == '/' && (i == 2 || i == 4 || i == 5))
                {
                    if (novoContato->nascimento[i] == '/' && novoContato->nascimento[i + 2] == '/')
                    {
                        printf("Entrada inválida, certifique-se de digitar uma data no formato dd/mm/aaaa.\nDigite novamente: ");
                        break;
                    }
                    else if (novoContato->nascimento[i] == '/' && novoContato->nascimento[i + 1] == '/')
                    {
                        printf("Entrada inválida, certifique-se de digitar uma data no formato dd/mm/aaaa.\nDigite novamente: ");
                        break;
                    }
                    continue;
                }
                else
                {
                    printf("Entrada inválida, certifique-se de digitar uma data no formato dd/mm/aaaa.\nDigite novamente: ");
                    break;
                }
            }
        }
        int temp = i;
        if (novoContato->nascimento[2] != '/' && i == strlen(novoContato->nascimento) - 1)
        {
            for (; i > 1; i--)
            {
                novoContato->nascimento[i + 1] = novoContato->nascimento[i];
            }
            novoContato->nascimento[i + 1] = '/';
            if (novoContato->nascimento[5] != '/')
            {
                for (; temp > 3; temp--)
                {
                    novoContato->nascimento[temp + 2] = novoContato->nascimento[temp + 1];
                }
                novoContato->nascimento[temp + 2] = '/';
            }
        }
        if (novoContato->nascimento[5] != '/' && i == strlen(novoContato->nascimento) - 1)
        {

            for (; temp > 4; temp--)
            {
                novoContato->nascimento[temp + 1] = novoContato->nascimento[temp];
            }
            novoContato->nascimento[temp + 1] = '/';
        }
        int day, month, year;
        char *divide;
        char nascimento[13];
        strcpy(nascimento, novoContato->nascimento);
        divide = strtok(nascimento, "/");
        for (temp = 0; divide != NULL; temp++)
        {
            if (temp == 0)
            {
                day = atoi(divide);
            }
            else if (temp == 1)
            {
                month = atoi(divide);
            }
            else
            {
                year = atoi(divide);
            }
            divide = strtok(NULL, "/");
        }
        if (month < 1 || day < 1 || day > 31 || month > 12 || year < 1903 || year > 2018)
        {
            printf("\nData inválida, digite novamente: ");
            i = 0;
            continue;
        }
        else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        {
            printf("\nData inválida, digite novamente: ");
            i = 0;
            continue;
        }
        if (year % 4 == 0)
        {
            if (year % 100 == 0)
            {
                if (year % 400 == 0)
                {
                    leap = true;
                }
            }
            else
                leap = true;
        }
        if (leap && month == 2 && day > 29)
        {
            printf("\nData inválida, digite novamente: ");
            i = 0;
            continue;
        }
        else if (!leap && month == 2 && day > 28)
        {
            printf("\nData inválida, digite novamente: ");
            i = 0;
            continue;
        }
        break;
    }
    printf("Contato registrado com sucesso.\n\n\n");
    novoContato->proximo = NULL;
    if (*inicio == NULL)
    {
        novoContato->anterior = NULL;
        *inicio = novoContato;

    }
    while (fim->proximo != NULL)
    {
        fim = fim->proximo;
    }
    fim->proximo = novoContato;
    novoContato->anterior = fim;
    numeroRegistros++;
    insertionSort(novoContato, numeroRegistros);
}

void imprimeRegistrosEspecificos()
{
    char stringEspecifica[100];
    int aux = 1;
    printf("\nEscolha a string específica:\n");
    scanf("%s", stringEspecifica);
    char c[1000];
    char linhas[1000];
    int i = 0;
    FILE *Agenda;
    Agenda = fopen("contatos.txt", "r");
    printf("Registros com a string procurada:\n");
    while ((fgets(c, sizeof(c), Agenda)) != NULL)
    {
        if ((strstr(c, stringEspecifica)) != NULL)
        {
            printf("%s", c);
            i = aux;
            for (i = aux; i < aux + 5; i++)
            {
                fgets(linhas, sizeof(linhas), Agenda);
                printf("%s", linhas);
            }
        }
        aux++;
    }
    fclose(Agenda);
}

// void insertionSort(Contato *novoContato, int numeroRegistros){
//   printf("Insertion sort de %s\n", novoContato->nome);
//   printf("Numero de registros: %d\n", numeroRegistros);
//   Contato *contatoOrdenado = novoContato;
//   for(int i=0;i<numeroRegistros;i++){
//       while(strcmp(novoContato, contatoOrdenado)>0 && novoContato != NULL){
//           contatoOrdenado = ;
//       }
//   }
// }

void imprimeRegistrosAlfabeticamente(Contato *lista)
{
    Contato *fim;
    while (lista != NULL)
    {
        printf("%s%s%s%d\n%s$\n", lista->nome, lista->telefone, lista->endereco, lista->cep, lista->nascimento);
        fim = lista;
        lista = lista->proximo;
    }
}

void atualizarTxt(Contato *lista)
{
    FILE *Agenda;
    Contato *aux;
    Agenda = fopen("contatos.txt", "w+");
    while (lista != NULL)
    {
        aux = lista;
        fprintf(Agenda, "%s%s%s%d\n%s$\n", aux->nome, aux->telefone, aux->endereco, aux->cep, aux->nascimento);
        lista = lista->proximo;
    }
    fclose(Agenda);
}