#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 6
#define NUM_MISSOES 3
#define TAM_NOME 30

typedef struct {
    char nome[TAM_NOME];
    char corExercito[TAM_NOME];
    int tropas;
} Territorio;

/* Protótipos */
Territorio *alocarMapa(void);
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa);
void exibirMissao(int missao, const char *corJogador);

void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *origem, Territorio *destino, const char *corJogador);

int sortearMissao(void);
int verificarVitoria(const Territorio *mapa, int missao, const char *corJogador);

void limparBufferEntrada(void);

/* Função principal */
int main() {
    setlocale(LC_ALL, "Portuguese");

    srand((unsigned int)time(NULL));

    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: não foi possível alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[TAM_NOME] = "Azul";
    int missao = sortearMissao();
    int opcao;
    int venceu = 0;

    do {
        system("cls || clear");
        printf("====================================================\n");
        printf("              PROJETO WAR ESTRUTURADO               \n");
        printf("====================================================\n");
        printf("Jogador: Exército %s\n\n", corJogador);

        exibirMapa(mapa);
        printf("\n");
        exibirMissao(missao, corJogador);
        printf("\n");
        exibirMenuPrincipal();

        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            limparBufferEntrada();
            opcao = -1;
        } else {
            limparBufferEntrada();
        }

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;

            case 2:
                if (verificarVitoria(mapa, missao, corJogador)) {
                    printf("\nParabéns! Você cumpriu sua missão e venceu o jogo!\n");
                    venceu = 1;
                } else {
                    printf("\nSua missão ainda não foi cumprida.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpção inválida.\n");
                break;
        }

        if (opcao != 0 && !venceu) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

/* Implementação das funções */

Territorio *alocarMapa(void) {
    return (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio *mapa) {
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].corExercito, "Azul");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].corExercito, "Vermelho");
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].corExercito, "Verde");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Peru");
    strcpy(mapa[3].corExercito, "Amarelo");
    mapa[3].tropas = 2;

    strcpy(mapa[4].nome, "Colombia");
    strcpy(mapa[4].corExercito, "Vermelho");
    mapa[4].tropas = 4;

    strcpy(mapa[5].nome, "Uruguai");
    strcpy(mapa[5].corExercito, "Azul");
    mapa[5].tropas = 3;
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

void exibirMenuPrincipal(void) {
    printf("--------------- MENU PRINCIPAL ----------------\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar missão\n");
    printf("0 - Sair\n");
    printf("-----------------------------------------------\n");
}

void exibirMapa(const Territorio *mapa) {
    int i;

    printf("--------------- MAPA DO JOGO ------------------\n");
    printf("%-3s %-15s %-12s %-6s\n", "ID", "Território", "Dono", "Tropas");
    printf("-----------------------------------------------\n");

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-15s %-12s %-6d\n",
               i,
               mapa[i].nome,
               mapa[i].corExercito,
               mapa[i].tropas);
    }

    printf("-----------------------------------------------\n");
}

void exibirMissao(int missao, const char *corJogador) {
    printf("Missão atual: ");
    switch (missao) {
        case 1:
            printf("Conquistar pelo menos 4 territórios.\n");
            break;
        case 2:
            printf("Conquistar pelo menos 3 territórios com no mínimo 2 tropas cada.\n");
            break;
        case 3:
            printf("Destruir totalmente o exército Vermelho.\n");
            if (strcmp(corJogador, "Vermelho") == 0) {
                printf("Como você é o Vermelho, essa missão deveria ser substituída.\n");
            }
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    int origem, destino;

    printf("\n--- Fase de Ataque ---\n");
    exibirMapa(mapa);

    printf("Informe o ID do território de origem: ");
    if (scanf("%d", &origem) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }

    printf("Informe o ID do território de destino: ");
    if (scanf("%d", &destino) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("IDs de territórios inválidos.\n");
        return;
    }

    simularAtaque(&mapa[origem], &mapa[destino], corJogador);
}

void simularAtaque(Territorio *origem, Territorio *destino, const char *corJogador) {
    int dadoAtaque, dadoDefesa;

    if (strcmp(origem->corExercito, corJogador) != 0) {
        printf("Você só pode atacar a partir de um território seu.\n");
        return;
    }

    if (strcmp(destino->corExercito, corJogador) == 0) {
        printf("Você não pode atacar um território que já é seu.\n");
        return;
    }

    if (origem->tropas <= 1) {
        printf("O território de origem precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    dadoAtaque = rand() % 6 + 1;
    dadoDefesa = rand() % 6 + 1;

    printf("\nAtaque: %s (%s) -> %s (%s)\n",
           origem->nome, origem->corExercito,
           destino->nome, destino->corExercito);

    printf("Dado do atacante: %d\n", dadoAtaque);
    printf("Dado do defensor: %d\n", dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        destino->tropas--;
        printf("O defensor perdeu 1 tropa.\n");

        if (destino->tropas <= 0) {
            printf("Território conquistado!\n");
            strcpy(destino->corExercito, corJogador);
            destino->tropas = 1;
            origem->tropas--;
        }
    } else {
        origem->tropas--;
        printf("O atacante perdeu 1 tropa.\n");
    }
}

int sortearMissao(void) {
    return rand() % NUM_MISSOES + 1;
}

int verificarVitoria(const Territorio *mapa, int missao, const char *corJogador) {
    int i;
    int territoriosDoJogador = 0;
    int territoriosCom2OuMais = 0;
    int vermelhoExiste = 0;

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].corExercito, corJogador) == 0) {
            territoriosDoJogador++;
            if (mapa[i].tropas >= 2) {
                territoriosCom2OuMais++;
            }
        }

        if (strcmp(mapa[i].corExercito, "Vermelho") == 0) {
            vermelhoExiste = 1;
        }
    }

    switch (missao) {
        case 1:
            return territoriosDoJogador >= 4;

        case 2:
            return territoriosCom2OuMais >= 3;

        case 3:
            return !vermelhoExiste;

        default:
            return 0;
    }
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* limpa o buffer */
    }
}