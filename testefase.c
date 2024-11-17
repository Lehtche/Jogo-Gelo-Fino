#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>  // Para usleep()
#include <conio.h>   // Para _kbhit() e _getch()

#define WIDTH 20
#define HEIGHT 20
#define TOTAL_PHASES 2  // Quantidade de fases no jogo

// Struct para representar o jogador
typedef struct {
    int x, y;      // Posição do jogador
    int hasKey;    // Se o jogador possui a chave
    int moves;     // Contador de movimentos
} Player;

// Struct para representar o jogo
typedef struct {
    int currentPhase;  // Fase atual do jogo
    int gameOver;      // Indica se o jogo terminou
} Game;

// Struct para representar o mapa
typedef struct {
    char grid[HEIGHT][WIDTH];  // Matriz que define o mapa
} Map;

// Função para desenhar o campo de jogo
void drawMap(Map *map) {
    system("cls");  // Limpa a tela no Windows
    int i, j;  // Variáveis declaradas fora do for
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (map->grid[i][j] == 'P') {
                printf("\033[42m  \033[0m");  // Jogador como quadrado verde
            } else if (map->grid[i][j] == '|') {
                printf("\033[48;5;69m  \033[0m");  // Parede como quadrado azul gelo escuro
            } else if (map->grid[i][j] == 'K') {
                printf("\033[33mK\033[0m ");  // Chave em amarelo
            } else if (map->grid[i][j] == 'M') {
                printf("\033[41m  \033[0m");  // Meta representada como um quadrado vermelho
            } else if (map->grid[i][j] == 'X') {
                printf("\033[44m  \033[0m");  // Trajeto já percorrido como um quadrado azul
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo azul bebê
            }
        }
        printf("\n");
    }
}

// Função para exibir estatísticas
void showStats(Game *game, Player *player) {
    printf("\n=== Estatísticas ===\n");
    printf("Fase Atual: %d/%d\n", game->currentPhase, TOTAL_PHASES);
    printf("Chave: %s\n", player->hasKey ? "Sim" : "Não");
    printf("Movimentos Realizados: %d\n", player->moves);
    printf("Posição do Jogador: (%d, %d)\n", player->x, player->y);
    if (game->gameOver) {
        if (game->currentPhase > TOTAL_PHASES) {
            printf("Estado: Jogo Concluído! Parabéns!\n");
        } else {
            printf("Estado: Você perdeu!\n");
        }
    } else {
        printf("Estado: Jogando...\n");
    }
    printf("=====================\n");
}

// Função para configurar o mapa da fase
void setupMap(Map *map, int phase) {
    int i, j;  // Variáveis declaradas fora do for
    // Limpa o mapa
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map->grid[i][j] = '.';  // Espaço vazio
        }
    }

    // Adiciona paredes nas bordas
    for (i = 0; i < HEIGHT; i++) {
        map->grid[i][0] = '|';
        map->grid[i][WIDTH - 1] = '|';
    }
    for (j = 0; j < WIDTH; j++) {
        map->grid[0][j] = '|';
        map->grid[HEIGHT - 1][j] = '|';
    }

    // Configuração específica por fase
    if (phase == 1) {
        //coluna x linha
       map->grid[2][2] = '|';
       map->grid[2][3] = '|';
       map->grid[2][4] = '|';
       map->grid[2][5] = '|';
       map->grid[2][7] = '|';
       map->grid[2][8] = '|';
       map->grid[2][10] = '|';
       map->grid[2][12] = '|';
       map->grid[2][13] = '|';
       map->grid[2][16] = '|';
       map->grid[2][18] = '|';

       map->grid[3][2]     = '|';
       map->grid[3][5]     = '|';
       map->grid[3][14] = '|';
       map->grid[3][16] = '|';

       map->grid[4][2]     = '|';

       map->grid[5][2]     = '|';

       map->grid[6][2]     = '|';

       map->grid[7][2]     = '|';
       map->grid[7][3]     = '|';
       map->grid[7][4]     = '|';
       map->grid[7][5]     = '|';



       map->grid[16][11] = '|';
       map->grid[16][12] = '|';
       map->grid[16][13] = '|';
       map->grid[16][14] = '|';
       map->grid[16][15] = '|';

       map->grid[17][15] = '|';
       map->grid[17][16] = '|';
       map->grid[17][17] = '|';
       map->grid[17][18] = '|';

        map->grid[3][3] = 'K';  // Chave
        map->grid[HEIGHT - 2][WIDTH - 2] = 'M';  // Meta
    } else if (phase == 2) {
        map->grid[5][5] = '|';
        map->grid[6][5] = '|';
        map->grid[7][5] = '|';
        map->grid[8][3] = '|';
        map->grid[4][4] = 'K';  // Chave
        map->grid[1][WIDTH - 2] = 'M';  // Meta
    }
}

// Função principal
int main() {
    setlocale(LC_ALL, "");

    Player player = {1, 1, 0, 0};   // Inicializa o jogador
    Game game = {1, 0};            // Inicializa o jogo
    Map map;                       // Inicializa o mapa

    setupMap(&map, game.currentPhase);
    map.grid[player.y][player.x] = 'P';  // Posiciona o jogador no mapa

    drawMap(&map);
    showStats(&game, &player);

    while (!game.gameOver) {
        if (_kbhit()) {
            char input = _getch();  // Captura a tecla pressionada
            int newX = player.x;
            int newY = player.y;

            // Movimenta o jogador com base na tecla pressionada
            if (input == 'w') newY--;  // Para cima
            if (input == 's') newY++;  // Para baixo
            if (input == 'a') newX--;  // Para esquerda
            if (input == 'd') newX++;  // Para direita

            // Verifica se a nova posição é válida
            if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                if (map.grid[newY][newX] != '|') {  // Não pode atravessar paredes
                    if (map.grid[newY][newX] == 'X') {
                        printf("Você pisou em um quadrado azul e perdeu!\n");
                        game.gameOver = 1;
                        usleep(1000000);
                        continue;
                    }

                    if (map.grid[newY][newX] == 'K') {
                        player.hasKey = 1;  // Pega a chave
                        printf("Você pegou a chave!\n");
                        usleep(500000);
                    }
                    if (map.grid[newY][newX] == 'M') {
                        if (player.hasKey) {
                            printf("Fase %d completa!\n", game.currentPhase);
                            usleep(1000000);
                            game.currentPhase++;
                            if (game.currentPhase > TOTAL_PHASES) {
                                game.gameOver = 1;
                            } else {
                                setupMap(&map, game.currentPhase);
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

                    map.grid[player.y][player.x] = 'X';  // Marca o caminho percorrido
                    player.x = newX;
                    player.y = newY;
                    map.grid[player.y][player.x] = 'P';
                    player.moves++;
                }
            }

            drawMap(&map);
            showStats(&game, &player);
        }
        usleep(100000);
    }

    return 0;
}
