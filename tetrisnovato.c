#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5  // Tamanho fixo da fila de peças

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;  // Índice do primeiro elemento
    int fim;     // Índice do último elemento
    int qtd;     // Quantidade atual de elementos na fila
} Fila;

// ---------- Funções da Fila ---------- //

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];  // Escolhe aleatoriamente entre os tipos
    nova.id = id;
    return nova;
}

// Insere uma nova peça ao final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️  Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }

    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->itens[f->fim] = p;
    f->qtd++;

    printf("\n✅ Peça [%c %d] adicionada à fila.\n", p.tipo, p.id);
}

// Remove a peça da frente da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
        return;
    }

    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->qtd--;

    printf("\n🎮 Peça jogada: [%c %d]\n", removida.tipo, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\n===== Fila de Peças =====\n");

    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }

    int i, pos = f->inicio;
    for (i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[pos].tipo, f->itens[pos].id);
        pos = (pos + 1) % TAMANHO_FILA;
    }

    printf("\n=========================\n");
}

// ---------- Função principal ---------- //

int main() {
    Fila fila;
    int opcao;
    int contadorID = 0;

    srand(time(NULL)); // Semente para gerar peças aleatórias
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca p = gerarPeca(contadorID++);
        enfileirar(&fila, p);
    }

    do {
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;

            case 2:
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                } else {
                    printf("\n⚠️  A fila já está cheia!\n");
                }
                break;

            case 0:
                printf("\n👋 Encerrando o programa...\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}