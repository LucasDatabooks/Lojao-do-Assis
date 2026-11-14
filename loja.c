#include <stdio.h>      // Biblioteca para entrada, saída e arquivos
#include <stdlib.h>     // Biblioteca para funções gerais
#include <string.h>     // Biblioteca para manipulação de strings
#include <ctype.h>      // Biblioteca para conversão de caracteres

int main() {

    produtos(*gerarlistaprodutos); // Chama a função de produtos
    excluir(*excluirlistaprodutos); // Chama a função de excluir produtos
    alterar(*alterarlistaprodutos); // Chama a função de alterar produtos
    checar(*produtos); // Chama a função de checar produtos
}

int gerarlistaprodutos (char *listaprodutos) {
    FILE *fp = fopen(listaprodutos, "w"); // Abre o arquivo para escrita
    if (fp == NULL) {
        printf("Lista de produtos não encontrada.\n");
        return 0;
    }
    fclose(fp);
    return 1;
}
int excluirlistaprodutos () {
    if (remove("listaprodutos.txt") == 0)
printf("Lista removida com sucesso!\n");
else
printf("Erro ao deletar lista!\n");
return 0;
}
int alterarlistaprodutos (char *listaprodutos) {
    FILE *fp = fopen(listaprodutos, "a"), *fp2 = fopen("templist", "w"), char (100) *linha;
    if (fp == NULL || fp2 == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    } 
}