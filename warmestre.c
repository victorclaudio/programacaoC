#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Programa: WAR Estruturado com Missões Estratégicas
    Funcionalidades:
    - Cadastro dinâmico de territórios
    - Sorteio de missões para cada jogador
    - Simulação de ataques entre territórios
    - Verificação automática de cumprimento de missão
    - Liberação de memória ao final
*/

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// === Protótipos das funções ===
Territorio* cadastrarTerritorios(int qtd);
void exibirTerritorios(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void liberarMemoria(Territorio* mapa, char* missoesJogadores[], int qtdJogadores);

int main() {
    srand(time(NULL)); // inicializa a semente para números aleatórios

    int qtdTerritorios, qtdJogadores;
    printf("=== WAR Estruturado com Missões Estratégicas ===\n\n");

    // Número de territórios
    printf("Digite o número de territórios: ");
    scanf("%d", &qtdTerritorios);

    // Número de jogadores
    printf("Digite o número de jogadores: ");
    scanf("%d", &qtdJogadores);

    // Cadastro de territórios
    Territorio* mapa = cadastrarTerritorios(qtdTerritorios);

    // Missões pré-definidas
    char* missoes[] = {
        "Conquistar 3 territorios",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar qualquer territorio com pelo menos 5 tropas",
        "Dominar 2 territorios de cores diferentes",
        "Sobreviver com pelo menos 1 territorio"
    };
    int totalMissoes = 5;

    // Alocação dinâmica das missões dos jogadores
    char** missoesJogadores = (char**) malloc(qtdJogadores * sizeof(char*));
    for (int i = 0; i < qtdJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(100 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoes, totalMissoes);
        printf("\nJogador %d recebeu a missão: %s\n", i + 1, missoesJogadores[i]);
    }

    // Loop principal do jogo
    int op, turno = 0, vencedor = -1;
    do {
        printf("\n--- Turno do Jogador %d ---\n", (turno % qtdJogadores) + 1);
        printf("1. Realizar ataque\n");
        printf("2. Exibir territorios\n");
        printf("0. Encerrar jogo\n");
        printf("Escolha: ");
        scanf("%d", &op);

        if (op == 1) {
            int idAtacante, idDefensor;
            printf("\nDigite o número do território atacante: ");
            scanf("%d", &idAtacante);
            printf("Digite o número do território defensor: ");
            scanf("%d", &idDefensor);

            if (idAtacante < 0 || idAtacante >= qtdTerritorios || idDefensor < 0 || idDefensor >= qtdTerritorios) {
                printf("IDs inválidos!\n");
            } else if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
                printf("Não é possível atacar um território da mesma cor!\n");
            } else if (mapa[idAtacante].tropas <= 1) {
                printf("O território atacante deve ter mais de 1 tropa!\n");
            } else {
                atacar(&mapa[idAtacante], &mapa[idDefensor]);
            }
        } else if (op == 2) {
            exibirTerritorios(mapa, qtdTerritorios);
        }

        // Verifica se algum jogador cumpriu sua missão
        for (int i = 0; i < qtdJogadores; i++) {
            // Aqui assumimos que a cor do jogador é a cor do primeiro território dele
            char corJogador[10] = "";
            for (int t = 0; t < qtdTerritorios; t++) {
                if (strcmp(mapa[t].cor, "") != 0) {
                    strcpy(corJogador, mapa[t].cor);
                    break;
                }
            }

            if (verificarMissao(missoesJogadores[i], mapa, qtdTerritorios, corJogador)) {
                vencedor = i;
                break;
            }
        }

        if (vencedor != -1) {
            printf("\n🎉 Jogador %d venceu! Cumpriu sua missão: %s\n", vencedor + 1, missoesJogadores[vencedor]);
            break;
        }

        turno++;
    } while (op != 0);

    // Liberação da memória
    liberarMemoria(mapa, missoesJogadores, qtdJogadores);

    printf("\nJogo encerrado.\n");
    return 0;
}

/* Cadastro dinâmico dos territórios */
Territorio* cadastrarTerritorios(int qtd) {
    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    for (int i = 0; i < qtd; i++) {
        printf("\nCadastro do território %d:\n", i);

        printf("Nome do território: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    return mapa;
}

/* Exibe todos os territórios */
void exibirTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritório %d:\n", i);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor: %s\n", mapa[i].cor);
        printf("  Tropas: %d\n", mapa[i].tropas);
    }
}

/* Simula ataque entre territórios */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nBatalha entre %s (atacante) e %s (defensor):\n",
           atacante->nome, defensor->nome);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu!\n");
        defensor->tropas -= 1;

        if (defensor->tropas <= 0) {
            printf("%s foi conquistado!\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2;
            atacante->tropas -= defensor->tropas;
        }
    } else {
        printf("Defensor resistiu!\n");
        atacante->tropas -= 1;
    }
}

/* Sorteia e atribui uma missão */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

/* Verifica se a missão foi cumprida */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) count++;
        }
        return (count >= 3);
    } else if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0) return 0;
        }
        return 1;
    } else if (strcmp(missao, "Conquistar qualquer territorio com pelo menos 5 tropas") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 5) return 1;
        }
    } else if (strcmp(missao, "Dominar 2 territorios de cores diferentes") == 0) {
        char cor1[10] = "", cor2[10] = "";
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                if (count == 0) strcpy(cor1, mapa[i].cor);
                else if (strcmp(cor1, mapa[i].cor) != 0) {
                    strcpy(cor2, mapa[i].cor);
                    return 1;
                }
                count++;
            }
        }
    } else if (strcmp(missao, "Sobreviver com pelo menos 1 territorio") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) return 1;
        }
        return 0;
    }

    return 0;
}

/* Libera toda a memória alocada dinamicamente */
void liberarMemoria(Territorio* mapa, char* missoesJogadores[], int qtdJogadores) {
    free(mapa);
    for (int i = 0; i < qtdJogadores; i++) {
        free(missoesJogadores[i]);
    }
    free(missoesJogadores);
}
