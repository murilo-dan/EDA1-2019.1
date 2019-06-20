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
#define alturaMaxima 1000

//Variáveis globais utilizadas na impressão da árvore.
int esquerda[alturaMaxima];
int direita[alturaMaxima];
int distancia = 3;
int printNext;

//Declaração da struct de um nódulo da árvore.
struct node
{
    int data;
    struct node *left;
    struct node *right;
};

//Declaração da struct necessária para imprimir a árvore.
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

//Declaração do protótipo das funções principais.
struct node *loadTreeFromFile(char *);
void showTree(struct node *);
void printInOrder(struct node *);
void printPreOrder(struct node *);
void printPostOrder(struct node *);
void balanceTree(struct node *);
void removeValue(struct node *, int);
bool isFull(struct node *);
int searchValue(struct node *, int);
int getHeight(struct node *);

//Declaração do protótipo das funções auxiliares.
struct desenhoArvore *desenharArvore(struct node *);
struct desenhoArvore *desenharArvoreRecursivamente(struct node *);
struct node *findLesserChild(struct node *);
struct node *removeFromTree(struct node *, int);
void insertOnTree(struct node **, int);
void calcTamanhoExtremidades(struct desenhoArvore *);
void calcEsquerda(struct desenhoArvore *, int, int);
void calcDireita(struct desenhoArvore *, int, int);
void printLevel(struct desenhoArvore *, int, int);
void rotacaoDireita(struct node *);
void rotacaoEsquerda(struct node *);
void freeTree(struct node *);
bool isBalanced(struct node *);
bool basicSearch(struct node *, int);
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
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            printf("\n");
            showTree(head);
            break;
        case 3:
            fullCheck = isFull(head);
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
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
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
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
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            printf("\n");
            height = getHeight(head);
            printf("%d\n", height);
            break;
        case 6:
            printf("\nDigite o valor a ser removido: ");
            scanf("%d", &target);
            printf("\n");
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            removeValue(head, target);
            break;
        case 7:
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            printf("\n");
            printInOrder(head);
            printf("\n");
            break;
        case 8:
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
            printf("\n");
            printPreOrder(head);
            printf("\n");
            break;
        case 9:
            if (head == NULL)
            {
                printf("\nÁrvore não existe.\n");
                break;
            }
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
        esquerda[i] = (1 << 20);
    }
    calcEsquerda(temp, 0, 0);
    xmin = 0;
    for (i = 0; i < temp->height && i < alturaMaxima; i++)
    {
        xmin = min(xmin, esquerda[i]);
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

//Função remove valor da árvore.
void removeValue(struct node *head, int target)
{
    //Primeiro verifica se o valor está presente na árvore.
    bool found = basicSearch(head, target);
    //Caso esteja, remove o valor.
    if (found)
    {
        head = removeFromTree(head, target);
        printf("Valor removido com sucesso.\n");
        return;
    }
    //Caso contrário, notifica que não foi encontrado.
    else
    {
        printf("Valor não ecnontrado.\n");
        return;
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
            rotacaoDireita(current);
        }
        current = current->right;
        nodeCount++;
    }

    int expected = ceil(log2(nodeCount)) - nodeCount;
    struct node *aux = head;

    //Balanceia o backbone gerado.
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

//Função de rotação à direita.
void rotacaoDireita(struct node *node)
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
void rotacaoEsquerda(struct node *node)
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

//Função inicial para a impressão da árvore.
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

//Função recursiva que preenche a struct definida com os valores adequados.
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

//Calcula a "distância" entre os extremos da árvore para definir o espaçamento adequado.
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
                direita[i] = -(1 << 20);
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
                esquerda[i] = (1 << 20);
            }
            calcEsquerda(node->right, 0, 0);
            hmin = min(node->right->height, hmin);
        }
        else
        {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++)
        {
            delta = max(delta, distancia + 1 + direita[i] - esquerda[i]);
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
        h = max(node->left->height + node->larguraBarra + 1, h);
    }
    if (node->right != NULL)
    {
        h = max(node->right->height + node->larguraBarra + 1, h);
    }
    node->height = h;
}

//Calcula o espaçamento à esquerda.
void calcEsquerda(struct desenhoArvore *node, int x, int y)
{
    int i, isleft;
    if (node == NULL)
    {
        return;
    }
    isleft = (node->direcaoPai == -1);
    esquerda[y] = min(esquerda[y], x - ((node->labelSize - isleft) / 2));
    if (node->left != NULL)
    {
        for (i = 1; i <= node->larguraBarra && y + i < alturaMaxima; i++)
        {
            esquerda[y + i] = min(esquerda[y + i], x - i);
        }
    }
    calcEsquerda(node->left, x - node->larguraBarra - 1, y + node->larguraBarra + 1);
    calcEsquerda(node->right, x + node->larguraBarra + 1, y + node->larguraBarra + 1);
}

//Calcula o espaçamento à direita.
void calcDireita(struct desenhoArvore *node, int x, int y)
{
    int i, notleft;
    if (node == NULL)
    {
        return;
    }
    notleft = (node->direcaoPai != -1);
    direita[y] = max(direita[y], x + ((node->labelSize - notleft) / 2));
    if (node->right != NULL)
    {
        for (i = 1; i <= node->larguraBarra && y + i < alturaMaxima; i++)
        {
            direita[y + i] = max(direita[y + i], x + i);
        }
    }
    calcDireita(node->left, x - node->larguraBarra - 1, y + node->larguraBarra + 1);
    calcDireita(node->right, x + node->larguraBarra + 1, y + node->larguraBarra + 1);
}

//Busca o filho com menor valor do nódulo fornecido.
struct node *findLesserChild(struct node *head)
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

//Remove valor da árvore.
struct node *removeFromTree(struct node *head, int target)
{
    if (head == NULL)
    {
        return head;
    }
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
            return (temp);
        }
        else if (head->right == NULL)
        {
            struct node *temp = head->left;
            free(head);
            return (temp);
        }
        //Quando possui dois filhos.
        struct node *temp = findLesserChild(head->right);
        head->data = temp->data;
        head->right = removeFromTree(head->right, temp->data);
    }
    return head;
}

//Função básica de busca para determinar somente so o valor está presente na árvore.
bool basicSearch(struct node *head, int target)
{
    if (target == head->data)
    {
        return true;
    }
    else if (target < head->data)
    {
        if (head->left == NULL)
        {
            return false;
        }
        else
        {
            basicSearch(head->left, target);
        }
    }
    else
    {
        if (head->right == NULL)
        {
            return false;
        }
        else
        {
            basicSearch(head->right, target);
        }
    }
}