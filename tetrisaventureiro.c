#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5   // Tamanho fixo da fila de peças futuras
#define TAMANHO_PILHA 3  // Capacidade máxima da pilha de reserva

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// ---------- Estrutura da Fila Circular ---------- //
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// ---------- Estrutura da Pilha Linear ---------- //
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

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

// Insere peça no final da fila
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n⚠️  Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->itens[f->fim] = p;
    f->qtd++;
}

// Remove e retorna a peça da frente da fila
Peca desenfileirar(Fila *f) {
    Peca removida = {' ', -1};

    if (filaVazia(f)) {
        printf("\n⚠️  Fila vazia! Nenhuma peça para remover.\n");
        return removida;
    }

    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->qtd--;

    return removida;
}

// Exibe o conteúdo da fila
void exibirFila(Fila *f) {
    printf("Fila de Peças: ");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }

    int i, pos = f->inicio;
    for (i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[pos].tipo, f->itens[pos].id);
        pos = (pos + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

// ---------- Funções da Pilha ---------- //

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAMANHO_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha uma peça no topo
void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = x;
}

// Desempilha e retorna a peça do topo
Peca desempilhar(Pilha *p) {
    Peca removida = {' ', -1};

    if (pilhaVazia(p)) {
        printf("\n⚠️  Pilha vazia! Nenhuma peça para usar.\n");
        return removida;
    }

    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// Exibe o conteúdo da pilha
void exibirPilha(Pilha *p) {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// ---------- Função que gera uma nova peça aleatória ---------- //
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------- Função principal ---------- //
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0;

    srand(time(NULL));  // Semente para geração aleatória
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca nova = gerarPeca(contadorID++);
        enfileirar(&fila, nova);
    }

    // Menu principal
    do {
        printf("\n===================== ESTADO ATUAL =====================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("========================================================\n");

        printf("\nOpções:\n");
        printf("1 - Jogar peça (remove da fila)\n");
        printf("2 - Reservar peça (fila → pilha)\n");
        printf("3 - Usar peça reservada (remove da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);

                    // Após jogar, gera uma nova peça e adiciona à fila
                    Peca nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça reservada: [%c %d]\n", reservada.tipo, reservada.id);

                    // Mantém a fila cheia
                    Peca nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                } else if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha de reserva está cheia!\n");
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n🧱 Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
                }
                break;
            }

            case 0:
                printf("\n👋 Encerrando o programa...\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}