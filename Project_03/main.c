/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

//Bicliotecas utilizadas.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//Definição da struct Contato que representa cada nódulo da lista.
struct Contato
{
    char nome[102];
    char telefone[12];
    char endereco[102];
    unsigned int cep;
    char nascimento[13];
    struct Contato *next, *prev;
};

//Declaração do protótipo das funções utilizadas.
void insertionSort(struct Contato **);
void imprimirLista(struct Contato *);
void inserirLista(struct Contato **, char[], char[], char[], unsigned int, char[]);
void imprimirRegistrosEspecificos(struct Contato *);
void removerRegistrosEspecificos(struct Contato **);
void freeRegistros(struct Contato **);
void atualizarTxt(struct Contato *);
void atualizarLista(struct Contato **);
struct Contato *inserirRegistro();

//Início da função principal.
int main(int argc, char** argv)
{
    //Definição da head(começo da lista), auxiliar e arquivo.
    struct Contato *head = NULL;
    struct Contato *aux;
    FILE *Agenda;
    int escolha = 0;

    Agenda = fopen("contatos.txt", "r");
    //Caso arquivo não exista, cria o arquivo.
    if (Agenda == NULL)
    {
        printf("\nCriando uma nova agenda.\n\n");
    }
    //Caso arquivo exista, extrai os dados presentes no arquivo e armazena na lista.
    else
    {
        while (fgets(aux->nome, sizeof(aux->nome), Agenda) != NULL)
        {
            fgets(aux->telefone, sizeof(aux->telefone), Agenda);
            fgets(aux->endereco, sizeof(aux->endereco), Agenda);
            fscanf(Agenda, "%u\n", &aux->cep);
            fgets(aux->nascimento, sizeof(aux->nascimento), Agenda);
            inserirLista(&head, aux->nome, aux->telefone, aux->endereco, aux->cep, aux->nascimento);
            //Chamada de função extra para ignorar caractere ($) no arquvio.
            fgets(aux->nome, sizeof(aux->nome), Agenda);
        }
        fclose(Agenda);
        //Organiza a lista alfabeticamente de acordo com o nome.
        insertionSort(&head);
    }
    //Início do loop que define o menu do programa.
    do
    {
        printf("Funções Disponíveis:\n");
        printf("1. Inserir novo registro na agenda.\n");
        printf("2. Remover registros com certa string no nome.\n");
        printf("3. Visualizar registros com certa string no nome.\n");
        printf("4. Visualizar registros em ordem alfabética de acordo com o nome.\n");
        printf("5. Sair.\n");
        scanf("%d", &escolha);
        //Função utilizada para retirar "\n" do buffer de entrada.
        getchar();
        switch (escolha)
        {
        case 1:
            //Struct aux recebe valores inseridos pelo usuário.
            aux = inserirRegistro();
            //Valores da struct aux são inseridos ao final da lista.
            inserirLista(&head, aux->nome, aux->telefone, aux->endereco, aux->cep, aux->nascimento);
            //A lista é organizada alfabeticamente de acordo com o nome.
            insertionSort(&head);
            break;
        case 2:
            //Remove todos os registros que possuem a substring inserida.
            removerRegistrosEspecificos(&head);
            break;
        case 3:
            //Imprime todos os registros que possuem a substring inserida.
            imprimirRegistrosEspecificos(head);
            break;
        case 4:
            //Imprime a lista completa.
            imprimirLista(head);
            break;
        case 5:
            //Escreve os valores da lista no .txt;
            atualizarTxt(head);
            //Libera a memória alocada dinamicamente.
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

//Retorna uma struct com os valores inseridos pelo usuário.
//A maior parte das operações nessa função são referentes a validação das entradas.
struct Contato *inserirRegistro()
{
    bool leap = false;
    int i;
    struct Contato *novoContato;
    
    //A entrada de nome não aceita números.
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
    //A entrada de telefone aceita somente números e o caractere "-" na quinta posição do array.
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
                    printf("Entrada inválida, certifique-se de digitar um número no formato xxxxx-xxxx.\nDigite novamente: ");
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
    //A entrada de endereço aceita qualquer string.
    printf("Insira o endereço: ");
    fgets(novoContato->endereco, sizeof(novoContato->endereco), stdin);
    //A entrada de CEP aceita somente números.
    printf("Insira o CEP (somente dígitos): ");
    char cep_aux[100];
    while (i != strlen(cep_aux) - 1)
    {
        fgets(cep_aux, sizeof(cep_aux), stdin);
        for (i = 0; i < strlen(cep_aux) - 1; i++)
        {
            if (!isdigit(cep_aux[i]))
            {
                printf("Entrada inválida, certifique-se de digitar somente números positivos.\nDigite novamente: ");
                break;
            }
        }
    }
    novoContato->cep = atoi(cep_aux);
    //A entrada de nascimento aceita somente números e o caractere "/" em algumas posições.
    //A entrada será aceita e formatada somente se estiver nos formatos dd/mm/aaaa, ddmmaaaa, dd/mmaaaa ou ddmm/aaaa.
    //Além disso, só serão aceitas datas válidas/reais.
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
            {
                leap = true;
            }
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

//Organiza a lista alfabeticamente de acordo com o nome.
//A função cria uma lista vazia e transfere os valores de forma organizada.
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
        {
            sorted = current;
        }
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
            {
                aux = aux->next;
            }
            current->next = aux->next;
            if (aux->next != NULL)
            {
                current->next->prev = current;
            }
            aux->next = current;
            current->prev = aux;
        }
        current = next;
    }
    *head_ref = sorted;
}

//Libera a memória alocada dinamicamente.
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

//Imprime toda a lista organizada.
void imprimirLista(struct Contato *head)
{
    bool exists = false;
    while (head != NULL)
    {
        if (!exists)
        {
            printf("\nLista de contatos:\n\n");
            exists = true;
        }
        printf("Nome: %sTelefone: %sEndereço: %sCEP: %u\nData de nascimento: %s$\n", head->nome, head->telefone, head->endereco, head->cep, head->nascimento);
        head = head->next;
    }
    if (!exists)
    {
        printf("\nLista de contatos vazia.\n\n");
        return;
    }
    printf("\n");
}

//Insere um contato na lista.
void inserirLista(struct Contato **head_ref, char nome[], char telefone[], char endereco[], unsigned int cep, char nascimento[])
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

//Remove registros que contenham a substring fornecida.
void removerRegistrosEspecificos(struct Contato **head)
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

//Imprime registros que contenham a substring fornecida.
void imprimirRegistrosEspecificos(struct Contato *head)
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
            printf("Nome: %sTelefone: %sEndereço: %sCEP: %u\nData de nascimento: %s$\n", head->nome, head->telefone, head->endereco, head->cep, head->nascimento);
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

//Escreve os valores da lista no .txt.
void atualizarTxt(struct Contato *head)
{
    FILE *Agenda;
    Agenda = fopen("contatos.txt", "w+");
    while (head != NULL)
    {
        fprintf(Agenda, "%s%s%s%u\n%s$\n", head->nome, head->telefone, head->endereco, head->cep, head->nascimento);
        head = head->next;
    }
    fclose(Agenda);
}