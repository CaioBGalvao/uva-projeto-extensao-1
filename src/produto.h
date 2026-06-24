/**
 * @file produto.h
 * @author Wallace Calisto (Dev 2)
 * @brief Interface do módulo de Produto.
 * 
 * Declarações de funções para gerenciamento de produtos, cadastro via
 * terminal e regras de negócio.
 */

#ifndef PRODUTO_H
#define PRODUTO_H

/**
 * @brief Menu interativo para cadastro de produto.
 * 
 * Apresenta a interface no terminal para que o usuário informe o nome 
 * e preço de um novo produto e o armazena no sistema.
 */
void produto_menu_cadastrar(void);

/**
 * @brief Registra um novo produto na persistência.
 * 
 * @param nome Nome do produto.
 * @param preco Preço unitário do produto.
 * @return int 0 em caso de sucesso, ou -1 em caso de falha de validação ou persistência.
 */
int produto_salvar(const char* nome, float preco);

#endif
