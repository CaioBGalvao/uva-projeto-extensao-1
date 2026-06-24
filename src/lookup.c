#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lookup.h"
#include "persistencia.h"
#include "tipos.h"
#include "input.h"
#include "data_utils.h"

#define PAGE_SIZE 50
#define PAGE_SIZE_PEDIDOS 10

static void mostrar_pagina_clientes(Cliente* clientes, int total, int page) {
    limpar_tela();
    int start = page * PAGE_SIZE;
    int end = start + PAGE_SIZE;
    if (end > total) end = total;

    printf("\n--- Busca de Clientes (Pagina %d/%d) ---\n", page + 1, (total + PAGE_SIZE - 1) / PAGE_SIZE == 0 ? 1 : (total + PAGE_SIZE - 1) / PAGE_SIZE);
    if (total == 0) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        printf("%-5s | %-30s\n", "ID", "Nome");
        printf("----------------------------------------\n");
        for (int i = start; i < end; i++) {
            printf("%-5d | %s\n", clientes[i].id, clientes[i].nome);
        }
    }
    printf("----------------------------------------\n");
}

static void mostrar_pagina_produtos(Produto* produtos, int total, int page) {
    limpar_tela();
    int start = page * PAGE_SIZE;
    int end = start + PAGE_SIZE;
    if (end > total) end = total;

    printf("\n--- Busca de Produtos (Pagina %d/%d) ---\n", page + 1, (total + PAGE_SIZE - 1) / PAGE_SIZE == 0 ? 1 : (total + PAGE_SIZE - 1) / PAGE_SIZE);
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
    } else {
        printf("%-5s | %-30s | %-10s\n", "ID", "Nome", "Preco");
        printf("----------------------------------------------------\n");
        for (int i = start; i < end; i++) {
            printf("%-5d | %-30s | %.2f\n", produtos[i].id, produtos[i].nome, produtos[i].preco);
        }
    }
    printf("----------------------------------------------------\n");
}

static void mostrar_pagina_pedidos(Pedido* pedidos, int total, int page) {
    limpar_tela();
    int start = page * PAGE_SIZE_PEDIDOS;
    int end = start + PAGE_SIZE_PEDIDOS;
    if (end > total) end = total;

    printf("\n--- Busca de Pedidos (Pagina %d/%d) ---\n", page + 1, (total + PAGE_SIZE_PEDIDOS - 1) / PAGE_SIZE_PEDIDOS == 0 ? 1 : (total + PAGE_SIZE_PEDIDOS - 1) / PAGE_SIZE_PEDIDOS);
    if (total == 0) {
        printf("Nenhum pedido cadastrado.\n");
    } else {
        for (int i = start; i < end; i++) {
            char data_br[11];
            converter_data_iso_para_br(pedidos[i].data, data_br, 0);
            printf("\n====== Recibo de Venda ======\n");
            printf("Pedido ID: %d\n", pedidos[i].id);
            printf("Data: %s\n", data_br);
            printf("Cliente ID: %d\n", pedidos[i].id_cliente);
            Cliente c_recibo = {0};
            if (csv_buscar_cliente_por_id(pedidos[i].id_cliente, &c_recibo) == 0) {
                printf("Nome do Cliente: %s\n", c_recibo.nome);
            }
            printf("----------------------------\n");

            ItemPedido itens[MAX_ITENS_PEDIDO];
            int total_itens = 0;
            csv_listar_itens_por_pedido(pedidos[i].id, itens, MAX_ITENS_PEDIDO, &total_itens);

            float total_pedido = 0;
            for (int j = 0; j < total_itens; j++) {
                Produto p = {0};
                csv_buscar_produto_por_id(itens[j].id_produto, &p);
                total_pedido += itens[j].valor_total;
                printf("%s | Qtd: %d | Unit: %.2f | Total: %.2f\n", p.nome, itens[j].quantidade, itens[j].preco_unitario, itens[j].valor_total);
            }
            printf("----------------------------\n");
            printf("Valor total: %.2f\n", total_pedido);
            printf("============================\n");
        }
    }
}

static void loop_clientes(void) {
    Cliente clientes[1000];
    int total = 0;
    csv_ler_todos_clientes(clientes, 1000, &total);

    int page = 0;
    int max_page = (total + PAGE_SIZE - 1) / PAGE_SIZE;
    if (max_page == 0) max_page = 1;

    char cmd[32];
    while (1) {
        mostrar_pagina_clientes(clientes, total, page);
        printf("\nDigite: 'prox' (proxima), 'ant' (anterior), 'sair': ");
        if (scanf("%31s", cmd) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF); // Flush newline
        if (strcmp(cmd, "sair") == 0) break;
        if (strcmp(cmd, "prox") == 0 && page < max_page - 1) page++;
        if (strcmp(cmd, "ant") == 0 && page > 0) page--;
    }
}

static void loop_produtos(void) {
    Produto produtos[1000];
    int total = 0;
    csv_ler_todos_produtos(produtos, 1000, &total);

    int page = 0;
    int max_page = (total + PAGE_SIZE - 1) / PAGE_SIZE;
    if (max_page == 0) max_page = 1;

    char cmd[32];
    while (1) {
        mostrar_pagina_produtos(produtos, total, page);
        printf("\nDigite: 'prox' (proxima), 'ant' (anterior), 'sair': ");
        if (scanf("%31s", cmd) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF); // Flush newline
        if (strcmp(cmd, "sair") == 0) break;
        if (strcmp(cmd, "prox") == 0 && page < max_page - 1) page++;
        if (strcmp(cmd, "ant") == 0 && page > 0) page--;
    }
}

static void loop_pedidos(void) {
    Pedido pedidos[1000];
    int total = 0;
    csv_ler_todos_pedidos(pedidos, 1000, &total);

    int page = 0;
    int max_page = (total + PAGE_SIZE_PEDIDOS - 1) / PAGE_SIZE_PEDIDOS;
    if (max_page == 0) max_page = 1;

    char cmd[32];
    while (1) {
        mostrar_pagina_pedidos(pedidos, total, page);
        printf("\nDigite: 'prox' (proxima), 'ant' (anterior), 'sair': ");
        if (scanf("%31s", cmd) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF); // Flush newline
        if (strcmp(cmd, "sair") == 0) break;
        if (strcmp(cmd, "prox") == 0 && page < max_page - 1) page++;
        if (strcmp(cmd, "ant") == 0 && page > 0) page--;
    }
}

void lookup_menu(void) {
    char cmd[32];
    while (1) {
        limpar_tela();
        printf("\n--- MENU DE BUSCA RAPIDA ---\n");
        printf("1. Clientes\n");
        printf("2. Produtos\n");
        printf("3. Pedidos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcão: ");

        if (scanf("%31s", cmd) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF); // Flush newline
        if (strcmp(cmd, "0") == 0 || strcmp(cmd, "sair") == 0) break;

        if (strcmp(cmd, "1") == 0) {
            loop_clientes();
        } else if (strcmp(cmd, "2") == 0) {
            loop_produtos();
        } else if (strcmp(cmd, "3") == 0) {
            loop_pedidos();
        }
    }
}
