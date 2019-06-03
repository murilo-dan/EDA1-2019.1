/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

//Bibliotecas utilizadas.
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//Definição da struct Queue que guarda as posições inicial e final da queue de pedidos.
struct Queue
{
    struct Plane *front;
    struct Plane *rear;
};

//Definição da struct Plane que guarda as informações de cada pedido da lista.
struct Plane
{
    char id[6];
    int fuel;
    char mode;
    struct Plane *next;
};

//Declaração da constante de unidade de tempo (5 minutos).
const int timeUnit = 5;

//Declaração de variáveis globais relacionadas aos tempos utilizados para gerenciar os pedidos.
//A variável globalTime define o horário de início das operações e é tratada como minutos.
int globalTime = 480;
int lane1, lane2, lane3, decreaseFuel;

//Declaração do protótipo das funções utilizadas.
void decreaseOvertime(struct Plane *);
void display(struct Plane *);
void insertRequests(struct Queue *, int, int, int, int *, char **);
void manageRequests(struct Plane *, int *, int *, int *, struct Queue *);
void removeRequest(struct Plane *, struct Plane *, struct Queue *, int *);
int emergencySwitch(struct Queue *, int *, int);
int *shuffle(int *);

//Início da função principal.
int main(int argc, char **argv)
{
    //Define o limite máximo de 23 horas e 59 minutos para o horários inicial.
    if (globalTime >= 1440)
    {
        globalTime = 0;
    }
    //Todos os horários auxiliares são inicializados com o horário inicial.
    lane1 = lane2 = lane3 = decreaseFuel = globalTime;

    //Função utilizada para que o resultado seja aleatório sempre que o programa for rodado.
    srand(time(NULL));

    //Alocação e inicialização da fila de pedidos, inicialmente vazia.
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->rear = queue->front = NULL;

    //Inicialização do array contendo os IDs disponíveis.
    char *idVoos[64] = {"VG3001", "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008",
                        "AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012", "TT4544",
                        "TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002", "AZ1007", "GL7604",
                        "AZ1006", "TG1503", "AZ1003", "JJ4403", "AZ1001", "LN7003", "AZ1004", "TG1504",
                        "AZ1005", "TG1502", "GL7601", "TT4500", "RL7801", "JJ4410", "GL7607", "AL0029",
                        "VV3390", "VV3392", "GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920",
                        "AZ1065", "LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
                        "TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609", "KL5610", "KL5611"};
    //Inicialização das variáveis referentes ao número de voos.
    int NAprox = rand() % 23 + 10;
    int NDec = rand() % 23 + 10;
    int NVoos = NAprox + NDec;

    //Array utilizado para garantir que um dado ID não seja utilizado duas vezes (exceto aqueles
    //que já são repetidos na lista).
    int random[64] = {0};

    //Variável utilizada para definir se há um alerta de emergência em ação.
    int emergency = 0;

    //Randomização do array "random" que será utilizado como índice do array "idVoos".
    shuffle(random);

    //Função gera as informações dos pedidos aleatoriamente e insere na fila.
    insertRequests(queue, NVoos, NAprox, NDec, random, idVoos);

    //Imprime o log inicial do aeroporto na tela, com a fila de pedidos conforme foi gerada.
    printf("Aeroporto Internacional de Brasília\nHora inicial: %02d:%02d\nQueue de pedidos:\nCódgigo de voo - Tipo - Prioridade\n", globalTime / 60, globalTime - (globalTime / 60) * 60);
    display(queue->front);
    printf("\nNVoos: %d\nNAproximações: %d\nNDecolagens: %d\n\nListagem de eventos:\n\n", NVoos, NAprox, NDec);

    for (; NVoos > 0;)
    {
        //Para cada iteração do laço, essa função verifica se algum avião está com 0 de combustível.
        //Caso encontre um ou mais aviões nesse estado, a função desloca todos os aviões em questão
        //para o começo da fila e retorna 1 (true) para a variável "emergency".
        emergency = emergencySwitch(queue, &NVoos, emergency);

        //Função que realiza o gerenciamente das pistas. A cada iteração do laço, essa função
        //retira um avião da fila, de acordo com prioridade ou disponibilidade das pistas.
        manageRequests(queue->front, &emergency, &NVoos, &NDec, queue);

        //Função utilizada para calcular o decaimento de combustível das aeronaves com o passar
        //do tempo. Diminui o combustível de todas as aeronaves (A) em 1 a cada 10 unidades de tempo (50 minutos).
        decreaseOvertime(queue->front);

        //Condicional verifica se os pedidos de decolagem da fila acabaram e informa a mudança de
        //disponibilidade da pista 3.
        if (NVoos > 0 && NDec == 0)
        {
            printf("Pedidos de decolagem finalizados, pista 3 liberada para pousos.\n\n");
            NDec--;
        }
    }

    printf("Todos os pedidos atendidos.\n\n");

    //Libera a memória alocada dinamicamente para a fila.
    free(queue);

    return 0;
}

//Função calcula o decaimento de combustível.
void decreaseOvertime(struct Plane *head)
{
    //Condicional verifica se o globalTime passou de 23:59 para atualizar "decreaseFuel" de acordo.
    //Garante que a função opere normalmente mesmo em casos de passagem do dia.
    if (decreaseFuel > globalTime)
    {
        decreaseFuel = decreaseFuel - 1440;
    }

    //Caso a diferença de tempo seja maior ou igual a 50 minutos, atualiza "decreaseFuel" para bater com
    // "globalTime" e diminui o nível de combustível em 1 para todos os aviões.
    if ((globalTime - decreaseFuel) >= 50)
    {
        decreaseFuel = globalTime;

        while (head != NULL)
        {
            if (head->fuel != -1)
            {
                head->fuel--;
            }
            head = head->next;
        }
    }
}

//Função imprime todos os pedidos que ainda estiverem na lista, apresentando ID, tipo e nível de combustível.
void display(struct Plane *head)
{
    while (head != NULL)
    {
        printf("     %s    -   %c  -    ", head->id, head->mode);

        if (head->fuel == -1)
        {
            printf("N/A\n");
        }
        else
        {
            printf("%d\n", head->fuel);
        }

        head = head->next;
    }
}

//Função gera as informações dos pedidos aleatoriamente e insere na fila.
void insertRequests(struct Queue *queue, int NVoos, int NAprox, int NDec, int *random, char **idVoos)
{
    for (int i = 0; i < NVoos; i++)
    {
        //Alocação da memória que será utilizada para armazenar as informações do pedido.
        struct Plane *newPlane = (struct Plane *)malloc(sizeof(struct Plane));

        //Copia um ID aleatório para a informação do pedido. A utilização do array "random" garante
        //que não ocorra repetição (exceto para os que já são repetidos na lista de entrada).
        strcpy(newPlane->id, idVoos[random[i]]);

        //Define a operação (aproximação ou decolagem) da aeronave aleatoriamente.
        //Caso a função "rand" retorne 0, a operação é definida como aproximação (A).
        //Caso a função "rand" retorne 1 (ou qualquer outro valor), a operação é definida como decolagem (D).
        if (rand() % 2 == 0 && NAprox != 0)
        {
            newPlane->mode = 'A';
            NAprox--;
        }
        else if (NDec != 0)
        {
            newPlane->mode = 'D';
            NDec--;
        }
        else
        {
            newPlane->mode = 'A';
        }

        //Define um nível de combustível aleatório (variando de 0 a 12) caso a operação seja de aproximação.
        //Caso a aproximação seja de decolagem, define nível de combustível -1.
        if (newPlane->mode == 'A')
        {
            newPlane->fuel = rand() % 13;
        }
        else
        {
            newPlane->fuel = -1;
        }

        //Caso a fila esteja vazia, a fila começa e termina no elemento inserido.
        if (queue->rear == NULL)
        {
            queue->rear = queue->front = newPlane;
            continue;
        }

        //Armazena o elemento inserido na próxima posição da fila.
        queue->rear->next = newPlane;
        queue->rear = newPlane;
    }
}

//Função gerencia as pistas do aeroporto, além de atualizar as variáveis de tempo.
void manageRequests(struct Plane *head, int *emergency, int *NVoos, int *NDec, struct Queue *queue)
{
    //Condicionais garantem que o horário apresentado na tela não exceda os limite do dia (00:00 - 23:59).
    if (lane1 >= 1440)
    {
        lane1 = lane1 - 1440;
    }
    if (lane2 >= 1440)
    {
        lane2 = lane2 - 1440;
    }
    if (lane2 >= 1440)
    {
        lane2 = lane2 - 1440;
    }

    struct Plane *prev = NULL;

    //Itera até chegar ao fim da fila.
    while (head != NULL)
    {
        //Condicional para os aviões de aproximação.
        //As condicionais de "dentro" permitem que as pistas sejam utilizadas da maneira mais eficiente
        //possível, sem que ocorram conflitos (duas operações na mesma pista ao mesmo tempo).
        if (head->mode == 'A')
        {
            if (lane1 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 1\n\n", head->id, lane1 / 60, lane1 - (lane1 / 60) * 60);
                lane1 += 4 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                break;
            }
            else if (lane2 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 2\n\n", head->id, lane2 / 60, lane2 - (lane2 / 60) * 60);
                lane2 += 4 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                break;
            }
            //A pista 3 só será usada para pouso em caso de emergência ou caso os pedidos de decolagem acabem.
            else if (((*emergency) && lane3 <= globalTime) || (*NDec) <= 0)
            {
                printf("Código do voo: %s\nStatus: aeronave pousou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 3\n\n", head->id, lane3 / 60, lane3 - (lane3 / 60) * 60);
                lane3 += 4 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                (*emergency) = 0;
                break;
            }
            //Caso todas as pistas estejam indiponíveis e o combustível acabe, a aeronave cai.
            else if (head->fuel == -1)
            {
                printf("ALERTA CRÍTICO, AERONAVE %s IRÁ CAIR\n\n", head->id);
                removeRequest(head, prev, queue, NVoos);
                break;
            }
        }
        //Condicional para os aviões de decolagem. Pista 3 tem prioridade.
        else if (head->mode == 'D')
        {
            if (lane3 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 3\n\n", head->id, lane3 / 60, lane3 - (lane3 / 60) * 60);
                lane3 += 2 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                (*NDec)--;
                break;
            }
            else if (lane2 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 2\n\n", head->id, lane2 / 60, lane2 - (lane2 / 60) * 60);
                lane2 += 2 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                (*NDec)--;
                break;
            }
            else if (lane1 <= globalTime)
            {
                printf("Código do voo: %s\nStatus: aeronave decolou\nHorário do início do procedimento:%02d:%02d\nNúmero da pista: 1\n\n", head->id, lane1 / 60, lane1 - (lane1 / 60) * 60);
                lane1 += 2 * timeUnit;
                removeRequest(head, prev, queue, NVoos);
                (*NDec)--;
                break;
            }
        }

        //Armazena o atual em "prev" e passa para o próximo.
        prev = head;
        head = head->next;
    }

    //Condicionais atualizam "globalTime" conforme os tempos das pistas para garantir melhor eficiência.
    if (lane1 <= globalTime)
    {
        globalTime = lane1;
    }
    else if (lane2 <= globalTime)
    {
        globalTime = lane2;
    }
    else if (lane3 <= globalTime)
    {
        globalTime = lane3;
    }
    else
    {
        int min = 1500;
        if (min >= lane1)
        {
            min = lane1;
        }
        if (min >= lane2)
        {
            min = lane2;
        }
        if (min >= lane3)
        {
            min = lane3;
        }
        globalTime = min;
    }
}

//Função retira um pedido da fila, seja na frente, no meio ou no final.
void removeRequest(struct Plane *del, struct Plane *prev, struct Queue *queue, int *NVoos)
{
    (*NVoos)--;
    if (prev == NULL)
    {
        queue->front = del->next;
        free(del);
        return;
    }
    else if (del->next == NULL)
    {
        prev->next = NULL;
        free(del);
    }
    else
    {
        prev->next = del->next;
        free(del);
    }
}

//Função organiza a fila de acordo com a prioridade e fornece alertas de emergência.
int emergencySwitch(struct Queue *queue, int *NVoos, int emergency)
{
    struct Plane *current, *prev, *temp;

    //Inicia leitura no começo da fila e contagem de aeronaves com 0 de combustível.
    current = queue->front;
    int fuelCheck = 0;

    //Verifica a primeira aeronave da fila.
    if (queue->front->fuel == 0)
    {
        prev = current;
        current = current->next;
        fuelCheck++;
    }

    //Verifica se há somente uma aeronave na fila.
    if (current == NULL)
    {
        (*NVoos)--;
        return emergency;
    }

    //Verifica as aeronaves no meio da fila.
    while (current->next != NULL)
    {
        if (current->fuel == 0)
        {
            prev->next = current->next;
            temp = queue->front;
            current->next = temp;
            queue->front = current;
            current = prev->next;
            fuelCheck++;
            continue;
        }
        prev = current;
        current = current->next;
    }

    //Verifica a última aeronave da fila.
    if (queue->rear->fuel == 0)
    {
        prev->next = NULL;
        temp = queue->front;
        current->next = temp;
        queue->front = current;
        queue->rear = prev;
        fuelCheck++;
    }

    //Gera alerta de emergência caso haja 3 ou mais aeronaves com 0 de combustível. 
    if (fuelCheck >= 3)
    {
        printf("ALERTA GERAL DE DESVIO DE AERONAVE\n\n");
        return 1;
    }
    return emergency;
}

//Função preenche o array recebido com valores crescentes de 0 até o tamanho do array
//e realiza o embaralhamento dos valores.
int *shuffle(int *random)
{
    for (int i = 0; i < 64; i++)
    {
        random[i] = i;
    }
    for (int i = 0; i < 64; i++)
    {
        int temp = random[i];
        int randomIndex = rand() % 64;
        random[i] = random[randomIndex];
        random[randomIndex] = temp;
    }
    return random;
}