#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Programa: Simulação simplificada do jogo War
    Funcionalidades:
    - Cadastro dinâmico de territórios
    - Exibição dos territórios
    - Simulação de ataques entre territórios (com dados aleatórios)
    - Atualização de dados após batalhas
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
void liberarMemoria(Territorio* mapa);

int main() {
    srand(time(NULL)); // inicializa a semente para números aleatórios

    int qtd, op;
    printf("=== Simulador Simplificado de WAR ===\n\n");

    // Pergunta ao usuário quantos territórios deseja criar
    printf("Digite o número de territórios a serem cadastrados: ");
    scanf("%d", &qtd);

    // Alocação dinâmica do vetor de territórios
    Territorio* mapa = cadastrarTerritorios(qtd);

    // Exibe os territórios cadastrados
    printf("\n=== Territórios cadastrados ===\n");
    exibirTerritorios(mapa, qtd);

    // Menu de ações
    do {
        printf("\n--- Menu ---\n");
        printf("1. Realizar ataque\n");
        printf("2. Exibir territórios\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        if (op == 1) {
            int idAtacante, idDefensor;

            printf("\nDigite o número do território atacante: ");
            scanf("%d", &idAtacante);
            printf("Digite o número do território defensor: ");
            scanf("%d", &idDefensor);

            if (idAtacante < 0 || idAtacante >= qtd || idDefensor < 0 || idDefensor >= qtd) {
                printf("IDs inválidos!\n");
            } else if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
                printf("Um território não pode atacar outro da mesma cor!\n");
            } else if (mapa[idAtacante].tropas <= 1) {
                printf("O território atacante deve ter mais de 1 tropa para atacar!\n");
            } else {
                atacar(&mapa[idAtacante], &mapa[idDefensor]);
            }
        } else if (op == 2) {
            exibirTerritorios(mapa, qtd);
        }

    } while (op != 0);

    // Liberação da memória
    liberarMemoria(mapa);

    printf("\nJogo encerrado. Memória liberada com sucesso.\n");
    return 0;
}

/* 
    Função: cadastrarTerritorios
    Objetivo: alocar dinamicamente memória e cadastrar territórios
*/
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

/* 
    Função: exibirTerritorios
    Objetivo: exibir todos os territórios cadastrados
*/
void exibirTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritório %d:\n", i);
        printf("  Nome: %s\n", mapa[i].nome);
        printf("  Cor: %s\n", mapa[i].cor);
        printf("  Tropas: %d\n", mapa[i].tropas);
    }
}

/* 
    Função: atacar
    Objetivo: simular um ataque entre dois territórios
    Regras:
    - Cada lado rola um dado (1 a 6)
    - Se atacante ganhar: defensor perde tropas, podendo mudar de dono
    - Se defensor ganhar: atacante perde uma tropa
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nBatalha entre %s (atacante) e %s (defensor):\n",
           atacante->nome, defensor->nome);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: Atacante venceu!\n");
        defensor->tropas -= 1;

        // Se defensor perdeu todas as tropas, muda de dono
        if (defensor->tropas <= 0) {
            printf("%s foi conquistado!\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2; // metade das tropas invadem
            atacante->tropas = atacante->tropas - defensor->tropas; // restante fica
        }
    } else {
        printf("Resultado: Defensor resistiu!\n");
        atacante->tropas -= 1;
    }

    printf("\n--- Situação Atualizada ---\n");
    printf("Atacante: %s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) - Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

/* 
    Função: liberarMemoria
    Objetivo: liberar a memória alocada para os territórios
*/
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}