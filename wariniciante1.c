#include <stdio.h>
#include <string.h>

/*
    Programa: Cadastro de Territórios (Estilo War)
    Objetivo: Demonstrar o uso de structs, vetores de structs, entrada e exibição de dados.
    Autor: [Seu Nome]
    Linguagem: C
*/

// Definição da struct Territorio
// Cada território terá: nome, cor do exército e quantidade de tropas
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor para armazenar 5 territórios
    Territorio territorios[5];
    int i;

    printf("=== Sistema de Cadastro de Territórios (Jogo War) ===\n\n");

    // Entrada dos dados de 5 territórios
    for (i = 0; i < 5; i++) {
        printf("Cadastro do território %d:\n", i + 1);

        // Nome do território
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", territorios[i].nome); // lê string com espaços

        // Cor do exército
        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        // Quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição dos dados após cadastro
    printf("\n=== Lista de Territórios Cadastrados ===\n\n");
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do exército: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("--------------------------------------\n");
    }

    return 0;
}