#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5   // Tamanho da fila circular
#define TAMANHO_PILHA 3  // Tamanho da pilha de reserva

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// ---------------- Fila Circular ---------------- //
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) { return f->qtd == TAMANHO_FILA; }
int filaVazia(Fila *f) { return f->qtd == 0; }

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->itens[f->fim] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {' ', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->qtd--;
    return removida;
}

// ---------------- Pilha Linear ---------------- //
typedef struct {
    Peca itens[TAMANHO_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) { p->topo = -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAMANHO_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void empilhar(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = x;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {' ', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------------- Geração de peças ---------------- //
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------------- Exibição ---------------- //
void exibirFila(Fila *f) {
    printf("Fila de peças: ");
    if (filaVazia(f)) { printf("(vazia)\n"); return; }
    int pos = f->inicio;
    for (int i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[pos].tipo, f->itens[pos].id);
        pos = (pos + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) { printf("(vazia)\n"); return; }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// ---------------- Trocas ---------------- //
// Troca a peça da frente da fila com o topo da pilha
void trocarFrentePilha(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  Não é possível realizar troca: fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("\n🔄 Troca pontual realizada entre a frente da fila e o topo da pilha.\n");
}

// Troca os 3 primeiros da fila com as 3 da pilha (se possível)
void trocarTres(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("\n⚠️  Não é possível realizar troca múltipla: elementos insuficientes.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int posFila = (f->inicio + i) % TAMANHO_FILA;
        Peca temp = f->itens[posFila];
        f->itens[posFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
    printf("\n🔄 Troca múltipla realizada entre os 3 primeiros da fila e da pilha.\n");
}

// ---------------- Função principal ---------------- //
int main() {
    Fila fila;
    Pilha pilha;
    int contadorID = 0;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    do {
        printf("\n==================== ESTADO ATUAL ====================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("=======================================================\n");

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enfileirar(&fila, gerarPeca(contadorID++));
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça enviada para a pilha: [%c %d]\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca(contadorID++));
                }
                break;
            }
            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n🧱 Peça usada da pilha: [%c %d]\n", usada.tipo, usada.id);
                }
                break;
            }
            case 4:
                trocarFrentePilha(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("\n👋 Encerrando o programa...\n");
                break;
            default:
                printf("\n❌ Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}