#include <stdio.h>
#include <time.h>
#include "test_utils.h"
#include "src/persistencia.h"
#include "tests/test_factory.h"

int main() {
    printf("\n--- Testes de Performance ---\n");

    // Limpar arquivos para ter uma base limpa
    remove("database/clientes.csv");
    remove("metadata/clientes.csv.meta");

    clock_t start, end;
    double cpu_time_used;

    // Teste de Escrita
    printf("Testando escrita de 5.000 clientes... ");
    start = clock();
    factory_gerar_clientes(5000);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Concluido em %f segundos.\n", cpu_time_used);

    // Teste de Leitura
    printf("Testando leitura de 500 clientes aleatorios... ");
    start = clock();
    Cliente c;
    for (int i = 0; i < 500; i++) {
        int target_id = (rand() % 5000) + 1;
        csv_buscar_cliente_por_id(target_id, &c);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Concluido em %f segundos.\n", cpu_time_used);

    // Limpeza
    remove("database/clientes.csv");
    remove("metadata/clientes.csv.meta");

    printf("test_performance PASSED\n");
    return 0;
}
