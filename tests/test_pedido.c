#include <stdio.h>
#include <string.h>
#include "src/persistencia.h"
#include "test_utils.h"

int main() {
    remove("pedidos.csv");
    remove("itens_pedido.csv");

    Pedido p = {1, 10, "2023-10-01"};
    ASSERT_EQ(0, csv_inserir_pedido(&p));

    ItemPedido ip1 = {1, 1, 100, 2, 50.0f, 100.0f};
    ItemPedido ip2 = {2, 1, 101, 1, 25.0f, 25.0f};
    ASSERT_EQ(0, csv_inserir_item_pedido(&ip1));
    ASSERT_EQ(0, csv_inserir_item_pedido(&ip2));

    ItemPedido res[10];
    int qtd = 0;
    ASSERT_EQ(0, csv_listar_itens_por_pedido(1, res, 10, &qtd));
    ASSERT_EQ(2, qtd);
    ASSERT_EQ(1, res[0].id);
    ASSERT_EQ(2, res[1].id);

    Pedido press[10];
    int qtd_pedidos = 0;
    ASSERT_EQ(0, csv_ler_todos_pedidos(press, 10, &qtd_pedidos));
    ASSERT_EQ(1, qtd_pedidos);
    ASSERT_EQ(1, press[0].id);
    ASSERT_STR_EQ("2023-10-01", press[0].data);

    remove("pedidos.csv");
    remove("itens_pedido.csv");
    printf("test_pedido PASSED\n");
    return 0;
}
