#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <conio.h>  // Para _kbhit() e _getch()

#define WIDTH 20
#define HEIGHT 20
#define TOTAL_PHASES 2  // Quantidade de fases no jogo

// Função para desenhar o campo de jogo
void drawMap(char map[HEIGHT][WIDTH]) {
    system("cls");  // Limpa a tela

    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (map[i][j] == 'P') {
                printf("\033[31mP\033[0m ");  // Jogador em vermelho
            } else if (map[i][j] == '|') {
                printf("\033[32m|\033[0m ");  // Parede em verde
            } else if (map[i][j] == 'K') {
                printf("\033[36mK\033[0m ");  // Chave em azul claro
            } else if (map[i][j] == 'M') {
                printf("\033[33mM\033[0m ");  // Meta em amarelo
            } else if (map[i][j] == 'X') {
                printf("\033[34mX\033[0m ");  // Trajeto já percorrido em azul escuro
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo azul bebê
            }
        }
        printf("\n");
    }
}

// Função para configurar o mapa da fase
void setupMap(char map[HEIGHT][WIDTH], int phase) {
    int i, j;

    // Limpa o mapa
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map[i][j] = '.';  // Espaço vazio
        }
    }

    // Adiciona paredes nas bordas
    for (i = 0; i < HEIGHT; i++) {
        map[i][0] = '|';
        map[i][WIDTH - 1] = '|';
    }
    for (j = 0; j < WIDTH; j++) {
        map[0][j] = '|';
        map[HEIGHT - 1][j] = '|';
    }

    // Configuração específica por fase
    if (phase == 1) {
        // Fase 1
        map[5][5] = '|';
        map[6][5] = '|';
        map[7][5] = '|';
        map[8][5] = '|';
        map[3][3] = 'K';  // Chave
        map[HEIGHT - 2][WIDTH - 2] = 'M';  // Meta
    } else if (phase == 2) {
        // Fase 2 (mais desafiadora)
        map[5][5] = '|';
        map[5][6] = '|';
        map[5][7] = '|';
        map[6][7] = '|';
        map[7][7] = '|';
        map[8][3] = '|';
        map[4][4] = 'K';  // Chave
        map[1][WIDTH - 2] = 'M';  // Meta no topo direito
    }
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");

    char map[HEIGHT][WIDTH];
    int playerX = 1, playerY = 1;  // Posição inicial do jogador
    char input;
    int hasKey = 0;  // Jogador começa sem a chave
    int currentPhase = 1;  // Começa na fase 1
    int gameOver = 0;

    // Configura o mapa inicial
    setupMap(map, currentPhase);
    map[playerY][playerX] = 'P';  // Posiciona o jogador no mapa

    // Desenha o campo inicial
    drawMap(map);

    while (!gameOver) {
        if (_kbhit()) {
            input = _getch();  // Captura a tecla pressionada
            int newX = playerX;
            int newY = playerY;

            // Movimenta o jogador com base na tecla pressionada
            if (input == 'w') newY--;  // Para cima
            if (input == 's') newY++;  // Para baixo
            if (input == 'a') newX--;  // Para esquerda
            if (input == 'd') newX++;  // Para direita

            // Verifica se a nova posição é válida
            if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                if (map[newY][newX] != '|') {  // Não pode atravessar paredes
                    if (map[newY][newX] == 'K') {
                        hasKey = 1;  // Pega a chave
                        printf("Você pegou a chave!\n");
                        usleep(500000);  // Pausa de 500ms
                    }

                    // Verifica se é a meta antes de mover
                    if (map[newY][newX] == 'M') {
                        if (hasKey) {
                            printf("Parabéns! Você completou a fase %d!\n", currentPhase);
                            usleep(1000000);  // Pausa de 1 segundo

                            // Avança para a próxima fase
                            currentPhase++;
                            if (currentPhase > TOTAL_PHASES) {
                                printf("Você completou todas as fases! Parabéns!\n");
                                gameOver = 1;  // Encerra o jogo
                            } else {
                                // Configura o mapa da próxima fase
                                setupMap(map, currentPhase);
                                playerX = 1;
                                playerY = 1;  // Redefine a posição inicial do jogador
                                hasKey = 0;  // Reseta a chave
                                map[playerY][playerX] = 'P';  // Posiciona o jogador
                                drawMap(map);  // Desenha o novo mapa
                            }
                            continue;  // Evita redesenhar a fase anterior
                        } else {
                            printf("Você precisa pegar a chave primeiro!\n");
                            usleep(1000000);  // Pausa de 1 segundo
                            continue;  // Não move o jogador
                        }
                    }

                    // Atualiza posição do jogador
                    map[playerY][playerX] = 'X';  // Marca o caminho percorrido
                    playerX = newX;
                    playerY = newY;
                    map[playerY][playerX] = 'P';  // Atualiza posição do jogador
                }
            }

            // Redesenha o mapa
            drawMap(map);
        }

        usleep(100000);  // Aguarda 100ms
    }

    return 0;
}
