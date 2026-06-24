/**
 * @file main.c
 * @author Caio Galvao (Dev 5)
 * @brief Ponto de entrada do sistema e integracao de menus.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

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
#include "persistencia.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void configurar_console_windows() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#endif

// Esqueleto do arquivo principal (Caio)
// Permite que os 5 desenvolvedores acoplem suas implementações diretamente no switch/case.

/**
 * @brief Executa a operacao de exibir_menu.
 *
 */
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

/**
 * @brief Executa a operacao de main.
 *
 * @return int Retorno da operacao.
 */
int main() {
    int opcao_escolhida;
    
#ifdef _WIN32
    configurar_console_windows();
#endif

    persistencia_init();
    registrar_log("SISTEMA: Programa iniciado.");
    input_configurar_atalhos();
    
    do {
        limpar_tela();
        exibir_menu();
        if (!ler_inteiro("Escolha uma opcao: ", &opcao_escolhida)) {
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
