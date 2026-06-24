#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/relatorio.h"
#include "src/persistencia.h"
#include "test_utils.h"

void criar_massa_dados() {
    remove("database/pedidos.csv");
    remove("metadata/pedidos.csv.meta");
    remove("database/itens_pedido.csv");
    remove("metadata/itens_pedido.csv.meta");

    // Pedido 1: 2023-10-01 (100.0)
    Pedido p1 = {1, 10, "2023-10-01"};
    csv_inserir_pedido(&p1);
    ItemPedido ip1 = {1, 1, 100, 2, 50.0f, 100.0f};
    csv_inserir_item_pedido(&ip1);

    // Pedido 2: 2023-10-01 (50.0)
    Pedido p2 = {2, 11, "2023-10-01"};
    csv_inserir_pedido(&p2);
    ItemPedido ip2 = {2, 2, 101, 1, 50.0f, 50.0f};
    csv_inserir_item_pedido(&ip2);

    // Pedido 3: 2023-10-02 (200.0) - Dia diferente
    Pedido p3 = {3, 10, "2023-10-02"};
    csv_inserir_pedido(&p3);
    ItemPedido ip3 = {3, 3, 100, 4, 50.0f, 200.0f};
    csv_inserir_item_pedido(&ip3);
}

void test_relatorio_diario() {
    criar_massa_dados();

    // Simula entrada do usuário (data 01/10/2023)
    FILE* in = fopen("input_mock.txt", "w");
    fprintf(in, "01/10/2023\n");
    fclose(in);

    freopen("input_mock.txt", "r", stdin);
    
    // Captura stdout
    freopen("output_mock.txt", "w", stdout);

    relatorio_diario();

    // Restaura stdout pro terminal (gambiarra de POSIX, mas para teste simples ok)
    freopen("/dev/tty", "w", stdout);

    // Lê output_mock.txt e verifica se o total faturado é 150.00
    FILE* out = fopen("output_mock.txt", "r");
    char buffer[1024];
    int encontrou_total = 0;
    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "150.00") != NULL) {
            encontrou_total = 1;
            break;
        }
    }
    fclose(out);

    ASSERT_EQ(1, encontrou_total);

    remove("database/pedidos.csv");
    remove("metadata/pedidos.csv.meta");
    remove("database/itens_pedido.csv");
    remove("metadata/itens_pedido.csv.meta");
    remove("input_mock.txt");
    remove("output_mock.txt");
    
    printf("test_relatorio_diario PASSED\n");
}

void test_relatorio_mensal() {
    criar_massa_dados();

    // Simula entrada do usuário (mes 10/2023)
    FILE* in = fopen("input_mock.txt", "w");
    fprintf(in, "10/2023\n");
    fclose(in);

    freopen("input_mock.txt", "r", stdin);
    freopen("output_mock.txt", "w", stdout);

    relatorio_mensal();

    freopen("/dev/tty", "w", stdout);

    // Soma do mes 10 é 100 + 50 + 200 = 350
    FILE* out = fopen("output_mock.txt", "r");
    char buffer[1024];
    int encontrou_total = 0;
    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "350.00") != NULL) {
            encontrou_total = 1;
            break;
        }
    }
    fclose(out);

    ASSERT_EQ(1, encontrou_total);

    remove("database/pedidos.csv");
    remove("metadata/pedidos.csv.meta");
    remove("database/itens_pedido.csv");
    remove("metadata/itens_pedido.csv.meta");
    remove("input_mock.txt");
    remove("output_mock.txt");
    
    printf("test_relatorio_mensal PASSED\n");
}

void test_relatorio_anual() {
    criar_massa_dados();

    // Simula entrada do usuário (ano 2023)
    FILE* in = fopen("input_mock.txt", "w");
    fprintf(in, "2023\n");
    fclose(in);

    freopen("input_mock.txt", "r", stdin);
    freopen("output_mock.txt", "w", stdout);

    relatorio_anual();

    freopen("/dev/tty", "w", stdout);

    // Soma do ano é 100 + 50 + 200 = 350
    FILE* out = fopen("output_mock.txt", "r");
    char buffer[1024];
    int encontrou_total = 0;
    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "350.00") != NULL) {
            encontrou_total = 1;
            break;
        }
    }
    fclose(out);

    ASSERT_EQ(1, encontrou_total);

    remove("data/pedidos.csv");
    remove("data/pedidos.csv.meta");
    remove("data/itens_pedido.csv");
    remove("data/itens_pedido.csv.meta");
    remove("input_mock.txt");
    remove("output_mock.txt");
    
    printf("test_relatorio_anual PASSED\n");
}

int main() {
    test_relatorio_diario();
    test_relatorio_mensal();
    test_relatorio_anual();
    return 0;
}
