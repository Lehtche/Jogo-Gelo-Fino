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
    int x, y;      // PosiÃ§Ã£o do jogador
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

// FunÃ§Ã£o para desenhar o campo de jogo
void drawMap(Map *map) {
    system("cls");  // Limpa a tela no Windows
    int i, j;  // VariÃ¡veis declaradas fora do for
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
                printf("\033[44m  \033[0m");  // Trajeto jÃ¡ percorrido como um quadrado azul
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo azul bebÃª
            }
        }
        printf("\n");
    }
}

// FunÃ§Ã£o para exibir estatÃ­sticas
void showStats(Game *game, Player *player) {
    printf("\n=== EstatÃ­sticas ===\n");
    printf("Fase Atual: %d/%d\n", game->currentPhase, TOTAL_PHASES);
    printf("Chave: %s\n", player->hasKey ? "Sim" : "NÃ£o");
    printf("Movimentos Realizados: %d\n", player->moves);
    printf("PosiÃ§Ã£o do Jogador: (%d, %d)\n", player->x, player->y);
    if (game->gameOver) {
        if (game->currentPhase > TOTAL_PHASES) {
            printf("Estado: Jogo ConcluÃ­do! ParabÃ©ns!\n");
        } else {
            printf("Estado: VocÃª perdeu!\n");
        }
    } else {
        printf("Estado: Jogando...\n");
    }
    printf("=====================\n");
}

// FunÃ§Ã£o para configurar o mapa da fase
void setupMap(Map *map, int phase) {
    int i, j;  // VariÃ¡veis declaradas fora do for
    // Limpa o mapa
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map->grid[i][j] = '.';  // EspaÃ§o vazio
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

    // ConfiguraÃ§Ã£o especÃ­fica por fase
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
	   map->grid[4][5]     = '|';
       map->grid[5][2]     = '|';

       map->grid[6][2]     = '|';

       map->grid[7][2]     = '|';
       map->grid[7][3]     = '|';
       map->grid[7][4]     = '|';
       map->grid[7][5]     = '|';
	   map->grid[7][6]     = '|';	
	   map->grid[7][7]     = '|';	
	   map->grid[7][8]     = '|';
	   
	   map->grid[8][2]     = '|';
	   map->grid[8][3]     = '|';
	   map->grid[8][9]     = '|';	
	   map->grid[8][14]     = '|';
	   map->grid[8][17]     = '|';
	   map->grid[8][18]     = '|';

	   map->grid[8][5] = '|';
	   
	   map->grid[5][9]     = '|';
	   map->grid[7][9]     = '|';
	   
	   map->grid[10][2]     = '|';
	   map->grid[10][9]     = '|';
	   map->grid[10][12] = '|';
	
	   map->grid[12][5] = '|';
	   map->grid[14][12] = '|';
	   map->grid[14][16] = '|';
	   map->grid[14][5] = '|';
	   map->grid[15][5] = '|';
	   
	   
	   map->grid[16][5] = '|';
       map->grid[16][18] = '|';
       map->grid[16][10] = '|';
       map->grid[16][11] = '|';
       map->grid[16][12] = '|';
       map->grid[16][13] = '|';
       map->grid[16][14] = '|';
       map->grid[16][15] = '|';

       map->grid[17][5] = '|';
       map->grid[17][9] = '|';
       map->grid[17][12] = '|';
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

// FunÃ§Ã£o principal
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

            // Verifica se a nova posiÃ§Ã£o Ã© vÃ¡lida
            if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                if (map.grid[newY][newX] != '|') {  // NÃ£o pode atravessar paredes
                    if (map.grid[newY][newX] == 'X') {
                        printf("VocÃª pisou em um quadrado azul e perdeu!\n");
                        game.gameOver = 1;
                        usleep(1000000);
                        continue;
                    }

                    if (map.grid[newY][newX] == 'K') {
                        player.hasKey = 1;  // Pega a chave
                        printf("VocÃª pegou a chave!\n");
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
