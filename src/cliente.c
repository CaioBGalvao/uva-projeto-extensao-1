#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "persistencia.h"

// Responsável pela interação com o usuário via terminal
void cliente_menu_cadastrar(void) {
    char nome[MAX_NOME];
    
    printf("\n--- Cadastrar Cliente ---\n");
    printf("Nome do cliente: ");
    
    // Ler o nome usando fgets para lidar com espaços
    if (fgets(nome, sizeof(nome), stdin) != NULL) {
        // Remover a quebra de linha (\n) lida pelo fgets
        size_t len = strlen(nome);
        if (len > 0 && nome[len - 1] == '\n') {
            nome[len - 1] = '\0';
        }
        
        int resultado = cliente_salvar(nome);
        if (resultado == 0) {
            printf("Cliente '%s' cadastrado com sucesso!\n", nome);
        } else {
            printf("Erro ao cadastrar o cliente '%s'. Codigo de erro: %d\n", nome, resultado);
        }
    } else {
        printf("Erro ao ler a entrada.\n");
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
    c.id = csv_obter_proximo_id("clientes.csv");
    
    // Copiar o nome de forma segura
    strncpy(c.nome, nome, MAX_NOME - 1);
    c.nome[MAX_NOME - 1] = '\0';
    
    // Inserir via módulo de persistência
    return csv_inserir_cliente(&c);
}
