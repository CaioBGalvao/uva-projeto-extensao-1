#!/bin/bash
set -e

echo "Compilando testes..."
gcc -I. tests/test_cliente.c src/persistencia.c -o tests/bin_test_cliente
gcc -I. tests/test_produto.c src/persistencia.c -o tests/bin_test_produto
gcc -I. tests/test_pedido.c src/persistencia.c -o tests/bin_test_pedido
gcc -I. tests/test_devolucao.c src/persistencia.c -o tests/bin_test_devolucao
gcc -I. tests/test_relatorio.c src/relatorio.c src/persistencia.c -o tests/bin_test_relatorio
gcc -I. tests/test_edge_cases.c tests/test_factory.c src/persistencia.c -o tests/bin_test_edge_cases
gcc -I. tests/test_performance.c tests/test_factory.c src/persistencia.c -o tests/bin_test_performance

echo "Rodando testes..."
./tests/bin_test_cliente
./tests/bin_test_produto
./tests/bin_test_pedido
./tests/bin_test_devolucao
./tests/bin_test_relatorio
./tests/bin_test_edge_cases
./tests/bin_test_performance

echo "Todos os testes passaram!"
