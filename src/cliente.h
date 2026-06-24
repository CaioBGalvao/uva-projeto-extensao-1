/**
 * @file cliente.h
 * @author Gabriel Bittencourt (Dev 1)
 * @brief Interface do módulo de Cliente.
 * 
 * Responsável pelas declarações de funções para interação com clientes e
 * persistência de seus dados.
 */

#ifndef CLIENTE_H
#define CLIENTE_H

/**
 * @brief Menu interativo para cadastro de cliente.
 * 
 * Exibe o formulário no terminal, coleta o nome do cliente validando a entrada
 * e aciona a regra de negócio para persistência.
 */
void cliente_menu_cadastrar(void);

/**
 * @brief Salva os dados de um novo cliente no sistema.
 * 
 * @param nome Nome do cliente a ser salvo.
 * @return int 0 em caso de sucesso, -1 se o nome for inválido ou houver erro no CSV.
 */
int cliente_salvar(const char* nome);

#endif
