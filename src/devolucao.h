#ifndef DEVOLUCAO_H
#define DEVOLUCAO_H

// Interface do módulo de Devolução (Dev 4)

// Responsável pela interação com o usuário via terminal
void devolucao_menu_registrar(void);

// Regra de negócio testável: captura devolução, verifica se é 2a+ e cobra taxa
// Retorna 0 em caso de sucesso ou código de erro negativo.
int devolucao_salvar(int id_item_pedido, const char* data);

#endif
