#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pedido.h"
#include "persistencia.h"
#include "tipos.h"
#include "cliente.h"
#include "produto.h"

// Interação com usuário para registrar um pedido
void pedido_menu_registrar(void) {
    char data[TAM_DATA] = {0};
    int id_cliente = 0;

    printf("\n--- Registrar Pedido ---\n");

    // 1. Pergunta a data
    printf("Data da compra (AAAA-MM-DD): ");
    if (fgets(data, sizeof(data), stdin) == NULL) {
        printf("Erro ao ler a data. Abortando.\n");
        return;
    }
    size_t len = strlen(data);
    if (len > 0 && data[len - 1] == '\n') data[len - 1] = '\0';

    // 2. Pergunta ID do cliente e valida
    while (1) {
        printf("ID do cliente: ");
        if (scanf("%d", &id_cliente) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        Cliente c = {0};
        if (csv_buscar_cliente_por_id(id_cliente, &c) == 0) {
            printf("Cliente encontrado: %s\n", c.nome);
            break;
        } else {
            char opc = 'N';
            printf("Cliente nao encontrado. Deseja tentar novamente? (S/N): ");
            if (scanf(" %c", &opc) != 1) {
                printf("Entrada invalida. Abortando.\n");
                return;
            }
            while (getchar() != '\n');
            if (opc == 'S' || opc == 's') continue;
            else return;
        }
    }

    // 3. Carrinho em memória
    int ids_produtos[MAX_ITENS_PEDIDO];
    int quantidades[MAX_ITENS_PEDIDO];
    char nomes_produtos[MAX_ITENS_PEDIDO][MAX_NOME];
    float precos_unitarios[MAX_ITENS_PEDIDO];
    int num_itens = 0;

    while (1) {
        if (num_itens >= MAX_ITENS_PEDIDO) {
            printf("Numero maximo de itens atingido.\n");
            break;
        }

        int id_produto = 0;
        printf("ID do produto: ");
        if (scanf("%d", &id_produto) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        Produto p = {0};
        if (csv_buscar_produto_por_id(id_produto, &p) != 0) {
            char opc = 'N';
            printf("Produto nao encontrado. Deseja tentar outro produto? (S/N): ");
            if (scanf(" %c", &opc) != 1) return;
            while (getchar() != '\n');
            if (opc == 'S' || opc == 's') continue;
            else break;
        }

        int qtd = 0;
        printf("Quantidade: ");
        if (scanf("%d", &qtd) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (qtd <= 0) {
            printf("Quantidade deve ser maior que zero. Tente novamente.\n");
            continue;
        }

        ids_produtos[num_itens] = id_produto;
        quantidades[num_itens] = qtd;
        strncpy(nomes_produtos[num_itens], p.nome, MAX_NOME - 1);
        nomes_produtos[num_itens][MAX_NOME - 1] = '\0';
        precos_unitarios[num_itens] = p.preco;
        num_itens++;

        char opc = 'N';
        printf("Deseja adicionar outro produto? (S/N): ");
        if (scanf(" %c", &opc) != 1) return;
        while (getchar() != '\n');
        if (opc == 'S' || opc == 's') continue;
        else break;
    }

    if (num_itens == 0) {
        printf("Nenhum item adicionado. Abortando registro do pedido.\n");
        return;
    }

    // 4..6 Persistir pedido e itens
    int salvar_result = pedido_salvar(id_cliente, data, ids_produtos, quantidades, num_itens);
    if (salvar_result != 0) {
        printf("Erro ao salvar pedido. Codigo: %d\n", salvar_result);
        return;
    }

    // 7. Imprimir recibo (reconstruimos valores locais para imprimir)
    float total = 0.0f;
    printf("\n====== Recibo de Venda ======\n");
    printf("Data: %s\n", data);
    printf("Cliente ID: %d\n", id_cliente);
    printf("----------------------------\n");
    for (int i = 0; i < num_itens; i++) {
        float line_total = quantidades[i] * precos_unitarios[i];
        total += line_total;
        printf("%s | Qtd: %d | Unit: %.2f | Total: %.2f\n", nomes_produtos[i], quantidades[i], precos_unitarios[i], line_total);
    }
    printf("----------------------------\n");
    printf("Valor total: %.2f\n", total);
    printf("============================\n");
}

// Regra de negocio: persiste pedido e itens, garantindo ids encadeados
int pedido_salvar(int id_cliente, const char* data, const int* ids_produtos, const int* quantidades, int num_itens) {
    if (data == NULL || ids_produtos == NULL || quantidades == NULL || num_itens <= 0) return -1;

    Pedido p = {0};
    p.id = csv_obter_proximo_id("pedidos.csv");
    p.id_cliente = id_cliente;
    strncpy(p.data, data, TAM_DATA - 1);
    p.data[TAM_DATA - 1] = '\0';

    if (csv_inserir_pedido(&p) != 0) return -2;

    for (int i = 0; i < num_itens; i++) {
        ItemPedido ip = {0};
        ip.id = csv_obter_proximo_id("itens_pedido.csv");
        ip.id_pedido = p.id;
        ip.id_produto = ids_produtos[i];
        ip.quantidade = quantidades[i];

        Produto prod = {0};
        if (csv_buscar_produto_por_id(ip.id_produto, &prod) != 0) {
            // produto desapareceu entre validacao e salvamento
            return -3;
        }
        ip.preco_unitario = prod.preco;
        ip.valor_total = ip.preco_unitario * ip.quantidade;

        if (csv_inserir_item_pedido(&ip) != 0) return -4;
    }

    return 0;
}
