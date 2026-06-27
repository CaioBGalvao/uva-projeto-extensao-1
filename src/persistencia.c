/**
 * @file persistencia.c
 * @author Ana Silva (Dev 3)
 * @brief Implementacao do modulo de Persistencia (Banco de Dados em CSV).
 * 
 * Este arquivo foi documentado conforme o padrao CDoc/Doxygen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif
#include "persistencia.h"

#define ARQ_CLIENTES "database/clientes.csv"
#define ARQ_PRODUTOS "database/produtos.csv"
#define ARQ_PEDIDOS "database/pedidos.csv"
#define ARQ_ITENS_PEDIDO "database/itens_pedido.csv"
#define ARQ_DEVOLUCOES "database/devolucoes.csv"

#define HEADER_CLIENTES "id;nome\n"
#define HEADER_PRODUTOS "id;nome;preco\n"
#define HEADER_PEDIDOS "id;id_cliente;data\n"
#define HEADER_ITENS_PEDIDO "id;id_pedido;id_produto;quantidade;preco_unitario;valor_total\n"
#define HEADER_DEVOLUCOES "id;id_item_pedido;data;contagem;taxa_cobrada\n"

/**
 * @brief Executa a operacao de garantir_cabecalho.
 *
 * @param f Parametro de entrada.
 * @param cabecalho Parametro de entrada.
 * @return static void Retorno da operacao.
 */
static void garantir_cabecalho(FILE* f, const char* cabecalho) {
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        fprintf(f, "%s", cabecalho);
    }
    fseek(f, pos, SEEK_SET);
}

/**
 * @brief Executa a operacao de inicializacao da persistencia.
 *
 */
void persistencia_init(void) {
    MKDIR("database");
    MKDIR("metadata");
    MKDIR("storage");
}

typedef struct {
    int max_id;
    int count;
} Metadata;

/**
 * @brief Executa a operacao de obter_nome_meta.
 *
 * @param arquivo_csv Parametro de entrada.
 * @param arquivo_meta Parametro de entrada.
 * @return static void Retorno da operacao.
 */
static void obter_nome_meta(const char* arquivo_csv, char* arquivo_meta) {
    const char* basename = strrchr(arquivo_csv, '/');
    if (basename) basename++; // Pula o '/'
    else basename = arquivo_csv;
    sprintf(arquivo_meta, "metadata/%s.meta", basename);
}

/**
 * @brief Executa a operacao de ler_metadados.
 *
 * @param arquivo_csv Parametro de entrada.
 * @param meta Parametro de entrada.
 * @return static int Retorno da operacao.
 */
static int ler_metadados(const char* arquivo_csv, Metadata* meta) {
    char arquivo_meta[256];
    obter_nome_meta(arquivo_csv, arquivo_meta);
    FILE* f = fopen(arquivo_meta, "rb");
    if (!f) return -1;
    if (fread(meta, sizeof(Metadata), 1, f) != 1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

/**
 * @brief Executa a operacao de salvar_metadados.
 *
 * @param arquivo_csv Parametro de entrada.
 * @param meta Parametro de entrada.
 * @return static void Retorno da operacao.
 */
static void salvar_metadados(const char* arquivo_csv, const Metadata* meta) {
    char arquivo_meta[256];
    obter_nome_meta(arquivo_csv, arquivo_meta);
    FILE* f = fopen(arquivo_meta, "wb");
    if (f) {
        fwrite(meta, sizeof(Metadata), 1, f);
        fclose(f);
    }
}

// Protótipo para o helper que atualiza ao inserir
static void atualizar_metadados_insercao(const char* arquivo_csv, int novo_id);

/**
 * @brief Executa a operacao de csv_obter_proximo_id.
 *
 * @param arquivo Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_obter_proximo_id(const char* arquivo) {
    Metadata meta;
    if (ler_metadados(arquivo, &meta) == 0) {
        return meta.max_id + 1;
    }

    FILE* f = fopen(arquivo, "r");
    if (!f) return 1;

    char linha[1024];
    // Pula header
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 1;
    }

    meta.max_id = 0;
    meta.count = 0;
    while (fgets(linha, sizeof(linha), f)) {
        meta.count++;
        int id = 0;
        if (sscanf(linha, "%d;", &id) == 1) {
            if (id > meta.max_id) {
                meta.max_id = id;
            }
        }
    }
    fclose(f);
    salvar_metadados(arquivo, &meta);
    return meta.max_id + 1;
}

/**
 * @brief Executa a operacao de atualizar_metadados_insercao.
 *
 * @param arquivo_csv Parametro de entrada.
 * @param novo_id Parametro de entrada.
 * @return static void Retorno da operacao.
 */
static void atualizar_metadados_insercao(const char* arquivo_csv, int novo_id) {
    Metadata meta;
    if (ler_metadados(arquivo_csv, &meta) == 0) {
        if (novo_id > meta.max_id) meta.max_id = novo_id;
        meta.count++;
        salvar_metadados(arquivo_csv, &meta);
    } else {
        // Falhou ler, chama csv_obter_proximo_id para forçar reconstrução lendo do CSV
        csv_obter_proximo_id(arquivo_csv);
    }
}

// Cliente
/**
 * @brief Executa a operacao de csv_inserir_cliente.
 *
 * @param c Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_inserir_cliente(const Cliente* c) {
    FILE* f = fopen(ARQ_CLIENTES, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_CLIENTES);
    fprintf(f, "%d;%s\n", c->id, c->nome);
    fclose(f);
    atualizar_metadados_insercao(ARQ_CLIENTES, c->id);
    return 0;
}

/**
 * @brief Executa a operacao de csv_buscar_cliente_por_id.
 *
 * @param id Parametro de entrada.
 * @param resultado Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_buscar_cliente_por_id(int id, Cliente* resultado) {
    FILE* f = fopen(ARQ_CLIENTES, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return -1;
    }

    while (fgets(linha, sizeof(linha), f)) {
        int cid = 0;
        char cnome[MAX_NOME] = {0};
        if (sscanf(linha, "%d;%254[^\n]", &cid, cnome) >= 1) {
            if (cid == id) {
                resultado->id = cid;
                strncpy(resultado->nome, cnome, MAX_NOME);
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return -1;
}

/**
 * @brief Executa a operacao de csv_ler_todos_clientes.
 *
 * @param resultados Parametro de entrada.
 * @param max_resultados Parametro de entrada.
 * @param qtd_encontrada Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_ler_todos_clientes(Cliente* resultados, int max_resultados, int* qtd_encontrada) {
    *qtd_encontrada = 0;
    FILE* f = fopen(ARQ_CLIENTES, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) && *qtd_encontrada < max_resultados) {
        Cliente c = {0};
        if (sscanf(linha, "%d;%254[^\n]", &c.id, c.nome) >= 1) {
            resultados[*qtd_encontrada] = c;
            (*qtd_encontrada)++;
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Le uma pagina de clientes do CSV.
 *
 * @param resultados Buffer para receber os clientes da pagina.
 * @param page_size Quantidade de registros por pagina.
 * @param page Indice da pagina (0-based).
 * @param qtd_lida Ponteiro para receber quantos registros foram lidos na pagina.
 * @param total_registros Ponteiro para receber o total de registros no arquivo.
 * @return int 0 para sucesso, -1 para erro.
 */
int csv_ler_pagina_clientes(Cliente* resultados, int page_size, int page, int* qtd_lida, int* total_registros) {
    *qtd_lida = 0;
    *total_registros = 0;
    FILE* f = fopen(ARQ_CLIENTES, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    int skip = page * page_size;
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        Cliente c = {0};
        if (sscanf(linha, "%d;%254[^\n]", &c.id, c.nome) >= 1) {
            if (count >= skip && *qtd_lida < page_size) {
                resultados[*qtd_lida] = c;
                (*qtd_lida)++;
            }
            count++;
        }
    }
    *total_registros = count;
    fclose(f);
    return 0;
}

// Produto
/**
 * @brief Executa a operacao de csv_inserir_produto.
 *
 * @param p Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_inserir_produto(const Produto* p) {
    FILE* f = fopen(ARQ_PRODUTOS, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_PRODUTOS);
    fprintf(f, "%d;%s;%.2f\n", p->id, p->nome, p->preco);
    fclose(f);
    atualizar_metadados_insercao(ARQ_PRODUTOS, p->id);
    return 0;
}

/**
 * @brief Executa a operacao de csv_buscar_produto_por_id.
 *
 * @param id Parametro de entrada.
 * @param resultado Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_buscar_produto_por_id(int id, Produto* resultado) {
    FILE* f = fopen(ARQ_PRODUTOS, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return -1;
    }

    while (fgets(linha, sizeof(linha), f)) {
        int pid = 0;
        char pnome[MAX_NOME] = {0};
        float ppreco = 0;
        if (sscanf(linha, "%d;%254[^;];%f", &pid, pnome, &ppreco) == 3) {
            if (pid == id) {
                resultado->id = pid;
                strncpy(resultado->nome, pnome, MAX_NOME);
                resultado->preco = ppreco;
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return -1;
}

/**
 * @brief Executa a operacao de csv_ler_todos_produtos.
 *
 * @param resultados Parametro de entrada.
 * @param max_resultados Parametro de entrada.
 * @param qtd_encontrada Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_ler_todos_produtos(Produto* resultados, int max_resultados, int* qtd_encontrada) {
    *qtd_encontrada = 0;
    FILE* f = fopen(ARQ_PRODUTOS, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) && *qtd_encontrada < max_resultados) {
        Produto p = {0};
        if (sscanf(linha, "%d;%254[^;];%f", &p.id, p.nome, &p.preco) == 3) {
            resultados[*qtd_encontrada] = p;
            (*qtd_encontrada)++;
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Le uma pagina de produtos do CSV.
 *
 * @param resultados Buffer para receber os produtos da pagina.
 * @param page_size Quantidade de registros por pagina.
 * @param page Indice da pagina (0-based).
 * @param qtd_lida Ponteiro para receber quantos registros foram lidos na pagina.
 * @param total_registros Ponteiro para receber o total de registros no arquivo.
 * @return int 0 para sucesso, -1 para erro.
 */
int csv_ler_pagina_produtos(Produto* resultados, int page_size, int page, int* qtd_lida, int* total_registros) {
    *qtd_lida = 0;
    *total_registros = 0;
    FILE* f = fopen(ARQ_PRODUTOS, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    int skip = page * page_size;
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        Produto p = {0};
        if (sscanf(linha, "%d;%254[^;];%f", &p.id, p.nome, &p.preco) == 3) {
            if (count >= skip && *qtd_lida < page_size) {
                resultados[*qtd_lida] = p;
                (*qtd_lida)++;
            }
            count++;
        }
    }
    *total_registros = count;
    fclose(f);
    return 0;
}

// Pedido
/**
 * @brief Executa a operacao de csv_inserir_pedido.
 *
 * @param p Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_inserir_pedido(const Pedido* p) {
    FILE* f = fopen(ARQ_PEDIDOS, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_PEDIDOS);
    fprintf(f, "%d;%d;%s\n", p->id, p->id_cliente, p->data);
    fclose(f);
    atualizar_metadados_insercao(ARQ_PEDIDOS, p->id);
    return 0;
}

/**
 * @brief Executa a operacao de csv_inserir_item_pedido.
 *
 * @param ip Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_inserir_item_pedido(const ItemPedido* ip) {
    FILE* f = fopen(ARQ_ITENS_PEDIDO, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_ITENS_PEDIDO);
    fprintf(f, "%d;%d;%d;%d;%.2f;%.2f\n", ip->id, ip->id_pedido, ip->id_produto, ip->quantidade, ip->preco_unitario, ip->valor_total);
    fclose(f);
    atualizar_metadados_insercao(ARQ_ITENS_PEDIDO, ip->id);
    return 0;
}

/**
 * @brief Executa a operacao de csv_listar_itens_por_pedido.
 *
 * @param id_pedido Parametro de entrada.
 * @param resultados Parametro de entrada.
 * @param max_resultados Parametro de entrada.
 * @param qtd_encontrada Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_listar_itens_por_pedido(int id_pedido, ItemPedido* resultados, int max_resultados, int* qtd_encontrada) {
    *qtd_encontrada = 0;
    FILE* f = fopen(ARQ_ITENS_PEDIDO, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) && *qtd_encontrada < max_resultados) {
        ItemPedido ip = {0};
        if (sscanf(linha, "%d;%d;%d;%d;%f;%f", &ip.id, &ip.id_pedido, &ip.id_produto, &ip.quantidade, &ip.preco_unitario, &ip.valor_total) == 6) {
            if (ip.id_pedido == id_pedido) {
                resultados[*qtd_encontrada] = ip;
                (*qtd_encontrada)++;
            }
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Executa a operacao de csv_ler_todos_pedidos.
 *
 * @param resultados Parametro de entrada.
 * @param max_resultados Parametro de entrada.
 * @param qtd_encontrada Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_ler_todos_pedidos(Pedido* resultados, int max_resultados, int* qtd_encontrada) {
    *qtd_encontrada = 0;
    FILE* f = fopen(ARQ_PEDIDOS, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) && *qtd_encontrada < max_resultados) {
        Pedido p = {0};
        if (sscanf(linha, "%d;%d;%10[^\n]", &p.id, &p.id_cliente, p.data) >= 2) {
            resultados[*qtd_encontrada] = p;
            (*qtd_encontrada)++;
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Le uma pagina de pedidos do CSV.
 *
 * @param resultados Buffer para receber os pedidos da pagina.
 * @param page_size Quantidade de registros por pagina.
 * @param page Indice da pagina (0-based).
 * @param qtd_lida Ponteiro para receber quantos registros foram lidos na pagina.
 * @param total_registros Ponteiro para receber o total de registros no arquivo.
 * @return int 0 para sucesso, -1 para erro.
 */
int csv_ler_pagina_pedidos(Pedido* resultados, int page_size, int page, int* qtd_lida, int* total_registros) {
    *qtd_lida = 0;
    *total_registros = 0;
    FILE* f = fopen(ARQ_PEDIDOS, "r");
    if (!f) return -1;

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    int skip = page * page_size;
    int count = 0;

    while (fgets(linha, sizeof(linha), f)) {
        Pedido p = {0};
        if (sscanf(linha, "%d;%d;%10[^\n]", &p.id, &p.id_cliente, p.data) >= 2) {
            if (count >= skip && *qtd_lida < page_size) {
                resultados[*qtd_lida] = p;
                (*qtd_lida)++;
            }
            count++;
        }
    }
    *total_registros = count;
    fclose(f);
    return 0;
}

/**
 * @brief Executa a operacao de calcular_faturamento_agregado.
 *
 * @param prefixo_data Parametro de entrada.
 * @param out_pedidos Parametro de entrada.
 * @param out_faturado Parametro de entrada.
 * @param totais_mes[12] Parametro de entrada.
 * @return static int Retorno da operacao.
 */
static int calcular_faturamento_agregado(const char* prefixo_data, int* out_pedidos, float* out_faturado, float totais_mes[12]) {
    *out_pedidos = 0;
    *out_faturado = 0.0f;
    if (totais_mes) {
        for (int i = 0; i < 12; i++) totais_mes[i] = 0.0f;
    }
    
    int max_id = csv_obter_proximo_id(ARQ_PEDIDOS);
    char* pedido_valido = calloc(max_id + 1, 1);
    char* pedido_mes = calloc(max_id + 1, 1);
    if (!pedido_valido || !pedido_mes) {
        if (pedido_valido) free(pedido_valido);
        if (pedido_mes) free(pedido_mes);
        return -1;
    }
    
    FILE* f_pedidos = fopen(ARQ_PEDIDOS, "r");
    if (!f_pedidos) { free(pedido_valido); free(pedido_mes); return -1; }
    
    char linha[1024];
    if (fgets(linha, sizeof(linha), f_pedidos)) {
        while (fgets(linha, sizeof(linha), f_pedidos)) {
            Pedido p = {0};
            if (sscanf(linha, "%d;%d;%10[^\n]", &p.id, &p.id_cliente, p.data) >= 2) {
                char *cr = strchr(p.data, '\r'); if (cr) *cr = '\0';
                if (prefixo_data == NULL || prefixo_data[0] == '\0' || strncmp(p.data, prefixo_data, strlen(prefixo_data)) == 0) {
                    if (p.id <= max_id) {
                        pedido_valido[p.id] = 1;
                        (*out_pedidos)++;
                        if (totais_mes) {
                            int mes = 0;
                            sscanf(&p.data[5], "%2d", &mes);
                            pedido_mes[p.id] = (char)mes;
                        }
                    }
                }
            }
        }
    }
    fclose(f_pedidos);
    
    if (*out_pedidos == 0) {
        free(pedido_valido); free(pedido_mes); return 0;
    }
    
    FILE* f_itens = fopen(ARQ_ITENS_PEDIDO, "r");
    if (!f_itens) { free(pedido_valido); free(pedido_mes); return -1; }
    
    if (fgets(linha, sizeof(linha), f_itens)) {
        while (fgets(linha, sizeof(linha), f_itens)) {
            ItemPedido ip = {0};
            if (sscanf(linha, "%d;%d;%d;%d;%f;%f", &ip.id, &ip.id_pedido, &ip.id_produto, &ip.quantidade, &ip.preco_unitario, &ip.valor_total) == 6) {
                if (ip.id_pedido <= max_id && pedido_valido[ip.id_pedido]) {
                    *out_faturado += ip.valor_total;
                    if (totais_mes) {
                        int mes = pedido_mes[ip.id_pedido];
                        if (mes >= 1 && mes <= 12) {
                            totais_mes[mes - 1] += ip.valor_total;
                        }
                    }
                }
            }
        }
    }
    fclose(f_itens);
    free(pedido_valido); free(pedido_mes);
    return 0;
}

/**
 * @brief Executa a operacao de csv_calcular_faturamento_diario.
 *
 * @param data_iso Parametro de entrada.
 * @param out_pedidos Parametro de entrada.
 * @param out_faturado Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_faturamento_diario(const char* data_iso, int* out_pedidos, float* out_faturado) {
    return calcular_faturamento_agregado(data_iso, out_pedidos, out_faturado, NULL);
}

/**
 * @brief Executa a operacao de csv_calcular_faturamento_mensal.
 *
 * @param ano_mes_iso Parametro de entrada.
 * @param out_pedidos Parametro de entrada.
 * @param out_faturado Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_faturamento_mensal(const char* ano_mes_iso, int* out_pedidos, float* out_faturado) {
    return calcular_faturamento_agregado(ano_mes_iso, out_pedidos, out_faturado, NULL);
}

/**
 * @brief Executa a operacao de csv_calcular_faturamento_anual.
 *
 * @param ano_iso Parametro de entrada.
 * @param out_pedidos Parametro de entrada.
 * @param out_faturado_total Parametro de entrada.
 * @param totais_mes[12] Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_faturamento_anual(const char* ano_iso, int* out_pedidos, float* out_faturado_total, float totais_mes[12]) {
    return calcular_faturamento_agregado(ano_iso, out_pedidos, out_faturado_total, totais_mes);
}

// Devolução
/**
 * @brief Executa a operacao de csv_inserir_devolucao.
 *
 * @param d Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_inserir_devolucao(const Devolucao* d) {
    FILE* f = fopen(ARQ_DEVOLUCOES, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_DEVOLUCOES);
    fprintf(f, "%d;%d;%s;%d;%.2f\n", d->id, d->id_item_pedido, d->data, d->contagem, d->taxa_cobrada);
    fclose(f);
    atualizar_metadados_insercao(ARQ_DEVOLUCOES, d->id);
    return 0;
}

/**
 * @brief Executa a operacao de csv_contar_devolucoes_item.
 *
 * @param id_item_pedido Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_contar_devolucoes_item(int id_item_pedido) {
    FILE* f = fopen(ARQ_DEVOLUCOES, "r");
    if (!f) return 0; // se o arquivo não existe, há 0 devoluções

    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }

    int count = 0;
    while (fgets(linha, sizeof(linha), f)) {
        int id = 0, id_item = 0;
        char data_str[20] = {0};
        int contagem = 0;
        float taxa = 0.0f;
        if (sscanf(linha, "%d;%d;%19[^;];%d;%f", &id, &id_item, data_str, &contagem, &taxa) >= 4) {
            if (id_item == id_item_pedido) {
                count += contagem;
            }
        }
    }
    fclose(f);
    return count;
}

/**
 * @brief Executa a operacao de calcular_taxas_devolucao_agregado.
 *
 * @param prefixo_data Parametro de entrada.
 * @param out_taxas Parametro de entrada.
 * @param totais_mes[12] Parametro de entrada.
 * @return static int Retorno da operacao.
 */
static int calcular_taxas_devolucao_agregado(const char* prefixo_data, float* out_taxas, float totais_mes[12]) {
    *out_taxas = 0.0f;
    if (totais_mes) {
        for (int i = 0; i < 12; i++) totais_mes[i] = 0.0f;
    }
    FILE* f = fopen(ARQ_DEVOLUCOES, "r");
    if (!f) return 0;
    
    char linha[1024];
    if (!fgets(linha, sizeof(linha), f)) {
        fclose(f);
        return 0;
    }
    
    while (fgets(linha, sizeof(linha), f)) {
        int id = 0, id_item = 0, contagem = 0;
        char data_str[20] = {0};
        float taxa = 0.0f;
        if (sscanf(linha, "%d;%d;%19[^;];%d;%f", &id, &id_item, data_str, &contagem, &taxa) == 5) {
            char *cr = strchr(data_str, '\r'); if (cr) *cr = '\0';
            if (prefixo_data == NULL || prefixo_data[0] == '\0' || strncmp(data_str, prefixo_data, strlen(prefixo_data)) == 0) {
                *out_taxas += taxa;
                if (totais_mes) {
                    int mes = 0;
                    sscanf(&data_str[5], "%2d", &mes);
                    if (mes >= 1 && mes <= 12) {
                        totais_mes[mes - 1] += taxa;
                    }
                }
            }
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Executa a operacao de csv_calcular_taxas_devolucao_diario.
 *
 * @param data_iso Parametro de entrada.
 * @param out_taxas Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_taxas_devolucao_diario(const char* data_iso, float* out_taxas) {
    return calcular_taxas_devolucao_agregado(data_iso, out_taxas, NULL);
}

/**
 * @brief Executa a operacao de csv_calcular_taxas_devolucao_mensal.
 *
 * @param ano_mes_iso Parametro de entrada.
 * @param out_taxas Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_taxas_devolucao_mensal(const char* ano_mes_iso, float* out_taxas) {
    return calcular_taxas_devolucao_agregado(ano_mes_iso, out_taxas, NULL);
}

/**
 * @brief Executa a operacao de csv_calcular_taxas_devolucao_anual.
 *
 * @param ano_iso Parametro de entrada.
 * @param out_taxas_total Parametro de entrada.
 * @param totais_mes[12] Parametro de entrada.
 * @return int Retorno da operacao.
 */
int csv_calcular_taxas_devolucao_anual(const char* ano_iso, float* out_taxas_total, float totais_mes[12]) {
    return calcular_taxas_devolucao_agregado(ano_iso, out_taxas_total, totais_mes);
}
