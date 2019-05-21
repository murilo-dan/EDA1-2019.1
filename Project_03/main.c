#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Elemento
{
    char nome[100];
    char telefone[10];
    char endereco[100];
    unsigned int cep;
    char nascimento[10];
    struct Contato *proximo;
    struct Contato *anterior;
} Contato;

typedef struct Lista
{
    Contato *inicio;
    Contato *fim;
} ListaA;

void criaListaVazia(ListaA *l);
void inserirRegistro(ListaA *l);
void removerRegistrosEspecificos(ListaA *l);
void imprimeRegistrosEspecificos(ListaA *l);
void imprimeRegistrosAlfabeticamente();
void insertionSort(Contato *c, ListaA *l, int n);

int main()
{
    int escolha = 0;
    ListaA *list;
    criaListaVazia(list);
    do
    {
        printf("\nFunções Disponíveis:\n");
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
            inserirRegistro(list);
            break;
        case 2:
          //removerRegistrosEspecificos();
          //break;
        case 3:
          imprimeRegistrosEspecificos(list);
          break;
        case 4:
          //imprimeRegistrosAlfabeticamente();
          //break;
        case 5:
            break;
        default:
            printf("Entrada inválida.\n");
            break;
        }
    } while (escolha != 5);
    return 0;
}

void criaListaVazia(ListaA *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
}

void inserirRegistro(ListaA *lista)
{
    int numeroRegistros = 0;
    Contato *novoContato;
    FILE *Agenda;
    novoContato = (Contato *)malloc(sizeof(Contato));
    printf("Registre um novo contato.\nInsira o nome: ");
    fgets(novoContato->nome, sizeof(novoContato->nome), stdin);
    printf("Insira o telefone: ");
    fgets(novoContato->telefone, sizeof(novoContato->telefone), stdin);
    for(int i = 0; i < strlen(novoContato->telefone);i++){
        if((novoContato->telefone[i] > '9' || novoContato->telefone[i] < '0') && novoContato->telefone[5] != '-'){
            printf("Entrada invalida\n");
        }
    }
    printf("Insira o endereço: ");
    fgets(novoContato->endereco, sizeof(novoContato->endereco), stdin);
    printf("Insira o CEP: ");
    scanf("%d", &novoContato->cep);
    getchar();
    printf("Insira a data de nascimento no formato dd/mm/aaaa: ");
    fgets(novoContato->nascimento, sizeof(novoContato->nascimento), stdin);
    Agenda = fopen("contatos.txt", "a+");
    fprintf(Agenda, "%s%s%s%d\n%s$\n", novoContato->nome, novoContato->telefone, novoContato->endereco, novoContato->cep, novoContato->nascimento);
    fclose(Agenda);
    novoContato->proximo = NULL;
    novoContato->anterior = NULL;
    numeroRegistros++;
    if (lista->inicio == NULL)
    {
        lista->inicio = novoContato;
        lista->fim = novoContato;
    }
     else
     {
         lista->fim->proximo = novoContato;
         novoContato->anterior = lista->fim;
         lista->fim = novoContato;
        // insertionSort(novoContato, lista, numeroRegistros);
     }
}

void imprimeRegistrosEspecificos(ListaA *lista){
  char stringEspecifica[100];
  int aux = 1;
  printf("\nEscolha a string específica:\n");
  scanf("%s",stringEspecifica);
  char c[1000];
  char linhas[1000];
  int i = 0;
  FILE *Agenda;
  Agenda = fopen("contatos.txt", "r");
  printf("Registros com a string procurada:\n");
    while((fgets(c, sizeof(c), Agenda))!= NULL){
      if((strstr(c, stringEspecifica))!= NULL){
        printf("%s", c);
        printf("%d\n", aux);
        i = aux;
        for(i=aux;i<aux+5;i++){
          fgets(linhas, sizeof(linhas), Agenda);
          printf("%s",linhas );
        }
      }
      aux++;
    }
  fclose(Agenda);
}
/*void insertionSort(Contato *novoContato, ListaA *lista, int numeroRegistros){
  FILE *Agenda;
  printf("Insertion sort de %s\n", novoContato->nome);
  free(contatoOrdenado);
}*/
