#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "cliente.h"
#include "produto.h"
#include "pedido.h"
#include "devolucao.h"
#include "relatorio.h"
#include "input.h"
#include "logger.h"

// Esqueleto do arquivo principal (Caio)
// Permite que os 5 desenvolvedores acoplem suas implementações diretamente no switch/case.

void exibir_menu() {
    printf("\n====================================\n");
    printf("        SISTEMA DE PEDIDOS          \n");
    printf("====================================\n");
    printf("%d. Cadastrar Cliente\n", OP_CADASTRAR_CLIENTE);
    printf("%d. Cadastrar Produto\n", OP_CADASTRAR_PRODUTO);
    printf("%d. Registrar Pedido\n", OP_REGISTRAR_PEDIDO);
    printf("%d. Registrar Devolucao\n", OP_REGISTRAR_DEVOLUCAO);
    printf("%d. Relatorio Diario\n", OP_RELATORIO_DIARIO);
    printf("%d. Relatorio Mensal\n", OP_RELATORIO_MENSAL);
    printf("%d. Relatorio Anual\n", OP_RELATORIO_ANUAL);
    printf("%d. Sair\n", OP_SAIR);
    printf("====================================\n");
}

int main() {
    int opcao_escolhida;
    
    registrar_log("SISTEMA: Programa iniciado.");
    input_configurar_atalhos();
    
    do {
        limpar_tela();
        exibir_menu();
        if (!ler_inteiro("Escolha uma opcão: ", &opcao_escolhida)) {
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        registrar_log("MENU: Usuario escolheu a opcao %d no menu principal.", opcao_escolhida);

        switch ((OpcaoMenuPrincipal)opcao_escolhida) {
            case OP_CADASTRAR_CLIENTE:
                cliente_menu_cadastrar();
                break;
            case OP_CADASTRAR_PRODUTO:
                produto_menu_cadastrar();
                break;
            case OP_REGISTRAR_PEDIDO:
                pedido_menu_registrar();
                break;
            case OP_REGISTRAR_DEVOLUCAO:
                devolucao_menu_registrar();
                break;
            case OP_RELATORIO_DIARIO:
                relatorio_diario();
                break;
            case OP_RELATORIO_MENSAL:
                relatorio_mensal();
                break;
            case OP_RELATORIO_ANUAL:
                relatorio_anual();
                break;
            case OP_SAIR:
                registrar_log("SISTEMA: Encerrando o sistema.");
                printf("Encerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida. Digite um numero de 0 a 7.\n");
                break;
        }
        
        // Pausa a tela após cada ação (ou ao sair) para que o resultado possa ser lido
        // no terminal do Windows antes que a janela seja fechada ou o menu reexibido.
        printf("\nPressione ENTER para continuar...");
        getchar();

    } while (opcao_escolhida != OP_SAIR);

    return 0;
}
