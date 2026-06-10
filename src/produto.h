#ifndef PRODUTO_H
#define PRODUTO_H

// Interface do módulo de Produto (Dev 2)

// Responsável pela interação com o usuário via terminal
void produto_menu_cadastrar(void);

// Regra de negócio testável: recebe dados, valida e salva usando persistencia.h
// Retorna 0 em caso de sucesso ou código de erro negativo.
int produto_salvar(const char* nome, float preco);

#endif
