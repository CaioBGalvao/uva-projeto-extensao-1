#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_factory.h"
#include "src/persistencia.h"

void factory_gerar_clientes(int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        Cliente c;
        c.id = csv_obter_proximo_id("database/clientes.csv");
        snprintf(c.nome, MAX_NOME, "Cliente Factory %d", i);
        csv_inserir_cliente(&c);
    }
}

void factory_gerar_produtos(int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        Produto p;
        p.id = csv_obter_proximo_id("database/produtos.csv");
        snprintf(p.nome, MAX_NOME, "Produto Factory %d", i);
        p.preco = (float)(rand() % 1000) / 10.0f + 1.0f;
        csv_inserir_produto(&p);
    }
}

void factory_gerar_pedidos(int quantidade_pedidos, int max_itens) {
    for (int i = 0; i < quantidade_pedidos; i++) {
        Pedido p;
        p.id = csv_obter_proximo_id("database/pedidos.csv");
        p.id_cliente = (rand() % 100) + 1;
        int dia = (rand() % 28) + 1;
        char temp_data[20];
        snprintf(temp_data, sizeof(temp_data), "2023-10-%02d", dia);
        strncpy(p.data, temp_data, TAM_DATA);
        csv_inserir_pedido(&p);
        
        int qtd_itens = (rand() % max_itens) + 1;
        for (int j = 0; j < qtd_itens; j++) {
            ItemPedido ip;
            ip.id = csv_obter_proximo_id("database/itens_pedido.csv");
            ip.id_pedido = p.id;
            ip.id_produto = (rand() % 100) + 1;
            ip.quantidade = (rand() % 5) + 1;
            ip.preco_unitario = 10.0f;
            ip.valor_total = ip.quantidade * ip.preco_unitario;
            
            csv_inserir_item_pedido(&ip);
        }
    }
}

void factory_corromper_meta(const char* arquivo_csv) {
    char arquivo_meta[256];
    const char* basename = strrchr(arquivo_csv, '/');
    if (basename) basename++;
    else basename = arquivo_csv;
    sprintf(arquivo_meta, "metadata/%s.meta", basename);
    
    // Deleting the file forces the persistence layer to read the raw .csv
    remove(arquivo_meta);
}
