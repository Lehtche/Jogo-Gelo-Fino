#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <conio.h>

#define LARGURA 20
#define ALTURA 20
#define TOTAL_DE_FASES 5

typedef struct {
    int x, y;        // Posição do jogador
    int hasKey;      // Se o jogador possui a chave
    int moves;       // Contador de movimentos
} Player;

typedef struct {
    int FaseAtual;      // Fase atual do jogo
    int gameOver;       // Indica se o jogo terminou
    int totalGames;     // Total de jogos realizados
    int totalMoves;     // Total de movimentos realizados em todas as partidas
    int phasesCompleted; // Total de fases concluídas em todas as partidas
} Game;

typedef struct {
    char grid[ALTURA][LARGURA];  // Matriz que define o mapa
} Map;

// Função para desenhar o campo de jogo
void drawMap(Map *map) {
    system("cls");  // Limpa a tela
    int i, j;       // Variáveis declaradas fora do laço for
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (map->grid[i][j] == 'P') {
                printf("\033[41m  \033[0m");  // Jogador como quadrado vermelho
            } else if (map->grid[i][j] == '|') {
                printf("\033[48;5;69m  \033[0m");  // Parede como quadrado azul gelo escuro
            } else if (map->grid[i][j] == 'K') {
                printf("\033[33mK\033[0m ");  // Chave em amarelo
            } else if (map->grid[i][j] == 'M') {
                printf("\033[42m  \033[0m");  // Meta representada como um quadrado verde
            } else if (map->grid[i][j] == 'X') {
                printf("\033[44m  \033[0m");  // Trajeto já percorrido como um quadrado azul
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo azul bebê
            }
        }
        printf("\n");
    }
}

// Função para configurar o mapa da fase
void setupMap(Map *map, int phase) {
    int i, j;  // Variáveis declaradas fora do laço for
    // Limpa o mapa
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            map->grid[i][j] = '.';  // Espaço vazio
        }
    }

    // Adiciona paredes nas bordas
    for (i = 0; i < ALTURA; i++) {
        map->grid[i][0] = '|';
        map->grid[i][LARGURA - 1] = '|';
    }
    for (j = 0; j < LARGURA; j++) {
        map->grid[0][j] = '|';
        map->grid[ALTURA - 1][j] = '|';
    }

    // Configuração específica por fase
    if (phase == 1) {
        map->grid[4][4] = 'K';                   // Chave
        map->grid[1][LARGURA - 2] = 'M';         // Meta final
    } else if (phase == 2) {
        map->grid[4][4] = 'K';                   // Chave
        map->grid[ALTURA - 2][LARGURA - 2] = 'M';// Meta final
        map->grid[5][5] = '|';                   // Obstáculos
        map->grid[6][5] = '|';
    }
    // Adicione mais fases conforme necessário...
}

// Função para exibir o menu principal
int showMenu() {
    int choice;
    do {
        system("cls");
        printf("=== MENU PRINCIPAL ===\n");
        printf("1. Jogar\n");
        printf("2. Estatísticas\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 3);
    return choice;
}

// Função para exibir estatísticas
void showStatistics(Game *game) {
    system("cls");
    printf("=== ESTATÍSTICAS ===\n");
    printf("Total de jogos: %d\n", game->totalGames);
    printf("Total de movimentos: %d\n", game->totalMoves);
    printf("Fases concluídas: %d\n", game->phasesCompleted);
    printf("\nPressione qualquer tecla para voltar ao menu...");
    getch();
}

// Função principal
int main() {
    setlocale(LC_ALL, "");

    Game game = {1, 0, 0, 0, 0};   // Inicializa o jogo

    while (1) {
        int choice = showMenu();

        if (choice == 1) {
            // Jogar
            Player player = {1, 1, 0, 0};   // Inicializa o jogador
            Map map;                       // Inicializa o mapa

            game.FaseAtual = 1;
            game.gameOver = 0;

            setupMap(&map, game.FaseAtual);
            map.grid[player.y][player.x] = 'P';

            drawMap(&map);

            while (!game.gameOver) {
                if (_kbhit()) {
                    char input = _getch();  // Captura a tecla pressionada
                    int newX = player.x;
                    int newY = player.y;

                    if (input == 'w') newY--;
                    if (input == 's') newY++;
                    if (input == 'a') newX--;
                    if (input == 'd') newX++;

                    if (newX >= 0 && newX < LARGURA && newY >= 0 && newY < ALTURA) {
                        if (map.grid[newY][newX] != '|') {
                            if (map.grid[newY][newX] == 'K') {
                                player.hasKey = 1;
                            }
                            if (map.grid[newY][newX] == 'M') {
                                if (player.hasKey) {
                                    printf("Fase %d completa!\n", game.FaseAtual);
                                    usleep(1000000);
                                    game.FaseAtual++;
                                    game.phasesCompleted++;
                                    if (game.FaseAtual > TOTAL_DE_FASES) {
                                        game.gameOver = 1;
                                    } else {
                                        setupMap(&map, game.FaseAtual);
                                        player.x = player.y = 1;
                                        player.hasKey = 0;
                                        player.moves = 0;
                                    }
                                } else {
                                    printf("Pegue a chave primeiro!\n");
                                    usleep(500000);
                                    continue;
                                }
                            }
                            map.grid[player.y][player.x] = 'X';
                            player.x = newX;
                            player.y = newY;
                            map.grid[player.y][player.x] = 'P';
                            player.moves++;
                        }
                    }

                    drawMap(&map);
                }
                usleep(100000);
            }

            game.totalGames++;
            game.totalMoves += player.moves;

            printf("\nJogo terminado!\n");
            printf("Deseja jogar novamente? (s/n): ");
            char playAgain;
            scanf(" %c", &playAgain);
            if (playAgain != 's') break;

        } else if (choice == 2) {
            // Estatísticas
            showStatistics(&game);
        } else {
            // Sair
            printf("Saindo do jogo...\n");
            break;
        }
    }

    return 0;
}
