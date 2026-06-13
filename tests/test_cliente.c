#include <stdio.h>
#include <string.h>
#include "src/persistencia.h"
#include "test_utils.h"

int main() {
    remove("database/clientes.csv");
    remove("metadata/clientes.csv.meta");

    ASSERT_EQ(1, csv_obter_proximo_id("database/clientes.csv"));

    Cliente c = {1, "Maria"};
    ASSERT_EQ(0, csv_inserir_cliente(&c));
    
    ASSERT_EQ(2, csv_obter_proximo_id("database/clientes.csv"));

    Cliente r;
    ASSERT_EQ(0, csv_buscar_cliente_por_id(1, &r));
    ASSERT_EQ(1, r.id);
    ASSERT_STR_EQ("Maria", r.nome);

    ASSERT_EQ(-1, csv_buscar_cliente_por_id(99, &r));

    remove("database/clientes.csv");
    remove("metadata/clientes.csv.meta");
    printf("test_cliente PASSED\n");
    return 0;
}
