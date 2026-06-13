#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void garantir_cabecalho(FILE* f, const char* cabecalho) {
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        fprintf(f, "%s", cabecalho);
    }
    fseek(f, pos, SEEK_SET);
}

typedef struct {
    int max_id;
    int count;
} Metadata;

static void obter_nome_meta(const char* arquivo_csv, char* arquivo_meta) {
    const char* basename = strrchr(arquivo_csv, '/');
    if (basename) basename++; // Pula o '/'
    else basename = arquivo_csv;
    sprintf(arquivo_meta, "metadata/%s.meta", basename);
}

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
int csv_inserir_cliente(const Cliente* c) {
    FILE* f = fopen(ARQ_CLIENTES, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_CLIENTES);
    fprintf(f, "%d;%s\n", c->id, c->nome);
    fclose(f);
    atualizar_metadados_insercao(ARQ_CLIENTES, c->id);
    return 0;
}

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

// Produto
int csv_inserir_produto(const Produto* p) {
    FILE* f = fopen(ARQ_PRODUTOS, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_PRODUTOS);
    fprintf(f, "%d;%s;%.2f\n", p->id, p->nome, p->preco);
    fclose(f);
    atualizar_metadados_insercao(ARQ_PRODUTOS, p->id);
    return 0;
}

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

// Pedido
int csv_inserir_pedido(const Pedido* p) {
    FILE* f = fopen(ARQ_PEDIDOS, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_PEDIDOS);
    fprintf(f, "%d;%d;%s\n", p->id, p->id_cliente, p->data);
    fclose(f);
    atualizar_metadados_insercao(ARQ_PEDIDOS, p->id);
    return 0;
}

int csv_inserir_item_pedido(const ItemPedido* ip) {
    FILE* f = fopen(ARQ_ITENS_PEDIDO, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_ITENS_PEDIDO);
    fprintf(f, "%d;%d;%d;%d;%.2f;%.2f\n", ip->id, ip->id_pedido, ip->id_produto, ip->quantidade, ip->preco_unitario, ip->valor_total);
    fclose(f);
    atualizar_metadados_insercao(ARQ_ITENS_PEDIDO, ip->id);
    return 0;
}

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

// Devolução
int csv_inserir_devolucao(const Devolucao* d) {
    FILE* f = fopen(ARQ_DEVOLUCOES, "a+");
    if (!f) return -1;
    garantir_cabecalho(f, HEADER_DEVOLUCOES);
    fprintf(f, "%d;%d;%s;%d;%.2f\n", d->id, d->id_item_pedido, d->data, d->contagem, d->taxa_cobrada);
    fclose(f);
    atualizar_metadados_insercao(ARQ_DEVOLUCOES, d->id);
    return 0;
}

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
        if (sscanf(linha, "%d;%d;", &id, &id_item) == 2) {
            if (id_item == id_item_pedido) {
                count++;
            }
        }
    }
    fclose(f);
    return count;
}
