#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

void printTree(struct node *);
struct node *build123a();
struct node *loadTreeFromFile();
void insertOnTree(struct node **, int);
void display(struct node *);
void showTree();
void isFull();
void searchValue(struct node *, int);
void getHeight();
void removeValue();
void printInOrder();
void printPreOrder();
void printPostOrder();
void balanceTree();

int main()
{
    struct node *tree = NULL;
    int escolha;
    do
    {
        printf("\nFunções Disponíveis:\n");
        printf("1. Carregar árvore de arquivo.\n");
        printf("2. Mostrar árvore.\n");
        printf("3. Mostrar se a árvore está cheia.\n");
        printf("4. Buscar valor na árvore.\n");
        printf("5. Mostrar altura da árvore.\n");
        printf("6. Remover valor da árvore.\n");
        printf("7. Mostrar árvore in order.\n");
        printf("8. Mostrar árvore pre order.\n");
        printf("9. Mostrar árvore post order.\n");
        printf("10. Balancear árvore.\n");
        printf("11. Sair.\n");
        printf("Entrada: ");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            tree = loadTreeFromFile();
            break;
        case 2:
            showTree();
            break;
        case 3:
            isFull();
            break;
        case 4:
            printTree(build123a());
            //searchValue();
            break;
        case 5:
            getHeight();
            break;
        case 6:
            removeValue();
            break;
        case 7:
            printInOrder();
            break;
        case 8:
            printPreOrder();
            break;
        case 9:
            printPostOrder();
            break;
        case 10:
            balanceTree();
            break;
        case 11:
            printf("\nFechando o programa.\n\n");
            break;
        default:
            printf("\nEntrada inválida.\n\n");
            break;
        }
    } while (escolha != 11);
    return 0;
}

void printTree(struct node *node)
{
    if (node == NULL)
        return;
    printTree(node->left);
    printf("%d ", node->data);
    printTree(node->right);
}

struct node *newNode(int data)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return (node);
}

struct node *build123a()
{
    struct node *root = newNode(2);
    struct node *lChild = newNode(1);
    struct node *rChild = newNode(3);
    root->left = lChild;
    root->right = rChild;

    return (root);
}

struct node *loadTreeFromFile()
{
    struct node *head = NULL;
    int aux;
    char fileName[10];
    FILE *fp;
    printf("\nInsira o nome do arquivo: ");
    fscanf(stdin, "%s", fileName);
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("\nArquivo não encontrado.\n");
    }
    else
    {
      while (!feof(fp))
      {
          fscanf(fp, "%d", &aux);
          insertOnTree(&head, aux);
      }
      fclose(fp);
      display(head);
    }
}

void display(struct node *head)
{
    if (head != NULL)
    {
        display(head->left);
        printf("%d ", head->data);
        display(head->right);
    }
}

void insertOnTree(struct node **head, int num)
{
    if (*head == NULL)
    {
        *head = malloc(sizeof(**head));
        (*head)->data = num;
        (*head)->left = NULL;
        (*head)->right = NULL;
    }
    else
    {
        if (num < (*head)->data)
        {
            insertOnTree(&(*head)->left, num);
        }
        if (num > (*head)->data)
        {
            insertOnTree(&(*head)->right, num);
        }
    }
}

void showTree()
{
}
void isFull()
{
}

void searchValue(struct node *node, int data)
{
}

void getHeight()
{
}

void removeValue()
{
}

void printInOrder()
{
}

void printPreOrder()
{
}

void printPostOrder()
{
}

void balanceTree()
{
}
