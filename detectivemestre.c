#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 101   // tamanho da tabela hash (primo simples)

// ----------------------------
// Estruturas
// ----------------------------
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char conteudo[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct HashEntry {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashEntry *proximo;
} HashEntry;

// ----------------------------
// Protótipos (principais funções)
// ----------------------------
/* criarSala() – cria dinamicamente um cômodo (Sala) com nome e pista. */
Sala* criarSala(const char *nome, const char *pista);

/* explorarSalas() – navega pela árvore de salas, coleta pistas automáticas e insere na BST. */
void explorarSalas(Sala *salaAtual, PistaNode **raizPistas, HashEntry *tabela[]);

/* inserirPista() / inserirPistaBST() – insere a pista coletada na árvore BST. */
PistaNode* inserirPistaBST(PistaNode *raiz, const char *conteudo);
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo); /* wrapper */

/* verificar existência de pista na BST (evita duplicatas) */
int existePistaBST(PistaNode *raiz, const char *conteudo);

/* exibirPistas() – imprime a BST em ordem alfabética (in-order traversal). */
void exibirPistasInOrder(PistaNode *raiz);

/* liberarPistasBST() – libera a memória da BST de pistas. */
void liberarPistasBST(PistaNode *raiz);

/* inserirNaHash() – insere associação pista -> suspeito na tabela hash. */
void inserirNaHash(HashEntry *tabela[], const char *pista, const char *suspeito);

/* encontrarSuspeito() – consulta a tabela hash e retorna o suspeito associado a uma pista (ou NULL). */
const char* encontrarSuspeito(HashEntry *tabela[], const char *pista);

/* liberarTabelaHash() – libera memória da tabela hash. */
void liberarTabelaHash(HashEntry *tabela[]);

/* verificarSuspeitoFinal() – pede a acusação do jogador e verifica se há >= 2 pistas que apontam para ele. */
void verificarSuspeitoFinal(PistaNode *raizPistas, HashEntry *tabela[]);

/* Helper: contar/listar pistas que apontam para um suspeito (retorna contagem) */
int listarPistasParaSuspeito(PistaNode *raizPistas, HashEntry *tabela[], const char *suspeito);

/* Inicializar tabela hash */
void inicializarTabelaHash(HashEntry *tabela[]);

/* Hash function (djb2) */
unsigned long hash_djb2(const char *str);

/* Funções utilitárias */
static void trim_newline(char *s);
static void trim_whitespace_inplace(char *s);
static int str_case_equal(const char *a, const char *b);

// ----------------------------
// Implementação
// ----------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro de alocacao para Sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, MAX_NOME-1); s->nome[MAX_NOME-1] = '\0';
    if (pista && pista[0] != '\0')
        strncpy(s->pista, pista, MAX_PISTA-1);
    else
        s->pista[0] = '\0';
    s->pista[MAX_PISTA-1] = '\0';
    s->esquerda = s->direita = NULL;
    return s;
}

PistaNode* inserirPistaBST(PistaNode *raiz, const char *conteudo) {
    if (!conteudo || conteudo[0] == '\0') return raiz;
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novo) { fprintf(stderr, "Erro de alocacao para PistaNode.\n"); exit(EXIT_FAILURE); }
        strncpy(novo->conteudo, conteudo, MAX_PISTA-1); novo->conteudo[MAX_PISTA-1] = '\0';
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    int cmp = strcmp(conteudo, raiz->conteudo);
    if (cmp < 0)
        raiz->esquerda = inserirPistaBST(raiz->esquerda, conteudo);
    else if (cmp > 0)
        raiz->direita = inserirPistaBST(raiz->direita, conteudo);
    /* se igual, não insere duplicata */
    return raiz;
}

PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    if (!existePistaBST(raiz, conteudo))
        return inserirPistaBST(raiz, conteudo);
    return raiz;
}

int existePistaBST(PistaNode *raiz, const char *conteudo) {
    if (!raiz) return 0;
    int cmp = strcmp(conteudo, raiz->conteudo);
    if (cmp == 0) return 1;
    if (cmp < 0) return existePistaBST(raiz->esquerda, conteudo);
    return existePistaBST(raiz->direita, conteudo);
}

void exibirPistasInOrder(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistasInOrder(raiz->esquerda);
    printf(" - %s\n", raiz->conteudo);
    exibirPistasInOrder(raiz->direita);
}

void liberarPistasBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistasBST(raiz->esquerda);
    liberarPistasBST(raiz->direita);
    free(raiz);
}

unsigned long hash_djb2(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

void inserirNaHash(HashEntry *tabela[], const char *pista, const char *suspeito) {
    if (!pista || !suspeito) return;
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashEntry *cur = tabela[h];
    while (cur) {
        if (strcmp(cur->pista, pista) == 0) {
            /* atualiza suspeito se já existe a pista */
            strncpy(cur->suspeito, suspeito, MAX_NOME-1);
            cur->suspeito[MAX_NOME-1] = '\0';
            return;
        }
        cur = cur->proximo;
    }
    HashEntry *novo = (HashEntry*) malloc(sizeof(HashEntry));
    if (!novo) { fprintf(stderr, "Erro de alocacao para HashEntry.\n"); exit(EXIT_FAILURE); }
    strncpy(novo->pista, pista, MAX_PISTA-1); novo->pista[MAX_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME-1); novo->suspeito[MAX_NOME-1] = '\0';
    novo->proximo = tabela[h];
    tabela[h] = novo;
}

const char* encontrarSuspeito(HashEntry *tabela[], const char *pista) {
    if (!pista) return NULL;
    unsigned long h = hash_djb2(pista) % HASH_SIZE;
    HashEntry *cur = tabela[h];
    while (cur) {
        if (strcmp(cur->pista, pista) == 0)
            return cur->suspeito;
        cur = cur->proximo;
    }
    return NULL;
}

void liberarTabelaHash(HashEntry *tabela[]) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashEntry *cur = tabela[i];
        while (cur) {
            HashEntry *next = cur->proximo;
            free(cur);
            cur = next;
        }
        tabela[i] = NULL;
    }
}

void inicializarTabelaHash(HashEntry *tabela[]) {
    for (int i = 0; i < HASH_SIZE; ++i) tabela[i] = NULL;
}

/* explorarSalas() – navegação interativa; coleta a pista da sala atual automaticamente
   e insere na BST (sem duplicatas). */
void explorarSalas(Sala *salaAtual, PistaNode **raizPistas, HashEntry *tabela[]) {
    if (!salaAtual) return;

    char escolha = '\0';
    printf("\n--- Sala atual: %s ---\n", salaAtual->nome);

    if (salaAtual->pista[0] != '\0') {
        printf("🔎 Pista encontrada: \"%s\"\n", salaAtual->pista);
        /* inserir na BST (evita duplicata) */
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);

        /* opcional: mostrar suspeito ligado à pista (se houver) */
        const char *sus = encontrarSuspeito(tabela, salaAtual->pista);
        if (sus) {
            printf("   (esta pista aponta para: %s)\n", sus);
        }
    } else {
        printf("Nada de relevante nesta sala.\n");
    }

    while (1) {
        printf("\nOpcoes:\n");
        if (salaAtual->esquerda) printf("  [e] Ir para a esquerda -> %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita)  printf("  [d] Ir para a direita -> %s\n", salaAtual->direita->nome);
        printf("  [s] Sair da exploracao e ir ao julgamento\n");
        printf("Sua escolha: ");

        if (scanf(" %c", &escolha) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} /* limpar */
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        if (escolha == 'e' && salaAtual->esquerda) {
            explorarSalas(salaAtual->esquerda, raizPistas, tabela);
            break;
        } else if (escolha == 'd' && salaAtual->direita) {
            explorarSalas(salaAtual->direita, raizPistas, tabela);
            break;
        } else if (escolha == 's') {
            printf("Encerrando a exploracao e seguindo para o julgamento...\n");
            /* consumir resto da linha, para garantir fgets posterior funcione */
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            break;
        } else {
            printf("Opcao invalida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

/* listarPistasParaSuspeito() – percorre a BST em ordem e imprime as pistas que apontam
   para o suspeito dado; retorna quantas pistas apontam para ele. */
int listarPistasParaSuspeito(PistaNode *raizPistas, HashEntry *tabela[], const char *suspeito) {
    if (!raizPistas) return 0;
    int count = 0;
    count += listarPistasParaSuspeito(raizPistas->esquerda, tabela, suspeito);
    const char *s = encontrarSuspeito(tabela, raizPistas->conteudo);
    if (s && str_case_equal(s, suspeito)) {
        printf(" * %s\n", raizPistas->conteudo);
        count++;
    }
    count += listarPistasParaSuspeito(raizPistas->direita, tabela, suspeito);
    return count;
}

/* verificarSuspeitoFinal() – exibe pistas coletadas, solicita o acusado e verifica
   se pelo menos duas pistas sustentam a acusacao. */
void verificarSuspeitoFinal(PistaNode *raizPistas, HashEntry *tabela[]) {
    char acusado[MAX_NOME];
    printf("\n=====================================\n");
    printf(" Fase Final: Pistas coletadas\n");
    printf("=====================================\n");

    if (!raizPistas) {
        printf("Nenhuma pista foi coletada durante a investigacao.\n");
    } else {
        printf("Pistas coletadas (ordem alfabetica):\n");
        exibirPistasInOrder(raizPistas);
    }

    printf("\nDigite o nome do suspeito que deseja acusar (ex: 'Sr. Almeida'): ");
    /* limpar buffer antes de fgets (caso haja sobras) */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    if (!fgets(acusado, sizeof(acusado), stdin)) {
        acusado[0] = '\0';
    }
    trim_newline(acusado);
    trim_whitespace_inplace(acusado);

    if (strlen(acusado) == 0) {
        printf("Nenhum nome valido informado. Encerrando sem acusacao.\n");
        return;
    }

    printf("\nVoce acusou: %s\n", acusado);
    printf("Verificando pistas que apontam para %s...\n", acusado);

    int matches = listarPistasParaSuspeito(raizPistas, tabela, acusado);
    printf("\nTotal de pistas que apontam para %s: %d\n", acusado, matches);

    if (matches >= 2) {
        printf("\nResultado: ACUSACAO SUSTENTADA ✅\n");
        printf("Existem evidencias suficientes para apoiar a acusacao contra %s.\n", acusado);
    } else {
        printf("\nResultado: ACUSACAO NAO SUSTENTADA ❌\n");
        printf("Nao ha evidencias suficientes para condenar %s. Procure mais pistas.\n", acusado);
    }
}

/* ----------------------------
   Funcoes utilitarias
   ---------------------------- */
static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}
static void trim_whitespace_inplace(char *s) {
    /* remove espacos inicias e finais, in-place */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len-1])) { s[len-1] = '\0'; len--; }
}
static int str_case_equal(const char *a, const char *b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

// ----------------------------
// main: monta mapa, inicializa hash, explora e julga
// ----------------------------
int main(void) {
    /* Criacao do mapa (arvore binaria da mansao) */
    Sala *hall       = criarSala("Hall de Entrada", "Luva de couro deixada no chao");
    Sala *salaEstar  = criarSala("Sala de Estar", "Copo quebrado com residuos de vinho");
    Sala *biblioteca = criarSala("Biblioteca", "Livro aberto com assinatura M.");
    Sala *cozinha    = criarSala("Cozinha", "Faca com marcas recentes");
    Sala *jardim     = criarSala("Jardim", "Pegadas molhadas ate o portao");
    Sala *escritorio = criarSala("Escritorio", "Carta rasgada contendo 'perdoe-me'");
    Sala *varanda    = criarSala("Varanda", "Cigarro com filtro especial");
    Sala *porao      = criarSala("Porao", "Pedaço de tecido vermelho preso a um prego");

    /* montagem fixa */
    hall->esquerda = salaEstar;
    hall->direita  = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = jardim;

    biblioteca->esquerda = escritorio;
    biblioteca->direita  = porao;

    cozinha->direita = varanda;  /* pequena ramificacao */

    /* inicializacao da BST de pistas (vazia) */
    PistaNode *raizPistas = NULL;

    /* inicializacao da tabela hash e preenchimento com associacoes pista->suspeito */
    HashEntry *tabela[HASH_SIZE];
    inicializarTabelaHash(tabela);

    inserirNaHash(tabela, "Luva de couro deixada no chao", "Sr. Almeida");
    inserirNaHash(tabela, "Copo quebrado com residuos de vinho", "Sra. Beatriz");
    inserirNaHash(tabela, "Livro aberto com assinatura M.", "Dr. Marcos");
    inserirNaHash(tabela, "Faca com marcas recentes", "Sr. Almeida");
    inserirNaHash(tabela, "Pegadas molhadas ate o portao", "Jovem Carlos");
    inserirNaHash(tabela, "Carta rasgada contendo 'perdoe-me'", "Sra. Beatriz");
    inserirNaHash(tabela, "Cigarro com filtro especial", "Dr. Marcos");
    inserirNaHash(tabela, "Pedaço de tecido vermelho preso a um prego", "Sr. Almeida");

    /* Boas-vindas e instrucoes */
    printf("==============================================\n");
    printf("   Detective Quest - Investigacao Final\n");
    printf("==============================================\n");
    printf("Voce inicia no Hall de Entrada. Explore a mansao e colete pistas.\n");
    printf("Comandos: [e] esquerda, [d] direita, [s] sair (ir ao julgamento)\n");

    /* exploracao interativa (coleta de pistas) */
    explorarSalas(hall, &raizPistas, tabela);

    /* fase de julgamento: exibir pistas e acusar */
    verificarSuspeitoFinal(raizPistas, tabela);

    /* liberacao de memoria */
    liberarPistasBST(raizPistas);
    liberarTabelaHash(tabela);

    /* liberar salas (sao ponteiros alocados estaticamente aqui) */
    free(hall); free(salaEstar); free(biblioteca); free(cozinha);
    free(jardim); free(escritorio); free(varanda); free(porao);

    printf("\nInvestigacao finalizada. Obrigado por jogar!\n");
    return 0;
}