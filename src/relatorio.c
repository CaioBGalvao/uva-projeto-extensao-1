#include <stdio.h>
#include <string.h>
#include "relatorio.h"
#include "persistencia.h"
#include "data_utils.h"
#include "input.h"
#include "logger.h"
#include <stdlib.h>

void relatorio_diario(void) {
    char data_alvo[TAM_DATA];
    limpar_tela();
    printf("\n--- RELATORIO DIARIO ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n\n");
    
    char data_iso[64];
    obter_data_atual(data_iso, 0);
    char data_br[64];
    converter_data_iso_para_br(data_iso, data_br, 0);

    char buffer[64];
    if (!ler_string_com_padrao("Digite a data para o relatorio (DD/MM/AAAA) [Enter para atual]: ", buffer, sizeof(buffer), data_br)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    converter_data_br_para_iso(buffer, data_alvo, 0);

    int pedidos_no_dia = 0;
    float total_faturado = 0.0f;
    float taxas_devolucao = 0.0f;
    if (csv_calcular_faturamento_diario(data_alvo, &pedidos_no_dia, &total_faturado) != 0) {
        printf("\nErro ao calcular relatorio no banco de dados.\n");
        return;
    }
    csv_calcular_taxas_devolucao_diario(data_alvo, &taxas_devolucao);

    char data_br_final[11];
    converter_data_iso_para_br(data_alvo, data_br_final, 0);

    printf("\n--- RELATORIO DIARIO ---\n");
    printf("Data: %s\n", data_br_final);
    printf("Pedidos realizados: %d\n", pedidos_no_dia);
    printf("Faturamento em Pedidos: R$ %.2f\n", total_faturado);
    if (taxas_devolucao > 0) {
        printf("Taxas de Devolucao: R$ %.2f\n", taxas_devolucao);
        printf("Faturamento Geral (Soma): R$ %.2f\n", total_faturado + taxas_devolucao);
    }
    printf("------------------------\n");

    FILE* f = fopen("storage/relatorio_diario.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO DIARIO ---\n");
        fprintf(f, "Data: %s\n", data_br_final);
        fprintf(f, "Pedidos realizados: %d\n", pedidos_no_dia);
        fprintf(f, "Faturamento em Pedidos: R$ %.2f\n", total_faturado);
        if (taxas_devolucao > 0) {
            fprintf(f, "Taxas de Devolucao: R$ %.2f\n", taxas_devolucao);
            fprintf(f, "Faturamento Geral (Soma): R$ %.2f\n", total_faturado + taxas_devolucao);
        }
        fprintf(f, "------------------------\n");
        fclose(f);
    }
    
    registrar_log("RELATORIO: Gerou Relatorio Diario para a data %s e o exportou em 'storage/relatorio_diario.txt'.", data_br_final);
    // free removido
}


void relatorio_mensal(void) {
    char data_alvo[8];
    limpar_tela();
    printf("\n--- RELATORIO MENSAL ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n\n");
    
    char data_iso[64];
    obter_data_atual(data_iso, 1);
    char data_br[64];
    converter_data_iso_para_br(data_iso, data_br, 1);

    char buffer[64];
    if (!ler_string_com_padrao("Digite o mes para o relatorio (MM/AAAA) [Enter para atual]: ", buffer, sizeof(buffer), data_br)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    converter_data_br_para_iso(buffer, data_alvo, 1);

    int pedidos_no_mes = 0;
    float total_faturado = 0.0f;
    float taxas_devolucao = 0.0f;
    if (csv_calcular_faturamento_mensal(data_alvo, &pedidos_no_mes, &total_faturado) != 0) {
        printf("\nErro ao calcular relatorio no banco de dados.\n");
        return;
    }
    csv_calcular_taxas_devolucao_mensal(data_alvo, &taxas_devolucao);

    char data_br_final[8];
    converter_data_iso_para_br(data_alvo, data_br_final, 1);

    printf("\n--- RELATORIO MENSAL ---\n");
    printf("Mes: %s\n", data_br_final);
    printf("Pedidos realizados: %d\n", pedidos_no_mes);
    printf("Faturamento em Pedidos: R$ %.2f\n", total_faturado);
    if (taxas_devolucao > 0) {
        printf("Taxas de Devolucao: R$ %.2f\n", taxas_devolucao);
        printf("Faturamento Geral (Soma): R$ %.2f\n", total_faturado + taxas_devolucao);
    }
    printf("------------------------\n");

    FILE* f = fopen("storage/relatorio_mensal.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO MENSAL ---\n");
        fprintf(f, "Mes: %s\n", data_br_final);
        fprintf(f, "Pedidos realizados: %d\n", pedidos_no_mes);
        fprintf(f, "Faturamento em Pedidos: R$ %.2f\n", total_faturado);
        if (taxas_devolucao > 0) {
            fprintf(f, "Taxas de Devolucao: R$ %.2f\n", taxas_devolucao);
            fprintf(f, "Faturamento Geral (Soma): R$ %.2f\n", total_faturado + taxas_devolucao);
        }
        fprintf(f, "------------------------\n");
        fclose(f);
    }
    
    registrar_log("RELATORIO: Gerou Relatorio Mensal para o mes %s e o exportou em 'storage/relatorio_mensal.txt'.", data_br_final);
    // free removido
}

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
    limpar_tela();
    printf("\n--- RELATORIO ANUAL ---\n");
    printf("(Pressione Ctrl+D ou Ctrl+Z com campo vazio para cancelar)\n\n");
    
    char data_iso[64];
    obter_data_atual(data_iso, 2);
    char data_br[64];
    converter_data_iso_para_br(data_iso, data_br, 2);

    char buffer[64];
    if (!ler_string_com_padrao("Digite o ano para o relatorio (AAAA) [Enter para atual]: ", buffer, sizeof(buffer), data_br)) {
        printf("\nOperacao cancelada.\n");
        return;
    }

    strncpy(ano_alvo, buffer, 4);
    ano_alvo[4] = '\0';

    int qtd_pedidos = 0;
    float total_anual = 0.0f;
    float totais_mes[12];
    float taxas_anuais = 0.0f;
    float totais_mes_taxas[12];
    
    if (csv_calcular_faturamento_anual(ano_alvo, &qtd_pedidos, &total_anual, totais_mes) != 0) {
        printf("\nErro ao calcular relatorio no banco de dados.\n");
        return;
    }
    csv_calcular_taxas_devolucao_anual(ano_alvo, &taxas_anuais, totais_mes_taxas);

    MesFaturamento faturamentos[12];
    for (int i = 0; i < 12; i++) {
        faturamentos[i].mes = i + 1;
        faturamentos[i].total = totais_mes[i] + totais_mes_taxas[i];
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
    printf("Faturamento em Pedidos no Ano: R$ %.2f\n", total_anual);
    if (taxas_anuais > 0) {
        printf("Taxas de Devolucao: R$ %.2f\n", taxas_anuais);
        printf("Faturamento Geral no Ano: R$ %.2f\n", total_anual + taxas_anuais);
    }
    printf("------------------------\n");

    FILE* f = fopen("storage/relatorio_anual.txt", "w");
    if (f) {
        fprintf(f, "--- RELATORIO ANUAL ---\n");
        fprintf(f, "Ano: %s\n", ano_alvo);
        fprintf(f, "Ranking de meses:\n");
        for (int i = 0; i < 12; i++) {
            if (faturamentos[i].total > 0) {
                fprintf(f, "%dº: Mes %02d - R$ %.2f\n", i + 1, faturamentos[i].mes, faturamentos[i].total);
            }
        }
        fprintf(f, "Faturamento em Pedidos no Ano: R$ %.2f\n", total_anual);
        if (taxas_anuais > 0) {
            fprintf(f, "Taxas de Devolucao: R$ %.2f\n", taxas_anuais);
            fprintf(f, "Faturamento Geral no Ano: R$ %.2f\n", total_anual + taxas_anuais);
        }
        fprintf(f, "------------------------\n");
        fclose(f);
    }
    
    registrar_log("RELATORIO: Gerou Relatorio Anual para o ano %s e o exportou em 'storage/relatorio_anual.txt'.", ano_alvo);
    // free removido
}
