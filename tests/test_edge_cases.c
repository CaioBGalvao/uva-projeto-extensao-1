#include <stdio.h>
#include "test_utils.h"
#include "src/persistencia.h"
#include "tests/test_factory.h"

void test_skipped_ids() {
    remove("database/clientes.csv");
    remove("metadata/clientes.csv.meta");

    Cliente c1 = {1, "Cliente A"};
    csv_inserir_cliente(&c1);
    
    Cliente c2 = {5, "Cliente B"};
    csv_inserir_cliente(&c2);
    
    Cliente c3 = {10, "Cliente C"};
    csv_inserir_cliente(&c3);

    ASSERT_EQ(11, csv_obter_proximo_id("database/clientes.csv"));
}

void test_metadata_corruption() {
    remove("database/produtos.csv");
    remove("metadata/produtos.csv.meta");

    Produto p1 = {1, "Prod A", 10.0f};
    csv_inserir_produto(&p1);
    Produto p2 = {2, "Prod B", 20.0f};
    csv_inserir_produto(&p2);

    ASSERT_EQ(3, csv_obter_proximo_id("database/produtos.csv"));

    // Corromper metadata
    factory_corromper_meta("database/produtos.csv");

    // Mesmo com arquivo meta corrompido, a lógica deve escanear o CSV e descobrir que o prox é 3
    ASSERT_EQ(3, csv_obter_proximo_id("database/produtos.csv"));
}

void test_direct_file_manipulation() {
    remove("database/produtos.csv");
    remove("metadata/produtos.csv.meta");

    Produto p1 = {1, "Prod A", 10.0f};
    csv_inserir_produto(&p1);

    // Manipulando o arquivo diretamente via append com linha malformada
    FILE* f = fopen("database/produtos.csv", "a");
    fprintf(f, "linha malformada que nao eh um produto\n");
    fprintf(f, "50;Produto X;99.99\n"); // Produto valido manipulado direto
    fclose(f);

    // Corromper meta para forçar leitura do CSV manipulado
    factory_corromper_meta("database/produtos.csv");

    ASSERT_EQ(51, csv_obter_proximo_id("database/produtos.csv"));

    Produto r;
    // O produto id 50 deve ser achado e linha malformada ignorada
    ASSERT_EQ(0, csv_buscar_produto_por_id(50, &r));
    ASSERT_STR_EQ("Produto X", r.nome);
}

int main() {
    test_skipped_ids();
    test_metadata_corruption();
    test_direct_file_manipulation();

    printf("test_edge_cases PASSED\n");
    return 0;
}
