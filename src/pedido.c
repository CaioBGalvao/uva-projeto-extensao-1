#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pedido.h"
#include "persistencia.h"
#include "tipos.h"
#include "cliente.h"
#include "produto.h"
#include "data_utils.h"
#include "input.h"
#include "logger.h"

// Interação com usuário para registrar um pedido
void pedido_menu_registrar(void) {
    char data[TAM_DATA] = {0};
    int id_cliente = 0;

    limpar_tela();
    printf("\n--- Registrar Pedido ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n");
    printf("(DICA: Pressione Ctrl+B para buscar clientes, produtos e pedidos)\n\n");

    // 1. Pergunta a data
    char data_iso[64];
    obter_data_atual(data_iso, 0);
    char data_br_padrao[64];
    converter_data_iso_para_br(data_iso, data_br_padrao, 0);

    char buffer_data[64];
    if (!ler_string_com_padrao("Data da compra (DD/MM/AAAA) [Enter para atual]: ", buffer_data, sizeof(buffer_data), data_br_padrao)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    converter_data_br_para_iso(buffer_data, data, 0);

    // 2. Pergunta ID do cliente e valida
    while (1) {
        if (!ler_inteiro("ID do cliente: ", &id_cliente)) {
            printf("\nOperacao cancelada.\n");
            return;
        }

        Cliente c = {0};
        if (csv_buscar_cliente_por_id(id_cliente, &c) == 0) {
            printf("Cliente encontrado: %s\n", c.nome);
            break;
        } else {
            char opc = 'N';
            if (!ler_char("Cliente nao encontrado. Deseja tentar novamente? (S/N): ", &opc)) {
                printf("\nOperacao cancelada.\n");
                return;
            }
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
        if (!ler_inteiro("ID do produto: ", &id_produto)) {
            printf("\nOperacao cancelada.\n");
            return;
        }

        Produto p = {0};
        if (csv_buscar_produto_por_id(id_produto, &p) != 0) {
            char opc = 'N';
            if (!ler_char("Produto nao encontrado. Deseja tentar outro produto? (S/N): ", &opc)) {
                printf("\nOperacao cancelada.\n");
                return;
            }
            if (opc == 'S' || opc == 's') continue;
            else break;
        }
        
        printf("Produto encontrado: %s | Preco: R$ %.2f\n", p.nome, p.preco);

        int qtd = 0;
        if (!ler_inteiro("Quantidade: ", &qtd)) {
            printf("\nOperacao cancelada.\n");
            return;
        }

        if (qtd <= 0) {
            printf("Quantidade deve ser maior que zero. Tente novamente.\n");
            continue;
        }

        ids_produtos[num_itens] = id_produto;
        quantidades[num_itens] = qtd;
        strncpy(nomes_produtos[num_itens], p.nome, MAX_NOME - 1);
        nomes_produtos[num_itens][MAX_NOME - 1] = '\0';
        precos_unitarios[num_itens] = p.preco;
        
        registrar_log("INTERMEDIARIO: Foi adicionado ao pedido o item %d ('%s') com valor unitario %.2f na quantidade %d.", id_produto, p.nome, p.preco, qtd);
        
        num_itens++;

        char opc = 'N';
        if (!ler_char("Deseja adicionar outro produto? (S/N): ", &opc)) {
            printf("\nOperacao cancelada.\n");
            return;
        }
        if (opc == 'S' || opc == 's') continue;
        else break;
    }

    if (num_itens == 0) {
        printf("Nenhum item adicionado. Abortando registro do pedido.\n");
        return;
    }

    // 4..6 Persistir pedido e itens
    int salvar_result = pedido_salvar(id_cliente, data, ids_produtos, quantidades, num_itens);
    if (salvar_result <= 0) {
        printf("Erro ao salvar pedido. Codigo: %d\n", salvar_result);
        return;
    }

    // 7. Imprimir recibo (reconstruimos valores locais para imprimir)
    char data_br[11];
    converter_data_iso_para_br(data, data_br, 0);
    float total = 0.0f;
    printf("\n====== Recibo de Venda ======\n");
    printf("Pedido ID: %d\n", salvar_result);
    printf("Data: %s\n", data_br);
    printf("Cliente ID: %d\n", id_cliente);
    Cliente c_recibo = {0};
    if (csv_buscar_cliente_por_id(id_cliente, &c_recibo) == 0) {
        printf("Nome do Cliente: %s\n", c_recibo.nome);
    }
    printf("----------------------------\n");
    for (int i = 0; i < num_itens; i++) {
        float line_total = quantidades[i] * precos_unitarios[i];
        total += line_total;
        printf("%s | Qtd: %d | Unit: %.2f | Total: %.2f\n", nomes_produtos[i], quantidades[i], precos_unitarios[i], line_total);
    }
    printf("----------------------------\n");
    printf("Valor total: %.2f\n", total);
    printf("============================\n");
    
    registrar_log("SUCESSO: Pedido ID %d fechado. Valor total do pedido: R$ %.2f.", salvar_result, total);
}

// Regra de negocio: persiste pedido e itens, garantindo ids encadeados
int pedido_salvar(int id_cliente, const char* data, const int* ids_produtos, const int* quantidades, int num_itens) {
    if (data == NULL || ids_produtos == NULL || quantidades == NULL || num_itens <= 0) return -1;

    Pedido p = {0};
    p.id = csv_obter_proximo_id("database/pedidos.csv");
    p.id_cliente = id_cliente;
    strncpy(p.data, data, TAM_DATA - 1);
    p.data[TAM_DATA - 1] = '\0';

    if (csv_inserir_pedido(&p) != 0) return -2;

    for (int i = 0; i < num_itens; i++) {
        ItemPedido ip = {0};
        ip.id = csv_obter_proximo_id("database/itens_pedido.csv");
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

    return p.id;
}
