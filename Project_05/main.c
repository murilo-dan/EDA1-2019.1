#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_HEIGHT 1000
#define INFINITY (1 << 20)

int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
int gap = 3;
int print_next;

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct asciinode
{
    int edge_length;
    int height;
    int lablen;
    int parent_dir;
    char label[11];
    struct asciinode *left;
    struct asciinode *right;
};

//Funções principais.
struct node *loadTreeFromFile(char *);
void showTree(struct node *);
void removeValue();
void printInOrder(struct node *);
void printPreOrder(struct node *);
void printPostOrder(struct node *);
void balanceTree();
bool isFull(struct node *);
int searchValue(struct node *, int);
int getHeight(struct node *);

//Funções auxiliares.
struct asciinode *build_ascii_tree(struct node *);
struct asciinode *build_ascii_tree_recursive(struct node *);
void insertOnTree(struct node **, int);
void compute_edge_lengths(struct asciinode *);
void compute_lprofile(struct asciinode *, int, int);
void compute_rprofile(struct asciinode *, int, int);
void print_level(struct asciinode *, int, int);
bool isBalanced(struct node *);
int MIN(int, int);
int MAX(int, int);

int main()
{
    struct node *head = NULL;
    int target;
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
            scanf("%d\n", &target);
            printf("\n");
            //target = removeValue(head, target);
            if (target == 0)
            {
                break;
            }
            else
            {
                printf("\nValor removido com sucesso.\n");
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
                    balanceTree();
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
    struct asciinode *proot;
    int xmin, i;
    if (head == NULL)
    {
        return;
    }
    proot = build_ascii_tree(head);
    compute_edge_lengths(proot);
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
    {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
    {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->height; i++)
    {
        print_next = 0;
        print_level(proot, -xmin, i);
        printf("\n");
    }
    if (proot->height >= MAX_HEIGHT)
    {
        printf("erro\n");
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

// int removeValue(struct node *head, int target)
// {
//   int height = 1;
//   if (head == NULL)
//   {
//       printf("Valor não encontrado, árvore não existe.\n");
//       return 0;
//   }
//   else if(head->left != NULL && head->right == NULL)
//   {
//     if(head)
//   }
// }

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

void balanceTree()
{
}

//Daqui em diante, somente funções auxiliares.

bool leftRotate(struct node *node)
{
    if (node->right == NULL)
    {
        struct node *right = node->right;
        node->right = right->right;
        right->right = right->left;
        right->left = node->left;
        node->left = right;

        int temp = node->data;
        node->data = right->data;
        right->data = temp;
    }
    return node;
}

struct node *rightRotate(struct node *node)
{
    if (node->left == NULL)
    {
        struct node *left = node->left;
        node->left = left->left;
        left->left = left->right;
        left->right = node->right;
        node->right = left;

        int temp = node->data;
        node->data = left->data;
        left->data = temp;
    }
    return node;
}

struct node *createBackBone(struct node *node)
{
    while (node->left != NULL)
    {
        node = rightRotate(node);
    }
    if (node->right != NULL)
    {
        node->right = createBackBone(node->right);
    }
    return node;
}

int getNodesCount(struct node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    int i;
    for (i = 1; node->right != NULL; i++)
    {
        node = node->right;
    }
    return i;
}

struct node *balanceBackBone(struct node *node, int nodeCount)
{
    int times = log2(nodeCount);
    struct node *newNode = node;
    for (int i = 0; i < times; i++)
    {
        newNode = leftRotate(newNode);
        node = newNode->right;
        for (int j = 0; j < nodeCount / 2 - 1; j++)
        {
            node = leftRotate(node);
            node = node->right;
        }
        nodeCount >>= 1;
    }
    return newNode;
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

void print_level(struct asciinode *node, int x, int level)
{
    int i, isleft;
    if (node == NULL)
        return;
    isleft = (node->parent_dir == -1);
    if (level == 0)
    {
        for (i = 0; i < (x - print_next - ((node->lablen - isleft) / 2)); i++)
        {
            printf(" ");
        }
        print_next += i;
        printf("%s", node->label);
        print_next += node->lablen;
    }
    else if (node->edge_length >= level)
    {
        if (node->left != NULL)
        {
            for (i = 0; i < (x - print_next - (level)); i++)
            {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (node->right != NULL)
        {
            for (i = 0; i < (x - print_next + (level)); i++)
            {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    }
    else
    {
        print_level(node->left, x - node->edge_length - 1, level - node->edge_length - 1);
        print_level(node->right, x + node->edge_length + 1, level - node->edge_length - 1);
    }
}

struct asciinode *build_ascii_tree(struct node *head)
{
    struct asciinode *node;
    if (head == NULL)
    {
        return NULL;
    }
    node = build_ascii_tree_recursive(head);
    node->parent_dir = 0;
    return node;
}

struct asciinode *build_ascii_tree_recursive(struct node *head)
{
    struct asciinode *node;
    if (head == NULL)
    {
        return NULL;
    }
    node = malloc(sizeof(struct asciinode));
    node->left = build_ascii_tree_recursive(head->left);
    node->right = build_ascii_tree_recursive(head->right);
    if (node->left != NULL)
    {
        node->left->parent_dir = -1;
    }
    if (node->right != NULL)
    {
        node->right->parent_dir = 1;
    }

    sprintf(node->label, "%d", head->data);
    node->lablen = strlen(node->label);

    return node;
}

void compute_edge_lengths(struct asciinode *node)
{
    int h, hmin, i, delta;
    if (node == NULL)
        return;
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);

    if (node->right == NULL && node->left == NULL)
    {
        node->edge_length = 0;
    }
    else
    {
        if (node->left != NULL)
        {
            for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
            {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(node->left, 0, 0);
            hmin = node->left->height;
        }
        else
        {
            hmin = 0;
        }
        if (node->right != NULL)
        {
            for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
            {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(node->right, 0, 0);
            hmin = MIN(node->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++)
        {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

        if (((node->left != NULL && node->left->height == 1) ||
             (node->right != NULL && node->right->height == 1)) &&
            delta > 4)
        {
            delta--;
        }

        node->edge_length = ((delta + 1) / 2) - 1;
    }

    h = 1;
    if (node->left != NULL)
    {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL)
    {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

void compute_lprofile(struct asciinode *node, int x, int y)
{
    int i, isleft;
    if (node == NULL)
    {
        return;
    }
    isleft = (node->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x - ((node->lablen - isleft) / 2));
    if (node->left != NULL)
    {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
        {
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

void compute_rprofile(struct asciinode *node, int x, int y)
{
    int i, notleft;
    if (node == NULL)
    {
        return;
    }
    notleft = (node->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((node->lablen - notleft) / 2));
    if (node->right != NULL)
    {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
        {
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}