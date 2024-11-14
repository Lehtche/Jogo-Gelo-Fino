#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <conio.h>  // Para a função _kbhit() e _getch()

#define WIDTH 20
#define HEIGHT 20

// Função para desenhar o campo de jogo
void drawMap(char map[HEIGHT][WIDTH]) {
    system("cls");  // Limpa a tela 

    int i, j;

    // Imprime o campo de jogo
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Se for o personagem 'P', pinta de vermelho
            if (map[i][j] == 'P') {
                printf("\033[31mP\033[0m ");  // 'P' em vermelho
            } 
            // Se for uma parede '|', pinta de verde
            else if (map[i][j] == '|') {
                printf("\033[32m|\033[0m ");  // Parede em verde
            }
            // Se for o item-chave 'K', pinta de azul claro
            else if (map[i][j] == 'K') {
                printf(" ");  // Item-chave em azul claro
            }
            // Se for a meta 'M', pinta de amarelo
            else if (map[i][j] == 'M') {
                printf("\033[33mM\033[0m ");  // Meta em amarelo
            }
            // Se for um espaço 'X', pinta de azul escuro
            else if (map[i][j] == 'X') {
                printf("\033[34mX\033[0m ");  // 'X' em azul escuro
            }
            // Caso contrário, espaço vazio (fundo azul bebê)
            else {
                printf("\033[48;5;153m. \033[0m");  // Fundo azul bebê, espaço vazio
            }
        }
        printf("\n");
    }
}

// Função para configurar o mapa da fase
void setupMap(char map[HEIGHT][WIDTH], int phase) {
    int i, j;

    // Limpar o mapa
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            map[i][j] = '.';  // O campo começa vazio
        }
    }

    // Adicionar paredes nas bordas
    for (i = 0; i < HEIGHT; i++) {
        map[i][0] = '|';  // Parede esquerda
        map[i][WIDTH - 1] = '|';  // Parede direita
    }
    for (j = 0; j < WIDTH; j++) {
        map[0][j] = '|';  // Parede superior
        map[HEIGHT - 1][j] = '|';  // Parede inferior
    }

    // Configurar mapa com base na fase
    if (phase == 1) {
        // Fase 1 - Mapa simples
        map[HEIGHT-2][WIDTH-2] = 'M';  // Meta em (WIDTH-2, HEIGHT-2)
    } else if (phase == 2) {
        // Fase 2 - Mapa com mais obstáculos
        map[HEIGHT-2][1] = 'M';  // Meta em (1, HEIGHT-2)
    } else if (phase == 3) {
        // Fase 3 - Mais obstáculos e item-chave
        map[5][5] = '|';  // Parede em (5,5)
        map[5][6] = '|';  // Parede em (5,6)
        map[6][6] = '|';  // Parede em (6,6)
        map[6][7] = '|';  // Parede em (6,7)
        map[7][7] = '|';  // Parede em (7,7)
        map[7][8] = '|';  // Parede em (7,8)
        
        // Adicionar o item-chave 'K' que o jogador deve pegar
        map[2][2] = 'K';  // Item-chave na posição (2,2)

        // A meta 'M' só pode ser acessada após pegar o item-chave
        map[HEIGHT-2][WIDTH-2] = 'M';  // Meta em (WIDTH-2, HEIGHT-2)
    } else if (phase == 4) {
        // Fase 4 - Novo mapa com mais desafios
        // Criar um labirinto mais complicado
        map[3][5] = '|';
        map[4][5] = '|';
        map[5][5] = '|';
        map[6][5] = '|';
        map[6][6] = '|';
        map[6][7] = '|';
        map[7][7] = '|';
        map[8][7] = '|';
        map[9][7] = '|';
        
        // Adicionar um item-chave 'K' em (2,3)
        map[2][3] = 'K';

        // A meta 'M' fica no canto oposto
        map[HEIGHT-2][1] = 'M';  // Meta em (1, HEIGHT-2)
    }
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");

    // Definir o campo de jogo
    char map[HEIGHT][WIDTH];
    int playerX = 1, playerY = 1;  // Posição inicial do jogador dentro do campo
    char input;
    int gameOver = 0;
    int phase = 1;  // Inicia na fase 1
    int hasKey = 0; // Variável para verificar se o jogador tem a chave

    // Configurar o mapa para a fase atual
    setupMap(map, phase);
    map[playerY][playerX] = 'P';  // 'P' representa o jogador

    // Desenhar o campo inicial
    drawMap(map);

    while (!gameOver) {
        // Ler a entrada do jogador
        if (_kbhit()) {
            input = _getch();  // Captura a tecla pressionada
            int newX = playerX;
            int newY = playerY;

            // Mover o personagem baseado na tecla pressionada
            if (input == 'w') newY--;  // Mover para cima
            if (input == 's') newY++;  // Mover para baixo
            if (input == 'a') newX--;  // Mover para a esquerda
            if (input == 'd') newX++;  // Mover para a direita

            // Verificar se a nova posição é válida
            if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                // Se a posição não for uma parede '|'
                if (map[newY][newX] != '|' && map[newY][newX] != 'X') {
                    // Se for o item-chave 'K', o jogador pega a chave
                    if (map[newY][newX] == 'K') {
                        hasKey = 1;  // Jogador agora tem a chave
                        printf("Você pegou a chave!\n");
                        usleep(500000);  // Espera por 500ms
                    }
                    // Se a posição for válida, move o personagem
                    map[playerY][playerX] = 'X';  // Marca o lugar onde o jogador passou
                    playerX = newX;
                    playerY = newY;
                    map[playerY][playerX] = 'P';  // Coloca o jogador na nova posição
                } else {
                    // Se a posição for uma parede ou um lugar já visitado
                    printf("Você morreu!\n");
                    usleep(500000);  // Espera por 500ms
                    gameOver = 1;
                }
            }

            // Desenhar o campo atualizado
            drawMap(map);

            // Checar se o jogador atingiu a meta
            if (map[playerY][playerX] == 'M') {
                if (hasKey) {
                    printf("Você completou a fase %d!\n", phase);
                    usleep(1000000);  // Espera por 1 segundo
                    phase++;  // Avança para a próxima fase
                    if (phase > 4) {
                        printf("Você venceu o jogo!\n");
                        gameOver = 1;
                    } else {
                        // Passa para a próxima fase
                        setupMap(map, phase);
                        map[playerY][playerX] = 'P';  // Coloca o jogador de volta no início
                        drawMap(map);
                    }
                } else {
                    printf("Você precisa pegar a chave primeiro!\n");
                    usleep(1000000);  // Espera por 1 segundo
                }
            }
        }

        usleep(100000);  // Aguarda 100ms
    }

    return 0;
}

