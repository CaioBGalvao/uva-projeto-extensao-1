/**
 * @file devolucao.h
 * @author Guilherme Brazil (Dev 4)
 * @brief Interface do modulo de Devolucao.
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#ifndef DEVOLUCAO_H
#define DEVOLUCAO_H

// Interface do módulo de Devolução (Dev 4)

#include "tipos.h"

// Responsável pela interação com o usuário via terminal
void devolucao_menu_registrar(void);

// Processa a devolução aplicando a regra da taxa de R$ 20.00
// Retornos:
// 0 = Sucesso
// -1 = Pedido não encontrado ou vazio
// -2 = Produto não pertence a este pedido
// -3 = Erro ao salvar no banco
int devolucao_processar(int id_pedido, int id_produto, const char* data, Devolucao* out_recibo);

#endif
