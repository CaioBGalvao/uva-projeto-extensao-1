#ifndef MENU_H
#define MENU_H

// Enums para controle de menus (desejo de centralizar enums em arquivos separados)
typedef enum {
    OP_SAIR = 0,
    OP_CADASTRAR_CLIENTE = 1,
    OP_CADASTRAR_PRODUTO = 2,
    OP_REGISTRAR_PEDIDO = 3,
    OP_REGISTRAR_DEVOLUCAO = 4,
    OP_RELATORIO_DIARIO = 5,
    OP_RELATORIO_MENSAL = 6,
    OP_RELATORIO_ANUAL = 7
} OpcaoMenuPrincipal;

#endif
