#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct node *loadTreeFromFile(char *);
void insertOnTree(struct node **, int);
void display(struct node *);
void showTree();
void isFull();
int searchValue(struct node *, int);
void getHeight();
void removeValue();
void printInOrder();
void printPreOrder();
void printPostOrder();
void balanceTree();

int main()
{
    struct node *head = NULL;
    int target;
    int escolha;
    char fileName[10];
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
            printf("\nInsira o nome do arquivo: ");
            fscanf(stdin, "%s", fileName);
            head = loadTreeFromFile(fileName);
            break;
        case 2:
            showTree();
            break;
        case 3:
            isFull();
            break;
        case 4:
            printf("\nDigite o valor a ser buscado: ");
            scanf("%d", &target);
            printf("\n");
            target = searchValue(head, target);
            if (target == 0)
            {
                break;
            }
            else
            {
                printf("Nível do valor encontrado: %d\n", target);
            }
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
            printf("\nEntrada inválida.\n");
            break;
        }
    } while (escolha != 11);
    return 0;
}

struct node *loadTreeFromFile(char *fileName)
{
    struct node *head = NULL;
    int aux;
    FILE *fp;
    for (int i = strlen(fileName); fileName[i] != '.'; i--)
    {
    }
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("\nArquivo não encontrado.\n");
        return head;
    }
    while (!feof(fp))
    {
        fscanf(fp, "%d", &aux);
        insertOnTree(&head, aux);
    }
    fclose(fp);
    return head;
}

void display(struct node *head)
{
    if (head != NULL)
    {
        display(head->left);
        printf("%d\n", head->data);
        display(head->right);
    }
}

void insertOnTree(struct node **head, int num)
{
    if ((*head) == NULL)
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

int searchValue(struct node *head, int target)
{
    int height = 1;
    if (head == NULL)
    {
        printf("Valor não encontrado, árvore não existe.\n");
        return 0;
    }
    else if (head->data == target)
    {
        printf("Valor do pai: NULO\n");
        printf("Valor do irmão: NULO\n");
        return 1;
    }
    else if (head->data > target)
    {
        if (head->left != NULL)
        {
            if (head->left->data == target)
            {
                printf("Valor do pai: %d\n", head->data);
                if (head->right != NULL)
                {
                    printf("Valor do irmão: %d\n", head->right->data);
                }
                else
                {
                    printf("Valor do irmão: NULO\n");
                }
                return 2;
            }
            else
            {
                int aux = height;
                height += searchValue(head->left, target);
                if (aux == height)
                {
                    return 0;
                }
                return height;
            }
        }
        else
        {
            printf("Valor não encontrado.\n");
            return 0;
        }
    }
    else
    {
        if (head->right != NULL)
        {
            if (head->right->data == target)
            {
                printf("Valor do pai: %d\n", head->data);
                if (head->left != NULL)
                {
                    printf("Valor do irmão: %d\n", head->left->data);
                }
                else
                {
                    printf("Valor do irmão: NULO\n");
                }
                return 2;
            }
            else
            {
                int aux = height;
                height += searchValue(head->right, target);
                if (aux == height)
                {
                    return 0;
                }
                return height;
            }
        }
        else
        {
            printf("Valor não encontrado.\n");
            return 0;
        }
    }
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
