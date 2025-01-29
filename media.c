#include <stdio.h>

int main(){

char nome[50];
int idade;
int matricula;
float n1;
float n2, media;

printf("Qual o seu nome? \n");
scanf("%s", &nome);
printf("Qual a sua idade? \n");
scanf("%d", &idade);
printf("Qual a sua matricula? \n");
scanf("%d", &matricula);

printf("Digite sua nota 1: \n");
scanf("%f", &n1);
printf("Digite sua nota 2: \n");
scanf("%f", &n2);

printf("Seu nome é: %c\n", nome);
printf(" Sua idade é: %d\n", idade);


return 0;
}
