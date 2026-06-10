#include <stdio.h>
#include <string.h>
#include "src/persistencia.h"
#include "test_utils.h"

int main() {
    remove("produtos.csv");

    ASSERT_EQ(1, csv_obter_proximo_id("produtos.csv"));

    Produto p = {1, "Mouse", 45.5f};
    ASSERT_EQ(0, csv_inserir_produto(&p));

    Produto r;
    ASSERT_EQ(0, csv_buscar_produto_por_id(1, &r));
    ASSERT_EQ(1, r.id);
    ASSERT_STR_EQ("Mouse", r.nome);
    ASSERT_EQ((int)(45.5f * 100), (int)(r.preco * 100));

    remove("produtos.csv");
    printf("test_produto PASSED\n");
    return 0;
}
