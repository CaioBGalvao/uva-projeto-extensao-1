#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "cliente.h"
#include "produto.h"
#include "pedido.h"
#include "devolucao.h"
#include "relatorio.h"

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
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao_escolhida;
    
    do {
        exibir_menu();
        if (scanf("%d", &opcao_escolhida) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            // Limpa buffer do stdin em caso de entrada não-numérica
            while (getchar() != '\n');
            continue;
        }
        
        // Limpa o buffer do '\n' residual sempre, para que pausas futuras funcionem perfeitamente.
        while (getchar() != '\n');

        switch ((OpcaoMenuPrincipal)opcao_escolhida) {
            case OP_CADASTRAR_CLIENTE:
                cliente_cadastrar();
                break;
            case OP_CADASTRAR_PRODUTO:
                produto_cadastrar();
                break;
            case OP_REGISTRAR_PEDIDO:
                pedido_registrar();
                break;
            case OP_REGISTRAR_DEVOLUCAO:
                devolucao_registrar();
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
