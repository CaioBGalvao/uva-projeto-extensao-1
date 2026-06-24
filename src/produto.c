/**
 * @file produto.c
 * @author Wallace Calisto (Dev 2)
 * @brief Implementação do módulo de Produto.
 * 
 * Implementa as funções para validação de produtos, recebimento de input
 * pelo terminal e a lógica de salvamento na base de dados.
 */

#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "persistencia.h"
#include "tipos.h"
#include "input.h"
#include "logger.h"

int produto_salvar(const char* nome, float preco) {
    // Valida se o nome não está vazio
    if (nome == NULL || nome[0] == '\0') {
        printf("Erro: nome do produto nao pode ser vazio.\n");
        return -1;
    }

    // Valida se o preço é estritamente positivo
    if (preco <= 0) {
        printf("Erro: preco deve ser maior que zero.\n");
        return -1;
    }

    // Pega o próximo ID disponível no arquivo de produtos
    int novo_id = csv_obter_proximo_id("database/produtos.csv");

    // Monta a struct Produto com os dados fornecidos
    Produto p;
    p.id = novo_id;
    strncpy(p.nome, nome, MAX_NOME - 1);
    p.nome[MAX_NOME - 1] = '\0';
    p.preco = preco;

    // Salva no CSV usando a função da camada de persistência
    int resultado = csv_inserir_produto(&p);
    if (resultado != 0) {
        printf("Erro ao salvar produto.\n");
        registrar_log("ERRO: Falha ao cadastrar produto '%s'.", nome);
        return -1;
    }

    // Notifica sucesso e registra a operação
    printf("Produto cadastrado com sucesso! ID: %d\n", novo_id);
    registrar_log("SUCESSO: Produto ID %d ('%s') cadastrado com preco R$ %.2f.", novo_id, nome, preco);
    return 0;
}

void produto_menu_cadastrar(void) {
    char nome[MAX_NOME];
    float preco;

    limpar_tela();
    printf("\n--- Cadastrar Produto ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n");
    printf("(Use Ctrl+B para Busca Rapida a qualquer momento)\n\n");

    // Pede o nome usando a utilidade de input (suporta Ctrl+D/Ctrl+Z e Ctrl+B)
    if (ler_string("Digite o nome do produto: ", nome, sizeof(nome))) {
        // Pede o preço com validação de float
        if (ler_float("Digite o preco unitario (ex: 19.90): ", &preco)) {
            // Chama a função de regra de negócio
            produto_salvar(nome, preco);
        } else {
            printf("\nOperacao cancelada (ou formato invalido).\n");
        }
    } else {
        printf("\nOperacao cancelada.\n");
    }
}
