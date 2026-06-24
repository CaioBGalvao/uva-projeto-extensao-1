#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "persistencia.h"
#include "input.h"
#include "logger.h"

// Responsável pela interação com o usuário via terminal
void cliente_menu_cadastrar(void) {
    char nome[MAX_NOME];
    
    limpar_tela();
    printf("\n--- Cadastrar Cliente ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n\n");

    // Solicita o nome
    if (ler_string("Digite o nome do cliente: ", nome, sizeof(nome))) {
        // Obtém o próximo ID para exibir na mensagem
        int proximo_id = csv_obter_proximo_id("database/clientes.csv");
        
        int resultado = cliente_salvar(nome);
        if (resultado == 0) {
            // Mensagem de sucesso idêntica a cliente_bittencurt.c
            printf("\nCliente de ID %d cadastrado com sucesso!\n", proximo_id);
            registrar_log("SUCESSO: Cadastrou usuario de ID %d, nome '%s'.", proximo_id, nome);
        } else {
            printf("Erro ao cadastrar o cliente. Codigo de erro: %d\n", resultado);
            registrar_log("ERRO: Falha ao cadastrar cliente '%s' (Erro %d).", nome, resultado);
        }
    } else {
        printf("\nOperacao cancelada.\n");
    }
}

// Regra de negócio testável: recebe dados, valida e salva usando persistencia.h
// Retorna 0 em caso de sucesso ou código de erro negativo.
int cliente_salvar(const char* nome) {
    // Validar se o nome não é nulo ou vazio
    if (nome == NULL || strlen(nome) == 0) {
        return -1;
    }
    
    Cliente c;
    // Obter o próximo ID válido para cliente
    c.id = csv_obter_proximo_id("database/clientes.csv");
    
    // Copiar o nome de forma segura
    strncpy(c.nome, nome, MAX_NOME - 1);
    c.nome[MAX_NOME - 1] = '\0';
    
    // Inserir via módulo de persistência
    return csv_inserir_cliente(&c);
}
