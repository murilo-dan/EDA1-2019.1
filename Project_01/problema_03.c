/*
Estrutura de Dados 1 - Turma A/2019.1
Murilo Loiola Dantas - 17/0163571
Gabriel Alves Hussein - 17/0103200
*/

#include <stdio.h>
#include <stdlib.h>

void torre (int, char, char, char);

int main(){
    int discos;
    printf ("Insira a quantidade de discos:\n");
    scanf ("%d", &discos);
    torre (discos, 'A', 'C', 'B');
    return 0;
}

void torre (int discos, char torre_a, char torre_c, char torre_b){
    if(discos == 1){
      printf ("Mover disco da torre %c para torre %c\n", torre_a, torre_c);
      return;
    }
    torre(discos-1, torre_a, torre_b, torre_c);
    printf ("Mover disco da torre %c para torre %c\n", torre_a, torre_c);
    torre(discos-1, torre_b, torre_c, torre_a);
}
