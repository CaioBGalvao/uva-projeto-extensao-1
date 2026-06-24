/**
 * @file tipos.h
 * @author Ana Silva (Dev 3)
 * @brief Definicao das estruturas de dados globais.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#ifndef TIPOS_H
#define TIPOS_H

#define MAX_PEDIDOS_DIA 9999
#define MAX_ITENS_PEDIDO 9999
#define MAX_QTD_ITEM 9999
#define MAX_NOME 255
#define TAM_DATA 11 // Formato: AAAA-MM-DD + \0

typedef struct {
    int id;
    char nome[MAX_NOME];
} Cliente;

typedef struct {
    int id;
    char nome[MAX_NOME];
    float preco;
} Produto;

typedef struct {
    int id;
    int id_pedido;
    int id_produto;
    int quantidade;
    float preco_unitario;
    float valor_total;
} ItemPedido;

typedef struct {
    int id;
    int id_cliente;
    char data[TAM_DATA];
} Pedido;

typedef struct {
    Pedido dados;
    ItemPedido itens[MAX_ITENS_PEDIDO];
    int qtd_itens;
} PedidoCompleto;

typedef struct {
    int id;
    int id_item_pedido;
    char data[TAM_DATA];
    int contagem;
    float taxa_cobrada;
} Devolucao;

#endif
