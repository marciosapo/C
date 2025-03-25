#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#define MAX 3

bool verificarVencedor(char tabuleiro[MAX][MAX], int turno) {
    char jogador;
    if (turno==1)jogador='O';
    else jogador='X';
    for (int i = 0; i < MAX; i++) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador && tabuleiro[i][2] == jogador) {
            return true;
        }
    }

    for (int i = 0; i < MAX; i++) {
        if (tabuleiro[0][i] == jogador && tabuleiro[1][i] == jogador && tabuleiro[2][i] == jogador) {
            return true;
        }
    }

    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][2] == jogador) {
        return true;
    }
    if (tabuleiro[0][2] == jogador && tabuleiro[1][1] == jogador && tabuleiro[2][0] == jogador) {
        return true;
    }
    return false;
}

bool tabuleiroCheio(char tabuleiro[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (tabuleiro[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    setlocale(LC_ALL,"Portuguese");
    char tabuleiro[3][3];
    int turno = 0, op, wins = 0, loses = 0, draws = 0, inGame = 0, row, col;

    do {
       printf("---------------\n");
       printf("Jogo do Galo!\n");
       printf("---------------\n");
       printf("\n");
       printf("Main Menu\n");
       printf("\n");
       printf("(1)Jogar\n");
       printf("(2)Resultados\n");
       printf("(0)Sair\n");
       scanf("%d", &op);
       switch(op){
       case 1:
            for (int i = 0; i < MAX; i++)
                for (int b = 0; b < MAX; b++)tabuleiro[i][b] = ' ';
            int inGame = 1;
            int turno = 1;
            do {
                printf("\n");
                for (int i = 0; i < MAX; i++) {
                    for (int b = 0; b < MAX; b++) {
                        printf(" %c ", tabuleiro[i][b] == ' ' ? ' ' : tabuleiro[i][b]);
                        if (b < MAX - 1) printf("|");
                    }
                    printf("\n");
                    if (i < MAX - 1) {
                        printf("---|---|---\n");
                    }
                }
                printf("\n");
                if(turno == 1){
                    printf("É o turno do Player 1!\n\n");
                    printf("Jogador O, insere a linha (0-2) e a coluna (0-2):");
                    scanf("%d%d", &row, &col);
                    if (row >= 0 && row < MAX && col >= 0 && col < MAX && tabuleiro[row][col] == ' ') {
                        tabuleiro[row][col] = 'O';
                        if(verificarVencedor(tabuleiro, turno)){
                            printf("Player 1 Ganhou!\n");
                            wins++;
                            inGame = 0;
                        }
                        else if (tabuleiroCheio(tabuleiro)) {
                            printf("Empate!\n");
                            draws++;
                            inGame = 0;
                        }
                        turno = 2;
                    }else {
                        printf("Posição já preenchida ou ínvalida!");
                    }
                }
                else {
                    printf("É o turno do CPU!\n\n");
                    bool played = false;
                    do {
                        int tempRow = rand()%3;
                        int tempCol = rand()%3;
                        if (tempRow >= 0 && tempRow < MAX && tempCol >= 0 && tempCol < MAX && tabuleiro[tempRow][tempCol] == ' '){
                            tabuleiro[tempRow][tempCol] = 'X';
                            if(verificarVencedor(tabuleiro, turno)){
                                printf("CPU Ganhou!\n");
                                loses++;
                                inGame = 0;
                            }
                            else if (tabuleiroCheio(tabuleiro)) {
                                printf("Empate!\n");
                                draws++;
                                inGame = 0;
                            }
                            turno = 1;
                            played = true;
                        }
                    }while(played == false);
                }

            }while (inGame == 1);
            break;
       case 2:
           printf("\n----------------\n");
           printf("Vitorias: %d\n", wins);
           printf("Derrotas: %d\n", loses);
           printf("Empates: %d\n", draws);
           printf("\n----------------\n");
           break;
       default:
            printf("Opção ínvalida!\n");
            break;
       }
    } while(op!=0);


    return 0;
}


