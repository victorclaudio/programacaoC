#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===========================================
// Estrutura que representa uma sala da mansão
// ===========================================
typedef struct Sala {
    char nome[50];              // Nome do cômodo
    struct Sala *esquerda;      // Caminho à esquerda
    struct Sala *direita;       // Caminho à direita
} Sala;

// ===========================================
// Função: criarSala()
// Objetivo: cria dinamicamente uma nova sala
// Parâmetros: nome da sala (string)
// Retorno: ponteiro para a nova sala
// ===========================================
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ===========================================
// Função: explorarSalas()
// Objetivo: permite que o jogador explore a
// mansão interativamente, escolhendo o caminho
// à esquerda (e) ou à direita (d).
// Parâmetros: ponteiro para a sala atual
// ===========================================
void explorarSalas(Sala *salaAtual) {
    char escolha;

    if (salaAtual == NULL)
        return;

    printf("\nVocê está agora em: %s\n", salaAtual->nome);

    // Caso a sala não tenha mais caminhos (nó-folha)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Você chegou ao fim da exploração. Não há mais caminhos.\n");
        return;
    }

    while (1) {
        printf("\nEscolha um caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf("  [e] Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  [d] Ir para a direita (%s)\n", salaAtual->direita->nome);
        printf("  [s] Sair da exploração\n");

        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            explorarSalas(salaAtual->esquerda);
            break;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            explorarSalas(salaAtual->direita);
            break;
        } else if (escolha == 's') {
            printf("Você decidiu sair da exploração. Até a próxima!\n");
            break;
        } else {
            printf("Escolha inválida! Tente novamente.\n");
        }
    }
}

// ===========================================
// Função: main()
// Objetivo: cria o mapa da mansão (árvore binária),
// inicia o jogo e chama a função de exploração.
// ===========================================
int main() {
    // Criação automática do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");
    Sala *porao = criarSala("Porão");

    // Montagem da estrutura da árvore binária
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = porao;

    // Início da exploração
    printf("======================================\n");
    printf("   Detective Quest - A Mansão Misteriosa\n");
    printf("======================================\n");
    printf("Bem-vindo ao Hall de Entrada!\n");
    explorarSalas(hall);

    // Liberação de memória
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(escritorio);
    free(porao);

    printf("\nExploração encerrada. Obrigado por jogar!\n");
    return 0;
}
