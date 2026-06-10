#ifndef CLIENTE_H
#define CLIENTE_H

// Interface do módulo de Cliente (Dev 1)

// Responsável pela interação com o usuário via terminal
void cliente_menu_cadastrar(void);

// Regra de negócio testável: recebe dados, valida e salva usando persistencia.h
// Retorna 0 em caso de sucesso ou código de erro negativo.
int cliente_salvar(const char* nome);

#endif
