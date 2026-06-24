/**
 * @file persistencia.h
 * @author Ana Silva (Dev 3)
 * @brief Interface do modulo de Persistencia.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "tipos.h"

// Retornos: 0 para sucesso, valor negativo em caso de erro.

// Inicialização
void persistencia_init(void);

// Utilitários de arquivo
int csv_obter_proximo_id(const char* arquivo);

// Entidade: Cliente
int csv_inserir_cliente(const Cliente* c);
int csv_buscar_cliente_por_id(int id, Cliente* resultado);
int csv_ler_todos_clientes(Cliente* resultados, int max_resultados, int* qtd_encontrada);

// Entidade: Produto
int csv_inserir_produto(const Produto* p);
int csv_buscar_produto_por_id(int id, Produto* resultado);
int csv_ler_todos_produtos(Produto* resultados, int max_resultados, int* qtd_encontrada);

// Entidade: Pedido e ItemPedido
int csv_inserir_pedido(const Pedido* p);
int csv_inserir_item_pedido(const ItemPedido* ip);
int csv_listar_itens_por_pedido(int id_pedido, ItemPedido* resultados, int max_resultados, int* qtd_encontrada);
int csv_ler_todos_pedidos(Pedido* resultados, int max_resultados, int* qtd_encontrada);
int csv_calcular_faturamento_diario(const char* data_iso, int* out_pedidos, float* out_faturado);
int csv_calcular_faturamento_mensal(const char* ano_mes_iso, int* out_pedidos, float* out_faturado);
int csv_calcular_faturamento_anual(const char* ano_iso, int* out_pedidos, float* out_faturado_total, float totais_mes[12]);

// Entidade: Devolução
int csv_inserir_devolucao(const Devolucao* d);
int csv_contar_devolucoes_item(int id_item_pedido);
int csv_calcular_taxas_devolucao_diario(const char* data_iso, float* out_taxas);
int csv_calcular_taxas_devolucao_mensal(const char* ano_mes_iso, float* out_taxas);
int csv_calcular_taxas_devolucao_anual(const char* ano_iso, float* out_taxas_total, float totais_mes[12]);

#endif
