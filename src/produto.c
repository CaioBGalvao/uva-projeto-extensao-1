#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "persistencia.h"
#include "tipos.h"

int produto_salvar(const char* nome, float preco) {
    // Valida se o nome não está vazio
    if (nome == NULL || nome[0] == '\0') {
        printf("Erro: nome do produto nao pode ser vazio.\n");
        return -1;
    }

    // Valida se o preço é positivo
    if (preco <= 0) {
        printf("Erro: preco deve ser maior que zero.\n");
        return -1;
    }

    // Pega o próximo ID disponível no arquivo de produtos
    int novo_id = csv_obter_proximo_id("dados/produtos.csv");

    // Monta a struct Produto
    Produto p;
    p.id = novo_id;
    strncpy(p.nome, nome, MAX_NOME - 1);
    p.nome[MAX_NOME - 1] = '\0';
    p.preco = preco;

    // Salva no CSV usando a função do Caio
    int resultado = csv_inserir_produto(&p);
    if (resultado != 0) {
        printf("Erro ao salvar produto.\n");
        return -1;
    }

    printf("Produto cadastrado com sucesso! ID: %d\n", novo_id);
    return 0;
}

void produto_menu_cadastrar(void) {
    char nome[MAX_NOME];
    float preco;

    printf("\n--- Cadastrar Produto ---\n");

    // Pede o nome
    printf("Digite o nome do produto: ");
    scanf(" %254[^\n]", nome);

    // Pede o preço
    printf("Digite o preco unitario (ex: 19.90): ");
    scanf("%f", &preco);

    // Chama a função de salvar
    produto_salvar(nome, preco);
}