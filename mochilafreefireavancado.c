/*
  Módulo Avançado: Ordenação, Busca Binária Otimizada e Análise de Desempenho
  ----------------------------------------------------------------------------
  - Linguagem: C (ANSI/C89 compatível com clock() para medir tempo)
  - Funcionalidade:
      * Cadastro de até 20 componentes (nome, tipo, prioridade)
      * Três algoritmos de ordenação:
          - Bubble Sort por nome (string)
          - Insertion Sort por tipo (string)
          - Selection Sort por prioridade (int)
      * Busca sequencial por nome
      * Busca binária otimizada por nome (aplicável somente após ordenação por nome)
      * Contagem de comparações e medição de tempo (clock)
      * Menu interativo e feedback numérico (comparações e tempo)
  - Observações:
      * Use fgets() para entrada de strings; removemos '\n' de forma segura
      * Cada algoritmo tem sua função separada e produz comparações/tempo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade; // 1..10
} Componente;

/* ---------- Protótipos ---------- */
/* Entrada / exibição */
void lerComponente(Componente *c);
void mostrarComponentes(Componente arr[], int n);

/* Ordenações com contagem de comparações e tempo */
void bubbleSortNome(Componente arr[], int n, long *comparacoes, double *tempoSegundos);
void insertionSortTipo(Componente arr[], int n, long *comparacoes, double *tempoSegundos);
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes, double *tempoSegundos);

/* Buscas */
int buscaSequencialPorNome(Componente arr[], int n, const char *nome, long *comparacoes);
int buscaBinariaPorNome(Componente arr[], int n, const char *nome, long *comparacoes);

/* Utilitários */
void trocarComponente(Componente *a, Componente *b);
void limparBufferStdin();
void removerQuebraLinha(char *s);

/* ---------- Implementação ---------- */

int main(void) {
    Componente componentes[MAX_COMPONENTES];
    int total = 0;
    int opcao;
    int sortedByName = 0; // flag: 1 se atualmente ordenado por nome

    printf("=== Módulo Avançado: Montagem da Torre de Resgate ===\n");

    do {
        printf("\nMenu Principal:\n");
        printf("1 - Cadastrar componente (até %d)\n", MAX_COMPONENTES);
        printf("2 - Listar componentes\n");
        printf("3 - Ordenar (escolher algoritmo)\n");
        printf("4 - Buscar componente (sequencial por nome)\n");
        printf("5 - Buscar componente (binária por nome) [requer ordenacao por nome]\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferStdin();
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }
        limparBufferStdin();

        if (opcao == 1) {
            if (total >= MAX_COMPONENTES) {
                printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
            } else {
                printf("\nCadastrando componente #%d\n", total + 1);
                lerComponente(&componentes[total]);
                total++;
                sortedByName = 0; // alteração na coleção torna inválida qualquer ordenacao prévia
                printf("Componente cadastrado com sucesso.\n");
            }
        } else if (opcao == 2) {
            mostrarComponentes(componentes, total);
        } else if (opcao == 3) {
            if (total == 0) {
                printf("Nenhum componente cadastrado ainda.\n");
                continue;
            }

            int choiceSort;
            printf("\nEscolha o algoritmo de ordenacao:\n");
            printf("1 - Bubble Sort (por NOME)\n");
            printf("2 - Insertion Sort (por TIPO)\n");
            printf("3 - Selection Sort (por PRIORIDADE)\n");
            printf("0 - Voltar\n");
            printf("Opcao: ");
            if (scanf("%d", &choiceSort) != 1) {
                limparBufferStdin();
                printf("Entrada invalida.\n");
                continue;
            }
            limparBufferStdin();

            long comparacoes = 0;
            double tempo = 0.0;

            if (choiceSort == 1) {
                bubbleSortNome(componentes, total, &comparacoes, &tempo);
                printf("\n[RESULTADO] Bubble Sort (por nome) concluido.\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(componentes, total);
                sortedByName = 1;
            } else if (choiceSort == 2) {
                insertionSortTipo(componentes, total, &comparacoes, &tempo);
                printf("\n[RESULTADO] Insertion Sort (por tipo) concluido.\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(componentes, total);
                sortedByName = 0; // ordenado por tipo, nao garantimos ordenacao por nome
            } else if (choiceSort == 3) {
                selectionSortPrioridade(componentes, total, &comparacoes, &tempo);
                printf("\n[RESULTADO] Selection Sort (por prioridade) concluido.\n");
                printf("Comparacoes: %ld | Tempo: %.6f segundos\n", comparacoes, tempo);
                mostrarComponentes(componentes, total);
                sortedByName = 0;
            } else if (choiceSort == 0) {
                /* voltar */
            } else {
                printf("Opcao invalida.\n");
            }

        } else if (opcao == 4) {
            if (total == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            char nomeBusca[TAM_NOME];
            long comps = 0;
            printf("Digite o nome do componente para busca sequencial: ");
            if (fgets(nomeBusca, sizeof(nomeBusca), stdin) == NULL) {
                printf("Erro na leitura.\n");
                continue;
            }
            removerQuebraLinha(nomeBusca);
            int idx = buscaSequencialPorNome(componentes, total, nomeBusca, &comps);
            if (idx != -1) {
                printf("\nComponente encontrado na posicao %d:\n", idx);
                printf("  Nome: %s\n  Tipo: %s\n  Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            } else {
                printf("\nComponente NAO encontrado.\n");
            }
            printf("Comparacoes realizadas: %ld\n", comps);

        } else if (opcao == 5) {
            if (total == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            if (!sortedByName) {
                printf("A busca binaria por nome so e aplicavel se o vetor estiver ordenado por NOME.\n");
                printf("Execute Bubble Sort por NOME antes de usar busca binaria.\n");
                continue;
            }
            char nomeBusca[TAM_NOME];
            long comps = 0;
            printf("Digite o nome do componente para busca BINARIA: ");
            if (fgets(nomeBusca, sizeof(nomeBusca), stdin) == NULL) {
                printf("Erro na leitura.\n");
                continue;
            }
            removerQuebraLinha(nomeBusca);
            int idx = buscaBinariaPorNome(componentes, total, nomeBusca, &comps);
            if (idx != -1) {
                printf("\nComponente encontrado na posicao %d (posicao no vetor ordenado):\n", idx);
                printf("  Nome: %s\n  Tipo: %s\n  Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            } else {
                printf("\nComponente NAO encontrado.\n");
            }
            printf("Comparacoes realizadas (binaria): %ld\n", comps);

        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ---------- Funções de Entrada / Exibição ---------- */

/* Ler um Componente do teclado com validação da prioridade */
void lerComponente(Componente *c) {
    char buffer[64];
    printf("Nome (ate %d chars): ", TAM_NOME - 1);
    if (fgets(c->nome, sizeof(c->nome), stdin) == NULL) {
        c->nome[0] = '\0';
    } else {
        removerQuebraLinha(c->nome);
    }

    printf("Tipo (ate %d chars) [ex: controle, suporte, propulsao]: ", TAM_TIPO - 1);
    if (fgets(c->tipo, sizeof(c->tipo), stdin) == NULL) {
        c->tipo[0] = '\0';
    } else {
        removerQuebraLinha(c->tipo);
    }

    for (;;) {
        int pri;
        printf("Prioridade (1 a 10): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }
        if (sscanf(buffer, "%d", &pri) != 1) {
            printf("Formato invalido. Digite um numero inteiro.\n");
            continue;
        }
        if (pri < 1 || pri > 10) {
            printf("Prioridade fora do intervalo. Digite entre 1 e 10.\n");
            continue;
        }
        c->prioridade = pri;
        break;
    }
}

/* Mostrar o vetor de componentes formatado */
void mostrarComponentes(Componente arr[], int n) {
    int i;
    printf("\n--- Componentes (total: %d) ---\n", n);
    if (n == 0) {
        printf("[Nenhum componente cadastrado]\n");
        return;
    }
    for (i = 0; i < n; ++i) {
        printf("%2d) Nome: %-28s | Tipo: %-12s | Prioridade: %2d\n",
               i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

/* ---------- Algoritmos de Ordenação (com contadores e tempo) ---------- */

/* Bubble Sort por nome (alfabético).
   Conta comparações entre nomes (cada strcmp é contado como 1 comparação).
   Mede tempo de execução com clock().
*/
void bubbleSortNome(Componente arr[], int n, long *comparacoes, double *tempoSegundos) {
    int i, j;
    *comparacoes = 0;
    clock_t inicio = clock();

    for (i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                trocarComponente(&arr[j], &arr[j+1]);
                trocou = 1;
            }
        }
        if (!trocou) break; /* otimização: se ja ordenado, sai cedo */
    }

    clock_t fim = clock();
    *tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

/* Insertion Sort por tipo (string).
   Conta comparações ao comparar tipos (cada strcmp conta).
*/
void insertionSortTipo(Componente arr[], int n, long *comparacoes, double *tempoSegundos) {
    int i, j;
    *comparacoes = 0;
    clock_t inicio = clock();

    for (i = 1; i < n; ++i) {
        Componente chave = arr[i];
        j = i - 1;
        /* comparacoes: enquanto j >=0 e arr[j].tipo > chave.tipo */
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }

    clock_t fim = clock();
    *tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

/* Selection Sort por prioridade (int).
   Conta comparações entre prioridades (cada comparação conta).
*/
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes, double *tempoSegundos) {
    int i, j;
    *comparacoes = 0;
    clock_t inicio = clock();

    for (i = 0; i < n - 1; ++i) {
        int idxMin = i;
        for (j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[idxMin].prioridade) {
                idxMin = j;
            }
        }
        if (idxMin != i) {
            trocarComponente(&arr[i], &arr[idxMin]);
        }
    }

    clock_t fim = clock();
    *tempoSegundos = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

/* ---------- Buscas ---------- */

/* Busca sequencial por nome.
   Incrementa comparacoes a cada strcmp feita.
   Retorna índice se encontrado, ou -1 se não encontrado.
*/
int buscaSequencialPorNome(Componente arr[], int n, const char *nome, long *comparacoes) {
    int i;
    *comparacoes = 0;
    for (i = 0; i < n; ++i) {
        (*comparacoes)++;
        if (strcmp(arr[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Busca binária por nome (vetor DEVE estar ordenado por nome).
   Conta comparacoes a cada comparação com elemento do meio.
   Retorna índice se encontrado, ou -1.
*/
int buscaBinariaPorNome(Componente arr[], int n, const char *nome, long *comparacoes) {
    int inicio = 0;
    int fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[meio].nome, nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/* ---------- Utilitários ---------- */

void trocarComponente(Componente *a, Componente *b) {
    Componente tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Limpa o buffer stdin até '\n' ou EOF */
void limparBufferStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* esvazia */ }
}

/* Remove a quebra de linha final de uma string obtida via fgets */
void removerQuebraLinha(char *s) {
    size_t len;
    if (s == NULL) return;
    len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') s[len - 1] = '\0';
}