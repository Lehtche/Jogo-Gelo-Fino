#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>  // Para usleep()
#include <conio.h>   // Para _kbhit() e _getch()

#define LARGURA 20
#define ALTURA 20
#define TOTAL_DE_FASES 5

typedef struct {
    int x, y;
    int hasKey;
    int moves;
} Player;

typedef struct {
    int FaseAtual;
    int gameOver;
} Game;

typedef struct {
    char grid[ALTURA][LARGURA];
} Map;  // Adicionada a definição da estrutura Map

typedef struct {
    int fasesConcluidas;
    int totalMovimentos;
    int chavePegada;
} Estatisticas;

Estatisticas estatisticas = {0, 0, 0};  // Inicializa as estatísticas como zeradas

void drawMap(Map *map) {
    system("cls");
    int i, j;
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (map->grid[i][j] == 'P') {
                printf("\033[41m  \033[0m");  // Jogador
            } else if (map->grid[i][j] == '|') {
                printf("\033[48;5;69m  \033[0m");  // Parede
            } else if (map->grid[i][j] == 'K') {
                printf("\033[33mK\033[0m ");  // Chave
            } else if (map->grid[i][j] == 'M') {
                printf("\033[42m  \033[0m");  // Meta
            } else if (map->grid[i][j] == 'X') {
                printf("\033[44m  \033[0m");  // Caminho percorrido
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo
            }
        }
        printf("\n");
    }
}

// Restante do código...
void showStats(Game *game, Player *player) {
    printf("\n=== Estatísticas ===\n");
    printf("Fase Atual: %d/%d\n", game->FaseAtual, TOTAL_DE_FASES);
    printf("Chave Pegada: %s\n", player->hasKey ? "Sim" : "Não");
    printf("Posição do Jogador: (%d, %d)\n", player->x, player->y);
    if (game->gameOver) {
        if (game->FaseAtual > TOTAL_DE_FASES) {
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
    int i, j; 
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
        //coluna x linha
       map->grid[4][4] = 'K';  // Chave
        map->grid[1][LARGURA - 2] = 'M';  // Meta final
    } else if (phase == 2) {
        map->grid[5][5] = '|';
        map->grid[6][5] = '|';
        map->grid[7][5] = '|';
        map->grid[8][3] = '|';
        map->grid[4][4] = 'K';  // Chave
        map->grid[ALTURA - 2][LARGURA - 2] = 'M';  // Meta final 
    }else if (phase == 3) {
        map->grid[4][4] = '|';
        map->grid[4][5] = '|';
        map->grid[5][5] = '|';
        map->grid[6][5] = '|';
        map->grid[7][3] = '|';
        map->grid[7][4] = '|';
        map->grid[7][5] = '|';
        map->grid[3][2] = '|';
        map->grid[3][5]  = '|';
        map->grid[3][14] = '|';
        map->grid[3][16] = '|';
        map->grid[4][2] = '|';
        map->grid[4][5] = '|';
        map->grid[5][2] = '|';
        map->grid[8][4] = 'K';  // Chave
        map->grid[ALTURA - 3][LARGURA - 3] = 'M';  // Meta final 
    }
    else if (phase == 4){
 		map->grid[4][8] = '|';
        map->grid[3][5] = '|';
        map->grid[5][5] = '|';
        map->grid[6][5] = '|';
        map->grid[7][3] = '|';
        map->grid[8][4] = '|';
        map->grid[7][5] = '|';
    	map->grid[5][2] = '|';
        map->grid[3][5] = '|';
        map->grid[3][14] = '|';
        map->grid[3][16] = '|';
        map->grid[4][4] = '|';
  		map->grid[4][5] = '|';
        map->grid[5][2] = '|';
        map->grid[4][4] = '|';
 	    map->grid[4][5] = '|';
        map->grid[5][2] = '|';
        map->grid[3][4] = 'K';  // Chave
        map->grid[ALTURA - 4][LARGURA - 4] = 'M';  // Meta final
    }
    else if (phase == 5) {
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

       map->grid[4][2] = '|';
	   map->grid[4][5] = '|';
       map->grid[5][2] = '|';

       map->grid[6][2] = '|';
       map->grid[7][2] = '|';
       map->grid[7][3] = '|';
       map->grid[7][4] = '|';
       map->grid[7][5] = '|';
	   map->grid[7][6] = '|';	
	   map->grid[7][7] = '|';	
	   map->grid[7][8] = '|';
	   
	   map->grid[8][2]     = '|';
	   map->grid[8][3]     = '|';
	   map->grid[8][9]     = '|';	
	   map->grid[8][14]     = '|';
	   map->grid[8][17]     = '|';
	   map->grid[8][18]     = '|';

	   map->grid[8][5] = '|';
	   
	   map->grid[5][9] = '|';
	   map->grid[7][9] = '|';
	   
	   map->grid[10][2] = '|';
	   map->grid[10][9] = '|';
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
    map->grid[ALTURA - 2][LARGURA - 2] = 'M';  // Meta final 
    }
}

void displayMenu() {
    system("cls");
    printf("=====================\n");
    printf("     BEM-VINDO       \n");
    printf("=====================\n");
    printf("1 - Jogar\n");
    printf("2 - Estatísticas\n");
    printf("3 - Sair\n");
    printf("=====================\n");
    printf("Escolha uma opção: ");
}

void showInstructions() {
    system("cls");
    printf("=====================\n");
    printf("     INSTRUÇÕES       \n");
    printf("=====================\n");
    printf("- Use W, A, S, D para mover o jogador.\n");
    printf("- Pegue a chave (K) antes de ir até a meta (M).\n");
    printf("- Evite pisar no mesmo quadrado duas vezes.\n");
    printf("- Complete todas as fases para vencer.\n");
    printf("=====================\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
}

void startGame() {
    Player player = {1, 1, 0, 0}; // Inicializa o jogador
    Game game = {1, 0};           // Inicia na primeira fase
    Map map;

    setupMap(&map, game.FaseAtual);
    map.grid[player.y][player.x] = 'P'; // Marca o jogador no mapa

    drawMap(&map);
    showStats(&game, &player);

    while (!game.gameOver) {
        if (_kbhit()) {
            char input = _getch();
            int newX = player.x;
            int newY = player.y;

            if (input == 'w') newY--;
            if (input == 's') newY++;
            if (input == 'a') newX--;
            if (input == 'd') newX++;

            if (newX >= 0 && newX < LARGURA && newY >= 0 && newY < ALTURA) {
                if (map.grid[newY][newX] != '|') { // Movimenta somente em locais válidos
                    if (map.grid[newY][newX] == 'X') {
                        printf("Você pisou em um quadrado azul e perdeu!\n");
                        game.gameOver = 1;
                        usleep(1000000);
                        continue;
                    }
                    if (map.grid[newY][newX] == 'K') {
                        player.hasKey = 1;
                        estatisticas.chavePegada = 1;
                        printf("Você pegou a chave!\n");
                        usleep(500000);
                    }
                    if (map.grid[newY][newX] == 'M') {
                        if (player.hasKey) {
                            printf("Fase %d completa!\n", game.FaseAtual);
                            usleep(1000000);
                            estatisticas.totalMovimentos += player.moves;
                            estatisticas.fasesConcluidas++;
                            game.FaseAtual++;
                            if (game.FaseAtual > TOTAL_DE_FASES) {
                                game.gameOver = 1;
                                printf("Você venceu todas as fases!\n");
                                usleep(1000000);
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

                    map.grid[player.y][player.x] = 'X'; // Marca o caminho
                    player.x = newX;
                    player.y = newY;
                    map.grid[player.y][player.x] = 'P'; // Atualiza posição do jogador
                    player.moves++;
                }
            }

            drawMap(&map);
            showStats(&game, &player);
        }
        usleep(100000);
    }
}

void showEstatisticas() {
    system("cls");
    if (estatisticas.fasesConcluidas == 0) {
        printf("Você ainda não jogou nenhuma vez.\n");
    } else {
        printf("=== Estatísticas ===\n");
        printf("Fases Concluídas: %d/%d\n", estatisticas.fasesConcluidas, TOTAL_DE_FASES);
        printf("Total de Movimentos: %d\n", estatisticas.totalMovimentos);
        printf("Chave Pegada: %s\n", estatisticas.chavePegada ? "Sim" : "Não");
        printf("=====================\n");
    }
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
}

int main() {
    setlocale(LC_ALL, "");
    int option;

    do {
        displayMenu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                startGame();
                break;
            case 2:
                showEstatisticas();
                break;
            case 3:
                printf("Saindo do jogo... Até a próxima!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                usleep(1000000);
        }
    } while (option != 3);

    return 0;
}
