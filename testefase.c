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
    int temChave;
    int movimentos;
} Jogador;

typedef struct {
    int faseAtual;
    int fimDeJogo;
} Jogo;

typedef struct {
    char grade[ALTURA][LARGURA];
} Mapa;

typedef struct {
    int fasesConcluidas;
    int totalMovimentos;
    int chaveObtida;
} Estatisticas;

Estatisticas estatisticas = {0, 0, 0};  // Inicializa as estatísticas como zeradas

void desenharMapa(Mapa *mapa) {
    system("cls");
    int i, j;
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (mapa->grade[i][j] == 'P') {
                printf("\033[41m  \033[0m");  // Jogador
            } else if (mapa->grade[i][j] == '|') {
                printf("\033[48;5;69m  \033[0m");  // Parede
            } else if (mapa->grade[i][j] == 'K') {
                printf("\033[33mK\033[0m ");  // Chave
            } else if (mapa->grade[i][j] == 'M') {
                printf("\033[42m  \033[0m");  // Meta
            } else if (mapa->grade[i][j] == 'X') {
                printf("\033[44m  \033[0m");  // Caminho percorrido
            } else {
                printf("\033[48;5;153m. \033[0m");  // Fundo
            }
        }
        printf("\n");
    }
}

void exibirEstatisticas(Jogo *jogo, Jogador *jogador) {
    printf("\n=== Estatísticas ===\n");
    printf("Fase Atual: %d/%d\n", jogo->faseAtual, TOTAL_DE_FASES);
    printf("Chave Obtida: %s\n", jogador->temChave ? "Sim" : "Não");
    printf("Posição do Jogador: (%d, %d)\n", jogador->x, jogador->y);
    if (jogo->fimDeJogo) {
        if (jogo->faseAtual > TOTAL_DE_FASES) {
            printf("Estado: Jogo Concluído! Parabéns!\n");
        } else {
            printf("Estado: Você perdeu!\n");
        }
    } else {
        printf("Estado: Jogando...\n");
    }
    printf("=====================\n");
}

void configurarMapa(Mapa *mapa, int fase) {
    int i, j; 
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            mapa->grade[i][j] = '.';  // Espaço vazio
        }
    }
    for (i = 0; i < ALTURA; i++) {
        mapa->grade[i][0] = '|';
        mapa->grade[i][LARGURA - 1] = '|';
    }
    for (j = 0; j < LARGURA; j++) {
        mapa->grade[0][j] = '|';
        mapa->grade[ALTURA - 1][j] = '|';
    }
    // Configuração específica por fase
    if (fase == 1) {
        //coluna x linha
       mapa->grade[4][4] = 'K';  // Chave
        mapa->grade[1][LARGURA - 2] = 'M';  // Meta final
    } else if (fase == 2) {
        mapa->grade[5][5] = '|';
        mapa->grade[6][5] = '|';
        mapa->grade[7][5] = '|';
        mapa->grade[8][3] = '|';
        mapa->grade[4][4] = 'K';  // Chave
        mapa->grade[ALTURA - 2][LARGURA - 2] = 'M';  // Meta final 
    }else if (fase == 3) {
        mapa->grade[4][4] = '|';
        mapa->grade[4][5] = '|';
        mapa->grade[5][5] = '|';
        mapa->grade[6][5] = '|';
        mapa->grade[7][3] = '|';
        mapa->grade[7][4] = '|';
        mapa->grade[7][5] = '|';
        mapa->grade[3][2] = '|';
        mapa->grade[3][5]  = '|';
        mapa->grade[3][14] = '|';
        mapa->grade[3][16] = '|';
        mapa->grade[4][2] = '|';
        mapa->grade[4][5] = '|';
        mapa->grade[5][2] = '|';
        mapa->grade[8][4] = 'K';  // Chave
        mapa->grade[ALTURA - 3][LARGURA - 3] = 'M';  // Meta final 
    }
    else if (fase == 4){
 		mapa->grade[4][8] = '|';
        mapa->grade[3][5] = '|';
        mapa->grade[5][5] = '|';
        mapa->grade[6][5] = '|';
        mapa->grade[7][3] = '|';
        mapa->grade[8][4] = '|';
        mapa->grade[7][5] = '|';
    	mapa->grade[5][2] = '|';
        mapa->grade[3][5] = '|';
        mapa->grade[3][14] = '|';
        mapa->grade[3][16] = '|';
        mapa->grade[4][4] = '|';
  		mapa->grade[4][5] = '|';
        mapa->grade[5][2] = '|';
        mapa->grade[4][4] = '|';
 	    mapa->grade[4][5] = '|';
        mapa->grade[5][2] = '|';
        mapa->grade[3][4] = 'K';  // Chave
        mapa->grade[ALTURA - 4][LARGURA - 4] = 'M';  // Meta final
    }
    else if (fase == 5) {
        mapa->grade[2][2] = '|';
       mapa->grade[2][3] = '|';
       mapa->grade[2][4] = '|';
       mapa->grade[2][5] = '|';
       mapa->grade[2][7] = '|';
       mapa->grade[2][8] = '|';
       mapa->grade[2][10] = '|';
       mapa->grade[2][12] = '|';
       mapa->grade[2][13] = '|';
       mapa->grade[2][16] = '|';
       mapa->grade[2][18] = '|';

       mapa->grade[3][2]     = '|';
       mapa->grade[3][5]     = '|';
       mapa->grade[3][14] = '|';
       mapa->grade[3][16] = '|';

       mapa->grade[4][2] = '|';
	   mapa->grade[4][5] = '|';
       mapa->grade[5][2] = '|';

       mapa->grade[6][2] = '|';
       mapa->grade[7][2] = '|';
       mapa->grade[7][3] = '|';
       mapa->grade[7][4] = '|';
       mapa->grade[7][5] = '|';
	   mapa->grade[7][6] = '|';	
	   mapa->grade[7][7] = '|';	
	   mapa->grade[7][8] = '|';
	   
	   mapa->grade[8][2]     = '|';
	   mapa->grade[8][3]     = '|';
	   mapa->grade[8][9]     = '|';	
	   mapa->grade[8][14]     = '|';
	   mapa->grade[8][17]     = '|';
	   mapa->grade[8][18]     = '|';

	   mapa->grade[8][5] = '|';
	   
	   mapa->grade[5][9] = '|';
	   mapa->grade[7][9] = '|';
	   
	   mapa->grade[10][2] = '|';
	   mapa->grade[10][9] = '|';
	   mapa->grade[10][12] = '|';
	
	   mapa->grade[12][5] = '|';
	   mapa->grade[14][12] = '|';
	   mapa->grade[14][16] = '|';
	   mapa->grade[14][5] = '|';
	   mapa->grade[15][5] = '|';
	   
	   
	   mapa->grade[16][5] = '|';
       mapa->grade[16][18] = '|';
       mapa->grade[16][10] = '|';
       mapa->grade[16][11] = '|';
       mapa->grade[16][12] = '|';
       mapa->grade[16][13] = '|';
       mapa->grade[16][14] = '|';
       mapa->grade[16][15] = '|';

       mapa->grade[17][5] = '|';
       mapa->grade[17][9] = '|';
       mapa->grade[17][12] = '|';
       mapa->grade[17][16] = '|';
       mapa->grade[17][17] = '|';
       mapa->grade[17][18] = '|';

    mapa->grade[3][3] = 'K';  // Chave
    mapa->grade[ALTURA - 2][LARGURA - 2] = 'M';  // Meta final 
    }
}

void exibirMenu() {
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

void exibirInstrucoes() {
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

void iniciarJogo() {
    Jogador jogador = {1, 1, 0, 0}; 
    Jogo jogo = {1, 0};           
    Mapa mapa;

    configurarMapa(&mapa, jogo.faseAtual);
    mapa.grade[jogador.y][jogador.x] = 'P';

    desenharMapa(&mapa);
    exibirEstatisticas(&jogo, &jogador);

    while (!jogo.fimDeJogo) {
        if (_kbhit()) {
            char input = _getch();
            int novoX = jogador.x;
            int novoY = jogador.y;

            if (input == 'w') novoY--;
            if (input == 's') novoY++;
            if (input == 'a') novoX--;
            if (input == 'd') novoX++;

            if (novoX >= 0 && novoX < LARGURA && novoY >= 0 && novoY < ALTURA) {
                if (mapa.grade[novoY][novoX] != '|') { 
                    if (mapa.grade[novoY][novoX] == 'X') {
                        printf("Você pisou em um quadrado azul e perdeu!\n");
                        jogo.fimDeJogo = 1;
                        usleep(1000000);
                        continue;
                    }
                    if (mapa.grade[novoY][novoX] == 'K') {
                        jogador.temChave = 1;
                        estatisticas.chaveObtida = 1;
                        printf("Você pegou a chave!\n");
                        usleep(500000);
                    }
                    if (mapa.grade[novoY][novoX] == 'M') {
                        if (jogador.temChave) {
                            printf("Fase %d completa!\n", jogo.faseAtual);
                            usleep(1000000);
                            estatisticas.totalMovimentos += jogador.movimentos;
                            estatisticas.fasesConcluidas++;
                            jogo.faseAtual++;
                            if (jogo.faseAtual > TOTAL_DE_FASES) {
                                jogo.fimDeJogo = 1;
                                printf("Você venceu todas as fases!\n");
                                usleep(1000000);
                            } else {
                                configurarMapa(&mapa, jogo.faseAtual);
                                jogador.x = jogador.y = 1;
                                jogador.temChave = 0;
                                jogador.movimentos = 0;
                            }
                        } else {
                            printf("Pegue a chave primeiro!\n");
                            usleep(500000);
                            continue;
                        }
                    }
                    mapa.grade[jogador.y][jogador.x] = 'X'; 
                    jogador.x = novoX;
                    jogador.y = novoY;
                    mapa.grade[jogador.y][jogador.x] = 'P'; 
                    jogador.movimentos++;
                }
            }

            desenharMapa(&mapa);
            exibirEstatisticas(&jogo, &jogador);
        }
        usleep(100000);
    }
}

void exibirEstatisticasGerais() {
    system("cls");
    if (estatisticas.fasesConcluidas == 0) {
        printf("Você ainda não jogou nenhuma vez.\n");
    } else {
        printf("=== Estatísticas ===\n");
        printf("Fases Concluídas: %d/%d\n", estatisticas.fasesConcluidas, TOTAL_DE_FASES);
        printf("Total de Movimentos: %d\n", estatisticas.totalMovimentos);
        printf("Chave Obtida: %s\n", estatisticas.chaveObtida ? "Sim" : "Não");
        printf("=====================\n");
    }
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                iniciarJogo();
                break;
            case 2:
                exibirEstatisticasGerais();
                break;
            case 3:
                printf("Saindo do jogo... Até a próxima!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                usleep(1000000);
        }
    } while (opcao != 3);

    return 0;
}
