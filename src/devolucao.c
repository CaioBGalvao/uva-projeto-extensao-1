/**
 * @file devolucao.c
 * @author Guilherme Brazil (Dev 4)
 * @brief Implementacao do modulo de Devolucao e calculo de taxas.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#include <stdio.h>
#include <string.h>
#include "devolucao.h"
#include "persistencia.h"
#include "data_utils.h"
#include "input.h"
#include "logger.h"

/**
 * @brief Executa a operacao de devolucao_menu_registrar.
 *
 */
void devolucao_menu_registrar(void) {
    char data[TAM_DATA];
    int id_pedido;

    limpar_tela();
    printf("\n--- MODULO DE DEVOLUCAO ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n");
    printf("(Use '?' para Busca Rapida a qualquer momento)\n\n");
    char data_iso[64];
    obter_data_atual(data_iso, 0);
    char data_br_padrao[64];
    converter_data_iso_para_br(data_iso, data_br_padrao, 0);

    char buffer[64];
    if (!ler_string_com_padrao("Data (DD/MM/AAAA) [Enter para hoje]: ", buffer, sizeof(buffer), data_br_padrao)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    converter_data_br_para_iso(buffer, data, 0);

    if (!ler_inteiro("ID do Pedido: ", &id_pedido)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    // Busca os itens do pedido
    ItemPedido itens[MAX_ITENS_PEDIDO];
    int total = 0;
    if (csv_listar_itens_por_pedido(id_pedido, itens, MAX_ITENS_PEDIDO, &total) != 0 || total == 0) {
        printf("Pedido %d nao encontrado ou sem itens.\n", id_pedido);
        return;
    }

    // Exibe os itens encontrados
    printf("\nItens do Pedido %d:\n", id_pedido);
    printf("ID Produto   Quantidade\n");

    for (int i = 0; i < total; i++) {
        printf("%d            %d\n", itens[i].id_produto, itens[i].quantidade);
    }

    // Usuário escolhe o item a devolver
    int id_produto;
    if (!ler_inteiro("\nID do Produto a devolver: ", &id_produto)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    Devolucao recibo;
    int resultado = devolucao_processar(id_pedido, id_produto, data, &recibo);

    if (resultado == -1) {
        printf("Pedido %d nao encontrado ou vazio.\n", id_pedido);
    } else if (resultado == -2) {
        printf("Produto %d nao pertence ao pedido %d.\n", id_produto, id_pedido);
    } else if (resultado == -3) {
        printf("Erro ao salvar devolucao no banco de dados.\n");
    } else if (resultado == 0) {
        char data_br[11];
        converter_data_iso_para_br(recibo.data, data_br, 0);
        // Imprime o comprovante
        printf("\nCOMPROVANTE DE DEVOLUCAO\n");
        printf("ID Devolucao : %d\n",    recibo.id);
        printf("ID Pedido    : %d\n",    id_pedido);
        printf("ID Produto   : %d\n",    id_produto);
        printf("Data         : %s\n",    data_br);
        printf("Contagem     : %d\n",    recibo.contagem);
        printf("Taxa         : R$ %.2f\n", recibo.taxa_cobrada);

        if (recibo.taxa_cobrada > 0.00f) {
            printf("\nAVISO: Taxa de R$ 20,00 ativada. Debitar do cliente.\n");
        }
        registrar_log("SUCESSO: Devolucao registrada para o pedido %d (item %d). Taxa R$ %.2f cobrada.", id_pedido, id_produto, recibo.taxa_cobrada);
    } else {
        registrar_log("ERRO: Falha ao tentar registrar devolucao (Pedido %d, Produto %d). Codigo: %d", id_pedido, id_produto, resultado);
    }
}

/**
 * @brief Executa a operacao de devolucao_processar.
 *
 * @param id_pedido Parametro de entrada.
 * @param id_produto Parametro de entrada.
 * @param data Parametro de entrada.
 * @param out_recibo Parametro de entrada.
 * @return int Retorno da operacao.
 */
int devolucao_processar(int id_pedido, int id_produto, const char* data, Devolucao* out_recibo) {
    if (id_pedido <= 0 || id_produto <= 0 || data == NULL || out_recibo == NULL) return -1;

    ItemPedido itens[MAX_ITENS_PEDIDO];
    int total = 0;
    if (csv_listar_itens_por_pedido(id_pedido, itens, MAX_ITENS_PEDIDO, &total) != 0 || total == 0) {
        return -1; // Pedido não encontrado ou sem itens
    }

    int id_item_pedido = -1;
    for (int i = 0; i < total; i++) {
        if (itens[i].id_produto == id_produto) {
            id_item_pedido = itens[i].id;
            break;
        }
    }

    if (id_item_pedido == -1) {
        return -2; // Produto não pertence a este pedido
    }

    int historico = csv_contar_devolucoes_item(id_item_pedido);
    int contagem = historico + 1;
    float taxa = (historico > 0) ? 20.00f : 0.00f;

    Devolucao d;
    d.id = csv_obter_proximo_id("database/devolucoes.csv");
    d.id_item_pedido = id_item_pedido;
    strncpy(d.data, data, TAM_DATA - 1);
    d.data[TAM_DATA - 1] = '\0';
    d.contagem = contagem;
    d.taxa_cobrada = taxa;

    if (csv_inserir_devolucao(&d) != 0) {
        return -3; // Erro ao salvar
    }

    *out_recibo = d;
    return 0;
}
