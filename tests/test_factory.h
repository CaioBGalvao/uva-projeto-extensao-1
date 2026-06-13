#ifndef TEST_FACTORY_H
#define TEST_FACTORY_H

#include "src/tipos.h"

void factory_gerar_clientes(int quantidade);
void factory_gerar_produtos(int quantidade);
void factory_gerar_pedidos(int quantidade_pedidos, int max_itens);
void factory_corromper_meta(const char* arquivo_csv);

#endif // TEST_FACTORY_H
