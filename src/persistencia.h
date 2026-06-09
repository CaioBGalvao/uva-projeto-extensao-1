#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "tipos.h"

// Retornos: 0 para sucesso, valor negativo em caso de erro.

// Utilitários de arquivo
int csv_obter_proximo_id(const char* arquivo);

// Entidade: Cliente
int csv_inserir_cliente(const Cliente* c);
int csv_buscar_cliente_por_id(int id, Cliente* resultado);

// Entidade: Produto
int csv_inserir_produto(const Produto* p);
int csv_buscar_produto_por_id(int id, Produto* resultado);

// Entidade: Pedido e ItemPedido
int csv_inserir_pedido(const Pedido* p);
int csv_inserir_item_pedido(const ItemPedido* ip);
int csv_listar_itens_por_pedido(int id_pedido, ItemPedido* resultados, int max_resultados, int* qtd_encontrada);
int csv_ler_todos_pedidos(Pedido* resultados, int max_resultados, int* qtd_encontrada);

// Entidade: Devolução
int csv_inserir_devolucao(const Devolucao* d);
int csv_contar_devolucoes_item(int id_item_pedido);

#endif
