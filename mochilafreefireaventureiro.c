#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// ===============================
// ESTRUTURAS DE DADOS
// ===============================

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// ===============================
// PROTÓTIPOS - VERSÃO VETOR
// ===============================
void inserirItemVetor(Item mochila[], int *total);
void removerItemVetor(Item mochila[], int *total);
void listarItensVetor(Item mochila[], int total);
int buscarSequencialVetor(Item mochila[], int total, char nome[], int *comparacoes);
void ordenarVetor(Item mochila[], int total);
int buscarBinariaVetor(Item mochila[], int total, char nome[], int *comparacoes);

// ===============================
// PROTÓTIPOS - VERSÃO LISTA
// ===============================
No* inserirItemLista(No *inicio);
No* removerItemLista(No *inicio, char nome[]);
void listarItensLista(No *inicio);
No* buscarSequencialLista(No *inicio, char nome[], int *comparacoes);
void liberarLista(No *inicio);

// ===============================
// FUNÇÃO AUXILIAR
// ===============================
void limparBuffer();

// ===============================
// PROGRAMA PRINCIPAL
// ===============================
int main() {
    Item mochilaVetor[MAX_ITENS];
    int totalItens = 0;
    No *inicio = NULL;
    int opcaoPrincipal;

    do {
        printf("\n=====================================\n");
        printf("     SISTEMA DE MOCHILA DO JOGADOR\n");
        printf("=====================================\n");
        printf("1 - Usar versao com Vetor\n");
        printf("2 - Usar versao com Lista Encadeada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcaoPrincipal);
        limparBuffer();

        switch (opcaoPrincipal) {
            case 1: {
                int opcaoVetor;
                do {
                    printf("\n=== MOCHILA (VETOR) ===\n");
                    printf("1 - Inserir item\n");
                    printf("2 - Remover item\n");
                    printf("3 - Listar itens\n");
                    printf("4 - Buscar (sequencial)\n");
                    printf("5 - Ordenar e buscar (binaria)\n");
                    printf("0 - Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &opcaoVetor);
                    limparBuffer();

                    if (opcaoVetor == 1) inserirItemVetor(mochilaVetor, &totalItens);
                    else if (opcaoVetor == 2) removerItemVetor(mochilaVetor, &totalItens);
                    else if (opcaoVetor == 3) listarItensVetor(mochilaVetor, totalItens);
                    else if (opcaoVetor == 4) {
                        char nome[30];
                        int comp = 0;
                        printf("Digite o nome do item: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        int pos = buscarSequencialVetor(mochilaVetor, totalItens, nome, &comp);
                        if (pos != -1)
                            printf("\nItem encontrado: %s | Tipo: %s | Qtd: %d\n",
                                   mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                        else
                            printf("\nItem nao encontrado!\n");
                        printf("Comparacoes: %d\n", comp);
                    } else if (opcaoVetor == 5) {
                        char nome[30];
                        int comp = 0;
                        ordenarVetor(mochilaVetor, totalItens);
                        printf("\nItens ordenados!\n");
                        listarItensVetor(mochilaVetor, totalItens);
                        printf("\nDigite o nome do item para busca binaria: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        int pos = buscarBinariaVetor(mochilaVetor, totalItens, nome, &comp);
                        if (pos != -1)
                            printf("\nItem encontrado: %s | Tipo: %s | Qtd: %d\n",
                                   mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                        else
                            printf("\nItem nao encontrado!\n");
                        printf("Comparacoes (busca binaria): %d\n", comp);
                    }

                } while (opcaoVetor != 0);
                break;
            }

            case 2: {
                int opcaoLista;
                do {
                    printf("\n=== MOCHILA (LISTA ENCADEADA) ===\n");
                    printf("1 - Inserir item\n");
                    printf("2 - Remover item\n");
                    printf("3 - Listar itens\n");
                    printf("4 - Buscar (sequencial)\n");
                    printf("0 - Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &opcaoLista);
                    limparBuffer();

                    if (opcaoLista == 1) inicio = inserirItemLista(inicio);
                    else if (opcaoLista == 2) {
                        char nome[30];
                        printf("Digite o nome do item a remover: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        inicio = removerItemLista(inicio, nome);
                    } else if (opcaoLista == 3) listarItensLista(inicio);
                    else if (opcaoLista == 4) {
                        char nome[30];
                        int comp = 0;
                        printf("Digite o nome do item: ");
                        fgets(nome, sizeof(nome), stdin);
                        nome[strcspn(nome, "\n")] = '\0';
                        No *res = buscarSequencialLista(inicio, nome, &comp);
                        if (res)
                            printf("\nItem encontrado: %s | Tipo: %s | Qtd: %d\n",
                                   res->dados.nome, res->dados.tipo, res->dados.quantidade);
                        else
                            printf("\nItem nao encontrado!\n");
                        printf("Comparacoes: %d\n", comp);
                    }

                } while (opcaoLista != 0);
                break;
            }

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcaoPrincipal != 0);

    liberarLista(inicio);
    return 0;
}

// ===============================
// IMPLEMENTAÇÃO - VERSÃO VETOR
// ===============================

void inserirItemVetor(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\nMochila cheia!\n");
        return;
    }

    Item novo;
    printf("\nNome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    limparBuffer();

    mochila[*total] = novo;
    (*total)++;
    printf("\nItem inserido com sucesso!\n");
}

void removerItemVetor(Item mochila[], int *total) {
    if (*total == 0) {
        printf("\nMochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *total - 1; j++)
                mochila[j] = mochila[j + 1];
            (*total)--;
            printf("\nItem removido com sucesso!\n");
            return;
        }
    }
    printf("\nItem nao encontrado!\n");
}

void listarItensVetor(Item mochila[], int total) {
    printf("\n--- ITENS NA MOCHILA (VETOR) ---\n");
    if (total == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("%d. %s | %s | Qtd: %d\n", i + 1,
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

int buscarSequencialVetor(Item mochila[], int total, char nome[], int *comparacoes) {
    for (int i = 0; i < total; i++) {
        (*comparacoes)++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void ordenarVetor(Item mochila[], int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (strcmp(mochila[i].nome, mochila[j].nome) > 0) {
                Item temp = mochila[i];
                mochila[i] = mochila[j];
                mochila[j] = temp;
            }
        }
    }
}

int buscarBinariaVetor(Item mochila[], int total, char nome[], int *comparacoes) {
    int inicio = 0, fim = total - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(mochila[meio].nome, nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// ===============================
// IMPLEMENTAÇÃO - VERSÃO LISTA
// ===============================

No* inserirItemLista(No *inicio) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocacao!\n");
        return inicio;
    }

    printf("\nNome: ");
    fgets(novo->dados.nome, sizeof(novo->dados.nome), stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(novo->dados.tipo, sizeof(novo->dados.tipo), stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    limparBuffer();

    novo->proximo = inicio;
    printf("\nItem inserido na lista!\n");
    return novo;
}

No* removerItemLista(No *inicio, char nome[]) {
    if (inicio == NULL) {
        printf("\nLista vazia!\n");
        return inicio;
    }

    No *atual = inicio, *anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\nItem nao encontrado!\n");
        return inicio;
    }

    if (anterior == NULL) inicio = atual->proximo;
    else anterior->proximo = atual->proximo;

    free(atual);
    printf("\nItem removido da lista!\n");
    return inicio;
}

void listarItensLista(No *inicio) {
    printf("\n--- ITENS NA LISTA ENCADEADA ---\n");
    if (inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    No *aux = inicio;
    int i = 1;
    while (aux != NULL) {
        printf("%d. %s | %s | Qtd: %d\n", i++, aux->dados.nome,
               aux->dados.tipo, aux->dados.quantidade);
        aux = aux->proximo;
    }
}

No* buscarSequencialLista(No *inicio, char nome[], int *comparacoes) {
    No *aux = inicio;
    while (aux != NULL) {
        (*comparacoes)++;
        if (strcmp(aux->dados.nome, nome) == 0)
            return aux;
        aux = aux->proximo;
    }
    return NULL;
}

void liberarLista(No *inicio) {
    No *aux;
    while (inicio != NULL) {
        aux = inicio;
        inicio = inicio->proximo;
        free(aux);
    }
}

// ===============================
// FUNÇÃO AUXILIAR
// ===============================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}