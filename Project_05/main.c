/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

//Bibliotecas utilizadas.
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Definição de um limite para a altura da árvore.
//Um valor muito alto pode comprometer o funcionamento adequado da função de imprimir a BST no formato de árvore.
#define MAX_HEIGHT 1000

//Variáveis globais utilizadas na impressão da árvore.
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
int gap = 3;
int print_next;

//Declaração da struct de um nódulo da árvore.
struct node
{
    int data;
    struct node *left;
    struct node *right;
};

//Declaração da struct necessária para imprimir a árvore.
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

//Declaração do protótipo das funções principais.
struct node *loadTreeFromFile(char *);
void showTree(struct node *);
void printInOrder(struct node *);
void printPreOrder(struct node *);
void printPostOrder(struct node *);
void balanceTree(struct node *);
bool isFull(struct node *);
int removeValue(struct node *, int);
int searchValue(struct node *, int);
int getHeight(struct node *);

//Declaração do protótipo das funções auxiliares.
struct asciinode *build_ascii_tree(struct node *);
struct asciinode *build_ascii_tree_recursive(struct node *);
struct node *searchMinChildValue(struct node *);
struct node *removeFromTree(struct node *, int);
void balanceTreeAfterBackbone(struct node *, int);
void insertOnTree(struct node **, int);
void compute_edge_lengths(struct asciinode *);
void compute_lprofile(struct asciinode *, int, int);
void compute_rprofile(struct asciinode *, int, int);
void print_level(struct asciinode *, int, int);
void rightRotate(struct node *);
void leftRotate(struct node *);
void freeTree(struct node *);
bool isBalanced(struct node *);
int min(int, int);
int max(int, int);

//Início da função principal.
int main()
{
    struct node *head = NULL;
    struct node *temp = NULL;
    int target;
    int escolha;
    int height = 0;
    bool fullCheck;
    char fileName[100];

    //Loop que define o menu do programa.
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
            if (head != NULL)
            {
                freeTree(head);
            }
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
            scanf("%d", &target);
            printf("\n");
            target = removeValue(head, target);
            if (target == 0)
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
            if (head != NULL)
            {
                freeTree(head);
            }
            printf("\nFechando o programa.\n\n");
            break;
        default:
            printf("\nEntrada inválida.\n");
            break;
        }
    } while (escolha != 11);
    return 0;
}

//Função carrega a árvore a partir do arquivo.
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

//Função imprime a BST no formato de árvore.
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
        lprofile[i] = (1 << 20);
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
    {
        xmin = min(xmin, lprofile[i]);
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

//Função remove valor da árvore.
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

//Função checa se a árvore é cheia.
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

//Função busca valor na árvore.
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

//Função calcula a altura da árvore.
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

//Função imprime a árvore InOrder.
void printInOrder(struct node *head)
{
    if (head != NULL)
    {
        printInOrder(head->left);
        printf("%d ", head->data);
        printInOrder(head->right);
    }
}

//Função imprime a árvore PreOrder.
void printPreOrder(struct node *head)
{
    if (head != NULL)
    {
        printf("%d ", head->data);
        printPreOrder(head->left);
        printPreOrder(head->right);
    }
}

//Função imprime a árvore PostOrder.
void printPostOrder(struct node *head)
{
    if (head != NULL)
    {
        printPostOrder(head->left);
        printPostOrder(head->right);
        printf("%d ", head->data);
    }
}

//Função de balanceamento da ávore.
//Foi utilizado o algoritmo de Day-Stout-Warren.
//Esse algoritmo foi escolhido por alguns motivos:
//1. Utiliza rotações, como é exigido na proposta de implementação. 
//2. A árvore gerada é a mais compacta possível: todos os níveis são completamente preenchidos, exceto talvez o último.
//3. O custo de operação é reduzido conforme o tamanho da árvore.
void balanceTree(struct node *head)
{
    struct node *current = head;
    int nodeCount = 0;

    //Gera o backbone.
    while (current != NULL)
    {
        while (current->left != NULL)
        {
            rightRotate(current);
        }
        current = current->right;
        nodeCount += 1;
    }

    int expected = ceil(log2(nodeCount)) - nodeCount;
    struct node *aux = head;

    //Balanceia o backbone gerado.
    for (int i = 0; i < expected; i++)
    {
        if (i == 0)
        {
            leftRotate(aux);
            aux = head;
        }
        else
        {
            leftRotate(aux->right);
            aux = aux->right;
        }
    }
    while (nodeCount > 1)
    {
        nodeCount /= 2;
        leftRotate(head);
        aux = head;

        for (int i = 0; i < nodeCount - 1; i++)
        {
            leftRotate(aux->right);
            aux = aux->right;
        }
    }
}

//Daqui em diante, somente funções auxiliares.

//Função de rotação à direita.
void rightRotate(struct node *node)
{
    if (node == NULL)
    {
        return;
    }
    else if (node->left == NULL)
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

//Função de rotação à esquerda.
void leftRotate(struct node *node)
{
    if (node == NULL)
    {
        return;
    }
    else if (node->right == NULL)
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

//Função verifica se a árvore é ou não balanceada.
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

//Função insere valor na árvore. Utilizada no carregamento de árvore a partir do arquivo.
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

//Funções auxiliares de mínimo e máximo.
int min(int x, int y)
{
    if (x < y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

//Função imprime os níveis da árvore utilizando o espaçamento entre nódulos.
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

//Função inicial para a impressão da árvore.
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

//Função recursiva que preenche a struct definida com os valores adequados.
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

//Calcula a "distância" entre os extremos da árvore para definir o espaçamento adequado.
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
                rprofile[i] = -(1 << 20);
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
                lprofile[i] = (1 << 20);
            }
            compute_lprofile(node->right, 0, 0);
            hmin = min(node->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++)
        {
            delta = max(delta, gap + 1 + rprofile[i] - lprofile[i]);
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
        h = max(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL)
    {
        h = max(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}

//Calcula o espaçamento à esquerda.
void compute_lprofile(struct asciinode *node, int x, int y)
{
    int i, isleft;
    if (node == NULL)
    {
        return;
    }
    isleft = (node->parent_dir == -1);
    lprofile[y] = min(lprofile[y], x - ((node->lablen - isleft) / 2));
    if (node->left != NULL)
    {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
        {
            lprofile[y + i] = min(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_lprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

//Calcula o espaçamento à direita.
void compute_rprofile(struct asciinode *node, int x, int y)
{
    int i, notleft;
    if (node == NULL)
    {
        return;
    }
    notleft = (node->parent_dir != -1);
    rprofile[y] = max(rprofile[y], x + ((node->lablen - notleft) / 2));
    if (node->right != NULL)
    {
        for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
        {
            rprofile[y + i] = max(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(node->left, x - node->edge_length - 1, y + node->edge_length + 1);
    compute_rprofile(node->right, x + node->edge_length + 1, y + node->edge_length + 1);
}

//Remove valor da árvore.
struct node *removeFromTree(struct node *head, int target)
{
    //Caso o valor seja menor que a raíz.
    if (target < head->data)
    {
        head->left = removeFromTree(head->left, target);
    }
    //Caso o valor seja maior que a raíz.
    else if (target > head->data)
    {
        head->right = removeFromTree(head->right, target);
    }
    //Caso o valor seja encontrado.
    else
    {
        //Quando o nódulo possui somente um filho ou não tem filhos.
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
        //Quando possui dois filhos.
        struct node *temp = searchMinChildValue(head->right);
        head->data = temp->data;
        head->right = removeFromTree(head->right, temp->data);
    }
    return head;
}

//Busca o filho com menor valor do nódulo fornecido.
struct node *searchMinChildValue(struct node *head)
{
    struct node *current = head;
    while (current && current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

//Função para liberar a memória alocada dinamicamente.
void freeTree(struct node *head)
{
    if (head == NULL)
    {
        return;
    }
    else
    {
        freeTree(head->left);
        freeTree(head->right);
        free(head);
    }
}
