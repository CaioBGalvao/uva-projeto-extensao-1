#include <stdio.h>
#include <string.h>
#include "relatorio.h"
#include "persistencia.h"
#include "tipos.h"

void relatorio_diario(void) {
    char data_alvo[TAM_DATA];
    printf("Digite a data para o relatorio (AAAA-MM-DD): ");
    if (scanf("%10s", data_alvo) != 1) return;

    Pedido pedidos[1000];
    int qtd_pedidos = 0;
    if (csv_ler_todos_pedidos(pedidos, 1000, &qtd_pedidos) != 0) return;

    float total_faturado = 0.0f;
    int pedidos_no_dia = 0;

    for (int i = 0; i < qtd_pedidos; i++) {
        if (strcmp(pedidos[i].data, data_alvo) == 0) {
            pedidos_no_dia++;
            ItemPedido itens[MAX_ITENS_PEDIDO];
            int qtd_itens = 0;
            if (csv_listar_itens_por_pedido(pedidos[i].id, itens, MAX_ITENS_PEDIDO, &qtd_itens) == 0) {
                for (int j = 0; j < qtd_itens; j++) {
                    total_faturado += itens[j].valor_total;
                }
            }
        }
    }

    printf("\n--- RELATORIO DIARIO ---\n");
    printf("Data: %s\n", data_alvo);
    printf("Pedidos realizados: %d\n", pedidos_no_dia);
    printf("Total Faturado: R$ %.2f\n", total_faturado);
    printf("------------------------\n");

    FILE* f = fopen("relatorio_diario.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO DIARIO ---\n");
        fprintf(f, "Data: %s\n", data_alvo);
        fprintf(f, "Pedidos realizados: %d\n", pedidos_no_dia);
        fprintf(f, "Total Faturado: R$ %.2f\n", total_faturado);
        fprintf(f, "------------------------\n");
        fclose(f);
    }
}


void relatorio_mensal(void) {
    char data_alvo[8];
    printf("Digite o mes para o relatorio (AAAA-MM): ");
    if (scanf("%7s", data_alvo) != 1) return;

    Pedido pedidos[1000];
    int qtd_pedidos = 0;
    if (csv_ler_todos_pedidos(pedidos, 1000, &qtd_pedidos) != 0) return;

    float total_faturado = 0.0f;
    int pedidos_no_mes = 0;

    for (int i = 0; i < qtd_pedidos; i++) {
        if (strncmp(pedidos[i].data, data_alvo, 7) == 0) {
            pedidos_no_mes++;
            ItemPedido itens[MAX_ITENS_PEDIDO];
            int qtd_itens = 0;
            if (csv_listar_itens_por_pedido(pedidos[i].id, itens, MAX_ITENS_PEDIDO, &qtd_itens) == 0) {
                for (int j = 0; j < qtd_itens; j++) {
                    total_faturado += itens[j].valor_total;
                }
            }
        }
    }

    printf("\n--- RELATORIO MENSAL ---\n");
    printf("Mes: %s\n", data_alvo);
    printf("Pedidos realizados: %d\n", pedidos_no_mes);
    printf("Total Faturado: R$ %.2f\n", total_faturado);
    printf("------------------------\n");

    FILE* f = fopen("relatorio_mensal.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO MENSAL ---\n");
        fprintf(f, "Mes: %s\n", data_alvo);
        fprintf(f, "Pedidos realizados: %d\n", pedidos_no_mes);
        fprintf(f, "Total Faturado: R$ %.2f\n", total_faturado);
        fprintf(f, "------------------------\n");
        fclose(f);
    }
}

#include <stdlib.h>

typedef struct {
    int mes;
    float total;
} MesFaturamento;

int comparar_mes(const void* a, const void* b) {
    const MesFaturamento* m1 = (const MesFaturamento*)a;
    const MesFaturamento* m2 = (const MesFaturamento*)b;
    if (m1->total < m2->total) return 1;
    if (m1->total > m2->total) return -1;
    return 0;
}

void relatorio_anual(void) {
    char ano_alvo[5];
    printf("Digite o ano para o relatorio (AAAA): ");
    if (scanf("%4s", ano_alvo) != 1) return;

    Pedido pedidos[1000];
    int qtd_pedidos = 0;
    if (csv_ler_todos_pedidos(pedidos, 1000, &qtd_pedidos) != 0) return;

    MesFaturamento faturamentos[12];
    for (int i = 0; i < 12; i++) {
        faturamentos[i].mes = i + 1;
        faturamentos[i].total = 0.0f;
    }

    float total_anual = 0.0f;

    for (int i = 0; i < qtd_pedidos; i++) {
        if (strncmp(pedidos[i].data, ano_alvo, 4) == 0) {
            int mes;
            sscanf(&pedidos[i].data[5], "%2d", &mes);
            if (mes >= 1 && mes <= 12) {
                ItemPedido itens[MAX_ITENS_PEDIDO];
                int qtd_itens = 0;
                if (csv_listar_itens_por_pedido(pedidos[i].id, itens, MAX_ITENS_PEDIDO, &qtd_itens) == 0) {
                    for (int j = 0; j < qtd_itens; j++) {
                        faturamentos[mes - 1].total += itens[j].valor_total;
                        total_anual += itens[j].valor_total;
                    }
                }
            }
        }
    }

    qsort(faturamentos, 12, sizeof(MesFaturamento), comparar_mes);

    printf("\n--- RELATORIO ANUAL ---\n");
    printf("Ano: %s\n", ano_alvo);
    printf("Ranking de meses:\n");
    for (int i = 0; i < 12; i++) {
        if (faturamentos[i].total > 0) {
            printf("%dº: Mes %02d - R$ %.2f\n", i + 1, faturamentos[i].mes, faturamentos[i].total);
        }
    }
    printf("Total Faturado no Ano: R$ %.2f\n", total_anual);
    printf("------------------------\n");

    FILE* f = fopen("relatorio_anual.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO ANUAL ---\n");
        fprintf(f, "Ano: %s\n", ano_alvo);
        fprintf(f, "Ranking de meses:\n");
        for (int i = 0; i < 12; i++) {
            if (faturamentos[i].total > 0) {
                fprintf(f, "%dº: Mes %02d - R$ %.2f\n", i + 1, faturamentos[i].mes, faturamentos[i].total);
            }
        }
        fprintf(f, "Total Faturado no Ano: R$ %.2f\n", total_anual);
        fprintf(f, "------------------------\n");
        fclose(f);
    }
}
