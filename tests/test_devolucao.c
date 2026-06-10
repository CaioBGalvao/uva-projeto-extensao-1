#include <stdio.h>
#include <string.h>
#include "src/persistencia.h"
#include "test_utils.h"

int main() {
    remove("devolucoes.csv");

    ASSERT_EQ(0, csv_contar_devolucoes_item(5));

    Devolucao d1 = {1, 5, "2023-10-02", 1, 0.0f};
    ASSERT_EQ(0, csv_inserir_devolucao(&d1));

    Devolucao d2 = {2, 5, "2023-10-05", 2, 20.0f};
    ASSERT_EQ(0, csv_inserir_devolucao(&d2));

    Devolucao d3 = {3, 9, "2023-10-06", 1, 0.0f};
    ASSERT_EQ(0, csv_inserir_devolucao(&d3));

    ASSERT_EQ(2, csv_contar_devolucoes_item(5));
    ASSERT_EQ(1, csv_contar_devolucoes_item(9));
    ASSERT_EQ(0, csv_contar_devolucoes_item(1));

    remove("devolucoes.csv");
    printf("test_devolucao PASSED\n");
    return 0;
}
