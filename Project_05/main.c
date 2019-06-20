#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define alturaMaxima 1000
#define INFINITO (1 << 20)

int esquerda[alturaMaxima];
int direita[alturaMaxima];
int distancia = 3;
int printNext;

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct desenhoArvore
{
    int larguraBarra;
    int height;
    int labelSize;
    int direcaoPai;
    char label[11];
    struct desenhoArvore *left;
    struct desenhoArvore *right;
};

//Funções principais.
struct node *loadTreeFromFile(char *);
void showTree(struct node *);
int removeValue(struct node *, int);
void printInOrder(struct node *);
void printPreOrder(struct node *);
void printPostOrder(struct node *);
void balanceTree(struct node *);
bool isFull(struct node *);
int searchValue(struct node *, int);
int getHeight(struct node *);

//Funções auxiliares.
struct desenhoArvore *desenharArvore(struct node *);
struct desenhoArvore *desenharArvoreRecursivamente(struct node *);
struct node *findLesserChild(struct node *);
struct node *removeFromTree(struct node *, int);
void balanceTreeAfterBackbone(struct node *, int);
void insertOnTree(struct node **, int);
void calcTamanhoExtremidades(struct desenhoArvore *);
void calcEsquerda(struct desenhoArvore *, int, int);
void calcDireita(struct desenhoArvore *, int, int);
void printLevel(struct desenhoArvore *, int, int);
void rotacaoDireita(struct node *);
void rotacaoEsquerda(struct node *);
bool isBalanced(struct node *);
int MIN(int, int);
int MAX(int, int);

int main()
{
    struct node *head = NULL;
    struct node *temp = NULL;
    int target;
    int target1;
    int escolha;
    int height = 0;
    bool fullCheck;
    char fileName[100];
    do
    {
        printf("\nFunções disponíveis:\n\n");
        printf("1. Carregar árvore a partir de arquivo.\n");
        printf("2. Mostrar árvore.\n");
        printf("3. Mostrar se a árvore é cheia.\n");
        printf("4. Buscar valor na árvore.\n");
        printf("5. Mostrar altura da árvore.\n");
        printf("6. Remover valor da árvore.\n");
        printf("7. Mostrar árvore in order.\n");
        printf("8. Mostrar árvore pre order.\n");
        printf("9. Mostrar árvore post order.\n");
        printf("10. Balancear árvore.\n");
        printf("11. Sair.\n");
        printf("\nEntrada: ");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            printf("\nInsira o nome do arquivo: ");
            fscanf(stdin, "%s", fileName);
            head = loadTreeFromFile(fileName);
            break;
        case 2:
            printf("\n");
            showTree(head);
            break;
        case 3:
            fullCheck = isFull(head);
            if (head == NULL)
            {
                break;
            }
            if (fullCheck == true)
            {
                printf("\nA árvore é cheia.\n");
            }
            else
            {
                printf("\nA árvore não é cheia.\n");
            }
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
            printf("\n");
            height = getHeight(head);
            printf("%d\n", height);
            break;
        case 6:
            printf("\nDigite o valor a ser removido: ");
            scanf("%d", &target1);
            printf("\n");
            target1 = removeValue(head, target1);
            if (target1 == 0)
            {
                break;
            }
            else
            {
                printf("Valor removido com sucesso.\n");
            }
            break;
        case 7:
            printf("\n");
            printInOrder(head);
            printf("\n");
            break;
        case 8:
            printf("\n");
            printPreOrder(head);
            printf("\n");
            break;
        case 9:
            printf("\n");
            printPostOrder(head);
            printf("\n");
            break;
        case 10:
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            else
            {
                if (isBalanced(head))
                {
                    printf("\nÁrvore já está balanceada.\n");
                    break;
                }
                else
                {
                    balanceTree(head);
                    printf("\nÁrvore foi balanceada.\n");
                    break;
                }
            }
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
    if (strstr(fileName, ".txt") == NULL)
    {
        strcat(fileName, ".txt");
    }
    struct node *head = NULL;
    int aux;
    FILE *fp;
    char file[100] = "trees/";
    strcat(file, fileName);
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("\nArquivo não encontrado.\n");
        return NULL;
    }
    else
    {
        while (!feof(fp))
        {
            fscanf(fp, "%d", &aux);
            insertOnTree(&head, aux);
        }
    }
    fclose(fp);
    printf("\nÁrvore carregada.\n");
    return head;
}

void showTree(struct node *head)
{
    struct desenhoArvore *temp;
    int xmin, i;
    if (head == NULL)
    {
        return;
    }
    temp = desenharArvore(head);
    calcTamanhoExtremidades(temp);
    for (i = 0; i < temp->height && i < alturaMaxima; i++)
    {
        esquerda[i] = INFINITO;
    }
    calcEsquerda(temp, 0, 0);
    xmin = 0;
    for (i = 0; i < temp->height && i < alturaMaxima; i++)
    {
        xmin = MIN(xmin, esquerda[i]);
    }
    for (i = 0; i < temp->height; i++)
    {
        printNext = 0;
        printLevel(temp, -xmin, i);
        printf("\n");
    }
    if (temp->height >= alturaMaxima)
    {
        printf("erro\n");
    }
}

int removeValue(struct node *head, int target)
{
    if (head == NULL)
    {
        printf("\nÁrvore não existe.\n");
        return 0;
    }
    else
    {
        head = removeFromTree(head, target);
        return 1;
    }
}

bool isFull(struct node *head)
{
    if (head == NULL)
    {
        printf("\nÁrvore não existe.\n");
    }
    else
    {
        if (head->left == NULL && head->right == NULL)
        {
            return true;
        }
        if ((head->left) && (head->right))
        {
            return (isFull(head->left) && isFull(head->right));
        }
        return false;
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

int getHeight(struct node *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int left = getHeight(head->left);
    int right = getHeight(head->right);
    if (left > right)
    {
        return left + 1;
    }
    else
    {
        return right + 1;
    }
}

void printInOrder(struct node *head)
{
    if (head != NULL)
    {
        printInOrder(head->left);
        printf("%d ", head->data);
        printInOrder(head->right);
    }
}

void printPreOrder(struct node *head)
{
    if (head != NULL)
    {
        printf("%d ", head->data);
        printPreOrder(head->left);
        printPreOrder(head->right);
    }
}

void printPostOrder(struct node *head)
{
    if (head != NULL)
    {
        printPostOrder(head->left);
        printPostOrder(head->right);
        printf("%d ", head->data);
    }
}

void balanceTree(struct node *head)
{
    struct node *current = head;
    int nodeCount = 0;

    while (current != NULL)
    {
        while (current->left != NULL)
        {
            rotacaoDireita(current);
        }
        current = current->right;
        nodeCount++;
    }

    int expected = ceil(log2(nodeCount)) - nodeCount;
    struct node *aux = head;

    for (int i = 0; i < expected; i++)
    {
        if (i == 0)
        {
            rotacaoEsquerda(aux);
            aux = head;
        }
        else
        {
            rotacaoEsquerda(aux->right);
            aux = aux->right;
        }
    }

    while (nodeCount > 1)
    {
        nodeCount = nodeCount / 2;
        rotacaoEsquerda(head);
        aux = head;
        for (int i = 0; i < nodeCount - 1; i++)
        {
            rotacaoEsquerda(aux->right);
            aux = aux->right;
        }
    }
}

//Daqui em diante, somente funções auxiliares.

void rotacaoDireita(struct node *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->left == NULL)
    {
        return;
    }
    int temp = node->data;
    node->data = node->left->data;
    node->left->data = temp;

    struct node *aux = node->left;
    node->left = aux->left;
    aux->left = aux->right;
    aux->right = node->right;
    node->right = aux;

    return;
}

void rotacaoEsquerda(struct node *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->right == NULL)
    {
        return;
    }
    int temp = node->data;
    node->data = node->right->data;
    node->right->data = temp;

    struct node *aux = node->right;
    node->right = aux->right;
    aux->right = aux->left;
    aux->left = node->left;
    node->left = aux;

    return;
}

bool isBalanced(struct node *node)
{
    if (node == NULL)
    {
        return true;
    }

    int left_height = getHeight(node->left);
    int right_height = getHeight(node->right);

    if (abs(left_height - right_height) <= 1 && isBalanced(node->left) && isBalanced(node->right))
    {
        return true;
    }
    return false;
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

int MIN(int X, int Y)
{
    return ((X) < (Y)) ? (X) : (Y);
}

int MAX(int X, int Y)
{
    return ((X) > (Y)) ? (X) : (Y);
}

void printLevel(struct desenhoArvore *node, int x, int level)
{
    int i, isleft;
    if (node == NULL)
        return;
    isleft = (node->direcaoPai == -1);
    if (level == 0)
    {
        for (i = 0; i < (x - printNext - ((node->labelSize - isleft) / 2)); i++)
        {
            printf(" ");
        }
        printNext += i;
        printf("%s", node->label);
        printNext += node->labelSize;
    }
    else if (node->larguraBarra >= level)
    {
        if (node->left != NULL)
        {
            for (i = 0; i < (x - printNext - (level)); i++)
            {
                printf(" ");
            }
            printNext += i;
            printf("/");
            printNext++;
        }
        if (node->right != NULL)
        {
            for (i = 0; i < (x - printNext + (level)); i++)
            {
                printf(" ");
            }
            printNext += i;
            printf("\\");
            printNext++;
        }
    }
    else
    {
        printLevel(node->left, x - node->larguraBarra - 1, level - node->larguraBarra - 1);
        printLevel(node->right, x + node->larguraBarra + 1, level - node->larguraBarra - 1);
    }
}

struct desenhoArvore *desenharArvore(struct node *head)
{
    struct desenhoArvore *node;
    if (head == NULL)
    {
        return NULL;
    }
    node = desenharArvoreRecursivamente(head);
    node->direcaoPai = 0;
    return node;
}

struct desenhoArvore *desenharArvoreRecursivamente(struct node *head)
{
    struct desenhoArvore *node;
    if (head == NULL)
    {
        return NULL;
    }
    node = malloc(sizeof(struct desenhoArvore));
    node->left = desenharArvoreRecursivamente(head->left);
    node->right = desenharArvoreRecursivamente(head->right);
    if (node->left != NULL)
    {
        node->left->direcaoPai = -1;
    }
    if (node->right != NULL)
    {
        node->right->direcaoPai = 1;
    }

    sprintf(node->label, "%d", head->data);
    node->labelSize = strlen(node->label);

    return node;
}

void calcTamanhoExtremidades(struct desenhoArvore *node)
{
    int h, hmin, i, delta;
    if (node == NULL)
        return;
    calcTamanhoExtremidades(node->left);
    calcTamanhoExtremidades(node->right);

    if (node->right == NULL && node->left == NULL)
    {
        node->larguraBarra = 0;
    }
    else
    {
        if (node->left != NULL)
        {
            for (i = 0; i < node->left->height && i < alturaMaxima; i++)
            {
                direita[i] = -INFINITO;
            }
            calcDireita(node->left, 0, 0);
            hmin = node->left->height;
        }
        else
        {
            hmin = 0;
        }
        if (node->right != NULL)
        {
            for (i = 0; i < node->right->height && i < alturaMaxima; i++)
            {
                esquerda[i] = INFINITO;
            }
            calcEsquerda(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++)
        {
            delta = MAX(delta, distancia + 1 + direita[i] - esquerda[i]);
        }

        if (((node->left != NULL && node->left->height == 1) ||
             (node->right != NULL && node->right->height == 1)) &&
            delta > 4)
        {
            delta--;
        }

        node->larguraBarra = ((delta + 1) / 2) - 1;
    }

    h = 1;
    if (node->left != NULL)
    {
        h = MAX(node->left->height + node->larguraBarra + 1, h);
    }
    if (node->right != NULL)
    {
        h = MAX(node->right->height + node->larguraBarra + 1, h);
    }
    node->height = h;
}

void calcEsquerda(struct desenhoArvore *node, int x, int y)
{
    int i, isleft;
    if (node == NULL)
    {
        return;
    }
    isleft = (node->direcaoPai == -1);
    esquerda[y] = MIN(esquerda[y], x - ((node->labelSize - isleft) / 2));
    if (node->left != NULL)
    {
        for (i = 1; i <= node->larguraBarra && y + i < alturaMaxima; i++)
        {
            esquerda[y + i] = MIN(esquerda[y + i], x - i);
        }
    }
    calcEsquerda(node->left, x - node->larguraBarra - 1, y + node->larguraBarra + 1);
    calcEsquerda(node->right, x + node->larguraBarra + 1, y + node->larguraBarra + 1);
}

void calcDireita(struct desenhoArvore *node, int x, int y)
{
    int i, notleft;
    if (node == NULL)
    {
        return;
    }
    notleft = (node->direcaoPai != -1);
    direita[y] = MAX(direita[y], x + ((node->labelSize - notleft) / 2));
    if (node->right != NULL)
    {
        for (i = 1; i <= node->larguraBarra && y + i < alturaMaxima; i++)
        {
            direita[y + i] = MAX(direita[y + i], x + i);
        }
    }
    calcDireita(node->left, x - node->larguraBarra - 1, y + node->larguraBarra + 1);
    calcDireita(node->right, x + node->larguraBarra + 1, y + node->larguraBarra + 1);
}

struct node *removeFromTree(struct node *head, int target)
{
    //menor que a base
    if (target < head->data)
    {
        head->left = removeFromTree(head->left, target);
    }
    //maior que a base
    else if (target > head->data)
    {
        head->right = removeFromTree(head->right, target);
    }
    else
    {
        //apenas um filho ou sem filhos
        if (head->left == NULL)
        {
            struct node *temp = head->right;
            free(head);
            return temp;
        }
        else if (head->right == NULL)
        {
            struct node *temp = head->left;
            free(head);
            return temp;
        }
        //dois filhos
        struct node *temp = findLesserChild(head->right);
        head->data = temp->data;
        head->right = removeFromTree(head->right, temp->data);
    }
    return head;
}

struct node *findLesserChild(struct node *head)
{
    struct node *current = head;
    while (current && current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
