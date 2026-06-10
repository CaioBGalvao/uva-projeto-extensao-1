#ifndef PEDIDO_H
#define PEDIDO_H

// Interface do módulo de Pedido (Dev 3)

// Responsável pela interação com o usuário via terminal
void pedido_menu_registrar(void);

// Regra de negócio testável: monta o pedido associando cliente a itens e persiste.
// Retorna 0 em caso de sucesso ou código de erro negativo.
int pedido_salvar(int id_cliente, const char* data, const int* ids_produtos, const int* quantidades, int num_itens);

#endif
