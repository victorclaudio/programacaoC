#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================================================
// Estrutura da sala da mansão (nós da árvore binária)
// =======================================================
typedef struct Sala {
    char nome[50];               // Nome do cômodo
    char pista[100];             // Pista opcional encontrada na sala
    struct Sala *esquerda;       // Caminho à esquerda
    struct Sala *direita;        // Caminho à direita
} Sala;

// =======================================================
// Estrutura da árvore de pistas (BST - Binary Search Tree)
// =======================================================
typedef struct PistaNode {
    char conteudo[100];          // Texto da pista
    struct PistaNode *esquerda;  // Subárvore à esquerda
    struct PistaNode *direita;   // Subárvore à direita
} PistaNode;

// =======================================================
// Função: criarSala()
// Objetivo: cria dinamicamente um cômodo da mansão,
// associado (ou não) a uma pista.
// Parâmetros: nome da sala e texto da pista
// Retorno: ponteiro para a nova sala
// =======================================================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        strcpy(novaSala->pista, ""); // sem pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// =======================================================
// Função: criarPistaNode()
// Objetivo: cria um novo nó da BST de pistas
// =======================================================
PistaNode* criarPistaNode(const char *conteudo) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista.\n");
        exit(1);
    }
    strcpy(novo->conteudo, conteudo);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// =======================================================
// Função: inserirPista()
// Objetivo: insere uma nova pista na BST, mantendo a
// ordem alfabética
// =======================================================
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    if (raiz == NULL)
        return criarPistaNode(conteudo);

    if (strcmp(conteudo, raiz->conteudo) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    else if (strcmp(conteudo, raiz->conteudo) > 0)
        raiz->direita = inserirPista(raiz->direita, conteudo);

    return raiz;
}

// =======================================================
// Função: exibirPistas()
// Objetivo: exibe as pistas em ordem alfabética (ordem
// simétrica - in-order traversal)
// =======================================================
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL)
        return;
    exibirPistas(raiz->esquerda);
    printf("• %s\n", raiz->conteudo);
    exibirPistas(raiz->direita);
}

// =======================================================
// Função: explorarSalasComPistas()
// Objetivo: controla a exploração da mansão e a coleta
// automática de pistas, armazenando-as na BST.
// =======================================================
void explorarSalasComPistas(Sala *salaAtual, PistaNode **raizPistas) {
    char escolha;

    if (salaAtual == NULL)
        return;

    printf("\nVocê entrou em: %s\n", salaAtual->nome);

    // Se houver uma pista na sala, coleta automaticamente
    if (strlen(salaAtual->pista) > 0) {
        printf("🔎 Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    } else {
        printf("Nada de interessante por aqui...\n");
    }

    while (1) {
        printf("\nEscolha seu próximo movimento:\n");
        if (salaAtual->esquerda != NULL)
            printf("  [e] Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  [d] Ir para a direita (%s)\n", salaAtual->direita->nome);
        printf("  [s] Sair da exploração e exibir pistas\n");
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            explorarSalasComPistas(salaAtual->esquerda, raizPistas);
            break;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            explorarSalasComPistas(salaAtual->direita, raizPistas);
            break;
        } else if (escolha == 's') {
            printf("\nEncerrando a exploração...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// =======================================================
// Função principal: main()
// Objetivo: constrói o mapa fixo da mansão, inicia a
// exploração e exibe as pistas coletadas ao final.
// =======================================================
int main() {
    // Criação do mapa (árvore binária da mansão)
    Sala *hall        = criarSala("Hall de Entrada", "Uma luva deixada no chão, tamanho médio.");
    Sala *salaEstar   = criarSala("Sala de Estar", "Um copo quebrado com resquícios de vinho.");
    Sala *biblioteca  = criarSala("Biblioteca", "Um livro aberto com uma assinatura misteriosa.");
    Sala *cozinha     = criarSala("Cozinha", "Uma faca limpa, recém-lavada.");
    Sala *jardim      = criarSala("Jardim", "Pegadas frescas que levam até o portão.");
    Sala *escritorio  = criarSala("Escritório", "Uma carta rasgada com as palavras 'me encontre'.");
    Sala *porao       = criarSala("Porão", "Um pedaço de tecido vermelho preso a um prego.");

    // Montagem da árvore binária da mansão
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->esquerda = escritorio;
    biblioteca->direita = porao;

    // Raiz da árvore de pistas (BST)
    PistaNode *raizPistas = NULL;

    // Início do jogo
    printf("======================================\n");
    printf("  Detective Quest - O Mistério da Mansão\n");
    printf("======================================\n");
    printf("Você é um detetive em busca do culpado.\n");
    printf("Explore os cômodos e colete as pistas!\n");

    explorarSalasComPistas(hall, &raizPistas);

    // Exibição final das pistas coletadas
    printf("\n======================================\n");
    printf("Pistas coletadas (em ordem alfabética):\n");
    printf("======================================\n");
    if (raizPistas == NULL)
        printf("Nenhuma pista foi encontrada.\n");
    else
        exibirPistas(raizPistas);

    printf("\nObrigado por jogar Detective Quest!\n");

    // Liberação de memória (simplificada)
    free(hall); free(salaEstar); free(biblioteca);
    free(cozinha); free(jardim); free(escritorio); free(porao);

    return 0;
}