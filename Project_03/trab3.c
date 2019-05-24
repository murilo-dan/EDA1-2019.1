#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct Contato
{
    char nome[102];
    char telefone[12];
    char endereco[102];
    unsigned int cep;
    char nascimento[13];
    struct Contato *next, *prev;
};

void insertionSort(struct Contato **);
void printList(struct Contato *);
void push(struct Contato **, char[], char[], char[], unsigned int, char[]);
void imprimeRegistrosEspecificos(struct Contato *);
void removeRegistrosEspecificos(struct Contato **);
void freeRegistros(struct Contato **);
struct Contato *inserirRegistro();

int main()
{
    struct Contato *head = NULL;
    struct Contato *aux;
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
            aux = inserirRegistro();
            push(&head, aux->nome, aux->telefone, aux->endereco, aux->cep, aux->nascimento);
            insertionSort(&head);
            break;
        case 2:
            removeRegistrosEspecificos(&head);
            break;
        case 3:
            imprimeRegistrosEspecificos(head);
            break;
        case 4:
            printList(head);
            break;
        case 5:
            freeRegistros(&head);
            printf("\nFechando agenda.\n\n");
            break;
        default:
            printf("\nEntrada inválida.\n\n");
            break;
        }
    } while (escolha != 5);
    return 0;
}

struct Contato *inserirRegistro()
{
    bool leap = false;
    int i;
    struct Contato *novoContato = (struct Contato *)malloc(sizeof(struct Contato));
    printf("\nRegistre um novo contato.\nInsira o nome: ");
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
        char bd[13];
        strcpy(bd, novoContato->nascimento);
        divide = strtok(bd, "/");
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
    printf("\nContato registrado com sucesso.\n\n");
    return novoContato;
}

void insertionSort(struct Contato **head_ref)
{
    struct Contato *sorted = NULL;

    struct Contato *current = *head_ref;

    while (current != NULL)
    {
        struct Contato *next = current->next;
        current->prev = current->next = NULL;
        struct Contato *aux;

        if (sorted == NULL)
            sorted = current;

        else if (strcmp((sorted)->nome, current->nome) >= 0)
        {
            current->next = sorted;
            current->next->prev = current;
            sorted = current;
        }

        else
        {
            aux = sorted;

            while (aux->next != NULL && (strcmp(aux->next->nome, current->nome) < 0))
                aux = aux->next;

            current->next = aux->next;

            if (aux->next != NULL)
                current->next->prev = current;

            aux->next = current;
            current->prev = aux;
        }
        current = next;
    }
    *head_ref = sorted;
}

void freeRegistros(struct Contato **head)
{
    struct Contato *current = *head;
    while (current != NULL)
    {
        struct Contato *next = current->next;
        free(current);
        current = next;
    }
}

void printList(struct Contato *head)
{
    bool exists = false;
    while (head != NULL)
    {
        if (!exists)
        {
            printf("\nLista de contatos:\n\n");
            exists = true;
        }
        printf("Nome: %sTelefone: %sEndereço: %sCEP: %d\nData de nascimento: %s$\n", head->nome, head->telefone, head->endereco, head->cep, head->nascimento);
        head = head->next;
    }
    if (!exists)
    {
        printf("\nLista de contatos vazia.\n\n");
        return;
    }
    printf("\n");
}

void push(struct Contato **head_ref, char nome[], char telefone[], char endereco[], unsigned int cep, char nascimento[])
{
    struct Contato *newNode = (struct Contato *)malloc(sizeof(struct Contato));

    strcpy(newNode->nome, nome);
    strcpy(newNode->telefone, telefone);
    strcpy(newNode->endereco, endereco);
    newNode->cep = cep;
    strcpy(newNode->nascimento, nascimento);

    newNode->next = (*head_ref);
    newNode->prev = NULL;

    if ((*head_ref) != NULL)
        (*head_ref)->prev = newNode;

    (*head_ref) = newNode;
}

void removeRegistrosEspecificos(struct Contato **head)
{
    struct Contato *del = (*head);
    bool exists = false;
    char stringEspecifica[102];
    printf("\nEscolha a string específica: ");
    scanf("%s", stringEspecifica);
    while (del != NULL)
    {
        if (strstr(del->nome, stringEspecifica) != NULL)
        {
            if (*head == del)
            {
                *head = del->next;
            }
            if (del->next != NULL)
            {
                del->next->prev = del->prev;
            }
            if (del->prev != NULL)
            {
                del->prev->next = del->next;
            }
            free(del);
            if (!exists)
            {
                printf("\nRegistros deletados!\n\n");
                exists = true;
            }
        }
        del = del->next;
    }
    if (!exists)
    {
        printf("\nNenhum registro encontrado com a string dada.\n\n");
        return;
    }
}

void imprimeRegistrosEspecificos(struct Contato *head)
{
    bool exists = false;
    char stringEspecifica[102];
    printf("\nEscolha a string específica: ");
    scanf("%s", stringEspecifica);
    while (head != NULL)
    {
        if (strstr(head->nome, stringEspecifica) != NULL)
        {
            if (!exists)
            {
                printf("\nRegistros com a string procurada:\n\n");
                exists = true;
            }
            printf("Nome: %sTelefone: %sEndereço: %sCEP: %d\nData de nascimento: %s$\n", head->nome, head->telefone, head->endereco, head->cep, head->nascimento);
        }
        head = head->next;
    }
    if (!exists)
    {
        printf("\nNenhum registro encontrado com a string dada.\n\n");
        return;
    }
    printf("\n");
}