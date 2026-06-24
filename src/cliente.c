/**
 * @file cliente.c
 * @author Gabriel Bittencourt (Dev 1)
 * @brief Implementação do módulo de Cliente.
 * 
 * Contém a lógica de interface do usuário para clientes e a regra de negócio
 * para validação e persistência dos dados de clientes.
 */

#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "persistencia.h"
#include "input.h"
#include "logger.h"

void cliente_menu_cadastrar(void) {
    char nome[MAX_NOME];

    limpar_tela();
    printf("\n--- Cadastrar Cliente ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n\n");

    // Solicita o nome com tratamento de cancelamento
    if (ler_string("Digite o nome do cliente: ", nome, sizeof(nome))) {
        // Obtém o próximo ID para exibir na mensagem ao usuário
        int proximo_id = csv_obter_proximo_id("database/clientes.csv");

        int resultado = cliente_salvar(nome);
        if (resultado == 0) {
            // Sucesso no cadastro
            printf("\nCliente de ID %d cadastrado com sucesso!\n", proximo_id);
            registrar_log("SUCESSO: Cadastrou usuario de ID %d, nome '%s'.", proximo_id, nome);
        } else {
            // Erro de persistência ou validação
            printf("Erro ao cadastrar o cliente. Codigo de erro: %d\n", resultado);
            registrar_log("ERRO: Falha ao cadastrar cliente '%s' (Erro %d).", nome, resultado);
        }
    } else {
        printf("\nOperacao cancelada.\n");
    }
}

int cliente_salvar(const char* nome) {
    // Validar se o nome não é nulo ou vazio
    if (nome == NULL || strlen(nome) == 0) {
        return -1;
    }

    Cliente c;
    // Obter o próximo ID válido para cliente diretamente do CSV
    c.id = csv_obter_proximo_id("database/clientes.csv");

    // Copiar o nome de forma segura limitando ao tamanho do buffer
    strncpy(c.nome, nome, MAX_NOME - 1);
    c.nome[MAX_NOME - 1] = '\0';

    // Inserir via módulo de persistência (Ana Silva - Dev 3)
    return csv_inserir_cliente(&c);
}
