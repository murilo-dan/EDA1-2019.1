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

struct node *loadTreeFromFile(char *);
void insertOnTree(struct node **, int);
void showTree();
bool isFull(struct node *);
int searchValue(struct node *, int);
int getHeight(struct node *);
//int removeValue(struct node *, int);
void printPreOrder(struct node *);
void printInOrder(struct node *);
void printPostOrder(struct node *);
void balanceTree();

int main()
{
    struct node *head = NULL;
    int target;
    int escolha;
    int height = 0;
    bool fullCheck;
    char fileName[10];
    do
    {
        printf("\nFunções Disponíveis:\n");
        printf("1. Carregar árvore de arquivo.\n");
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
        printf("Entrada: ");
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            printf("\nInsira o nome do arquivo: ");
            fscanf(stdin, "%s", fileName);
            head = loadTreeFromFile(fileName);
            printf("%d\n",head->data);
            break;
        case 2:
            showTree();
            break;
        case 3:
            fullCheck = isFull(head);
            if(fullCheck == true)
            {
              printf("A árvore é cheia.\n\n");
            }
            else if(fullCheck == false)
            {
              printf("A árvore não é cheia\n\n");
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
            height = getHeight(head);
            printf("%d\n",height);
            break;
        case 6:
            printf("\nDigite o valor a ser removido: ");
            scanf("%d\n",&target);
            printf("\n");
            //target = removeValue(head, target);
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
            printInOrder(head);
            break;
        case 8:
            printPreOrder(head);
            break;
        case 9:
            printPostOrder(head);
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
    else
    {
      while (!feof(fp))
      {
          fscanf(fp, "%d", &aux);
          insertOnTree(&head, aux);
      }
      fclose(fp);
    }
    fclose(fp);
    return head;
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

bool isFull(struct node *head)
{
    if(head == NULL)
    {
      printf("A árvore não existe\n");
    }
    else
    {
      if(head->left == NULL && head->right == NULL)
      {
        return true;
      }
      if((head->left)&&(head->right))
      {
        return (isFull(head->left)&&isFull(head->right));
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
    if(head==NULL)
    {
      return 0;
    }
    int left = getHeight(head->left);
    int right = getHeight(head->right);
    if(left>right)
    {
      return left + 1;
    }
    else
    {
      return right + 1;
    }
}

/*int removeValue(struct node *head, int target)
{
  int height = 1;
  if (head == NULL)
  {
      printf("Valor não encontrado, árvore não existe.\n");
      return 0;
  }
  else if(head->left != NULL && head->right == NULL)
  {
    if(head)
  }
}*/

void balanceTree()
{
}
