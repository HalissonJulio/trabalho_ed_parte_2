#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdpartidas.h"
#include "partida.h"
#include "bdtimes.h" 
#include "time.h"    

#define MAX_LINHA_CSV 100

// ========================================================================
// Estruturas de Dados (Lista Encadeada)
// ========================================================================

// Nó da lista encadeada de partidas
typedef struct NoPartida {
    Partida *partida;
    struct NoPartida *prox;
} NoPartida;

// Estrutura interna do TAD BDPartidas (Agora dinâmica)
struct bdpartidas {
    NoPartida *inicio;
    int total_partidas;
};

// ========================================================================
// Funções Auxiliares (Privadas)
// ========================================================================

// Gera o próximo ID disponível (Auto-incremento baseado no maior ID existente)
static int obter_proximo_id(BDPartidas *bd) {
    int max_id = -1;
    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        int id = partida_obter_id(atual->partida);
        if (id > max_id) {
            max_id = id;
        }
        atual = atual->prox;
    }
    return max_id + 1;
}

// Insere um nó no final da lista
static void inserir_na_lista(BDPartidas *bd, Partida *p) {
    NoPartida *novo = (NoPartida*) malloc(sizeof(NoPartida));
    novo->partida = p;
    novo->prox = NULL;

    if (bd->inicio == NULL) {
        bd->inicio = novo;
    } else {
        NoPartida *atual = bd->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    bd->total_partidas++;
}

// Analisa a linha CSV (Função mantida da Parte 1, sem alterações de lógica)
static int analisar_linha_partida_csv(char *linha, int *id, int *time1, int *time2, int *gols1, int *gols2) {
    char *token;
    int campo = 0;

    char *newline = strchr(linha, '\n');
    if (newline) *newline = '\0';
    
    token = strtok(linha, ",");
    if (token == NULL) return 0;
    *id = atoi(token);
    campo++;

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    *time1 = atoi(token);
    campo++;

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    *time2 = atoi(token);
    campo++;

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    *gols1 = atoi(token);
    campo++;

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    *gols2 = atoi(token);
    campo++;
    
    return campo == 5; 
}

// ========================================================================
// Funções de Gerenciamento do BD
// ========================================================================

// Cria e aloca memória para o gerenciador de partidas
BDPartidas* bd_partidas_cria() {
    BDPartidas *bd = (BDPartidas*) malloc(sizeof(BDPartidas));
    if (bd == NULL) {
        perror("[Sistema]\nErro: Nao foi possivel alocar memoria para o BDPartidas.");
        return NULL;
    }
    
    bd->inicio = NULL;
    bd->total_partidas = 0;
    return bd;
}

// Libera a memória do gerenciador de partidas e todas as partidas contidas
void bd_partidas_libera(BDPartidas *bd) {
    if (bd == NULL) return;

    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        NoPartida *temp = atual;
        atual = atual->prox;
        
        // Libera a estrutura Partida e o Nó da lista
        partida_libera(temp->partida);
        free(temp);
    }
    free(bd);
}

// Carrega o arquivo de partidas
int bd_partidas_carrega_arquivo(BDPartidas *bd, BDTimes *bd_times, const char *nome_arquivo) {
    if (bd == NULL || bd_times == NULL) return 0;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        return 0; // Erro silencioso
    }

    char linha[MAX_LINHA_CSV];
    int primeira_linha = 1;
    int partidas_carregadas = 0;
    
    // Limpa lista atual se necessário (opcional, aqui assume-se append ou lista vazia)

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }

        if (strlen(linha) <= 1) continue;

        char linha_copia[MAX_LINHA_CSV];
        strcpy(linha_copia, linha);

        int id, t1_id, t2_id, g1, g2;
        
        if (analisar_linha_partida_csv(linha_copia, &id, &t1_id, &t2_id, &g1, &g2)) {
            
            Partida *nova_partida = partida_cria(id, t1_id, t2_id, g1, g2);
            if (nova_partida == NULL) continue;

            // Insere na lista encadeada
            inserir_na_lista(bd, nova_partida);
            partidas_carregadas++;

            // ATUALIZA ESTATÍSTICAS (Soma)
            Time* time1 = bd_times_busca_por_id(bd_times, t1_id);
            Time* time2 = bd_times_busca_por_id(bd_times, t2_id);

            if (time1 && time2) {
                time_registra_partida(time1, g1, g2);
                time_registra_partida(time2, g2, g1);
            } 
        }
    }

    fclose(arquivo);
    return partidas_carregadas; 
}

// ========================================================================
// Funcionalidades da Parte I (Adaptadas para Lista Encadeada)
// ========================================================================

// Consulta partidas por nome de time (mandante, visitante ou ambos)
void bd_partidas_consulta_por_time(const BDPartidas *bd, const BDTimes *bd_times, const char *prefixo, int modo) {
    if (bd == NULL || bd_times == NULL || prefixo == NULL) return;

    printf("[Sistema]\nBuscando partidas (Modo: %d, Prefixo: '%s')...\n", modo, prefixo);

    int contador = 0;
    int primeira_impressao = 1;

    // Percorre a lista encadeada
    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        Partida* p = atual->partida;
        
        Time* t1 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time1_id(p));
        Time* t2 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time2_id(p));

        if (t1 != NULL && t2 != NULL) {
            int match1 = time_nome_comeca_com(t1, prefixo);
            int match2 = time_nome_comeca_com(t2, prefixo);

            int imprimir = 0;
            switch (modo) {
                case 1: if (match1) imprimir = 1; break;
                case 2: if (match2) imprimir = 1; break;
                case 3: if (match1 || match2) imprimir = 1; break;
            }

            if (imprimir) {
                if (primeira_impressao) {
                    partida_imprimir_cabecalho();
                    primeira_impressao = 0;
                }
                partida_imprimir(p, bd_times);
                contador++;
            }
        }
        atual = atual->prox;
    }

    if (contador == 0) {
        printf("Nenhuma partida encontrada para a busca.\n");
    } else {
        printf("--------------------------------------------------\n");
        printf("\n%d partida(s) encontrada(s).\n", contador);
    }
}

int bd_partidas_total(const BDPartidas *bd) {
    return (bd != NULL) ? bd->total_partidas : 0;
}

// ========================================================================
// Funcionalidades da Parte II (Novas)
// ========================================================================

// Busca uma partida específica pelo ID
Partida* bd_partidas_buscar_por_id(const BDPartidas *bd, int id) {
    if (bd == NULL) return NULL;
    
    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        if (partida_obter_id(atual->partida) == id) {
            return atual->partida;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Insere uma nova partida manualmente
int bd_partidas_inserir(BDPartidas *bd, BDTimes *bd_times, int id_time1, int id_time2, int gols_time1, int gols_time2) {
    if (bd == NULL || bd_times == NULL) return 0;

    // Valida se os times existem
    Time* t1 = bd_times_busca_por_id(bd_times, id_time1);
    Time* t2 = bd_times_busca_por_id(bd_times, id_time2);

    if (t1 == NULL || t2 == NULL) {
        printf("[Sistema]\nErro: Um ou ambos os times nao existem.\n");
        return 0;
    }

    // Gera ID e cria partida
    int novo_id = obter_proximo_id(bd);
    Partida *p = partida_cria(novo_id, id_time1, id_time2, gols_time1, gols_time2);
    
    if (p == NULL) return 0;

    // Adiciona na lista
    inserir_na_lista(bd, p);

    // ATUALIZA ESTATÍSTICAS (Soma)
    time_registra_partida(t1, gols_time1, gols_time2);
    time_registra_partida(t2, gols_time2, gols_time1);

    return 1;
}

// Remove uma partida existente pelo ID
int bd_partidas_remover(BDPartidas *bd, BDTimes *bd_times, int id) {
    if (bd == NULL || bd->inicio == NULL) return 0;

    NoPartida *atual = bd->inicio;
    NoPartida *anterior = NULL;

    // Busca o nó e o anterior
    while (atual != NULL && partida_obter_id(atual->partida) != id) {
        anterior = atual;
        atual = atual->prox;
    }

    // Se não encontrou
    if (atual == NULL) return 0;

    // REVERTE ESTATÍSTICAS (Subtrai)
    Partida *p = atual->partida;
    Time* t1 = bd_times_busca_por_id(bd_times, partida_obter_time1_id(p));
    Time* t2 = bd_times_busca_por_id(bd_times, partida_obter_time2_id(p));

    if (t1 && t2) {
        int g1 = partida_obter_gols_time1(p);
        int g2 = partida_obter_gols_time2(p);
        time_desfaz_partida(t1, g1, g2); // Requer nova função no time.c
        time_desfaz_partida(t2, g2, g1);
    }

    // Remove da lista (ajusta ponteiros)
    if (anterior == NULL) {
        bd->inicio = atual->prox; // Era o primeiro
    } else {
        anterior->prox = atual->prox;
    }

    // Libera memória
    partida_libera(p);
    free(atual);
    bd->total_partidas--;

    return 1;
}

// Atualiza o placar de uma partida existente
int bd_partidas_atualizar(BDPartidas *bd, BDTimes *bd_times, int id, int novo_gols1, int novo_gols2) {
    if (bd == NULL) return 0;

    // Busca a partida na lista (reutiliza a função de busca ou navega para ter acesso ao nó se precisasse)
    // Aqui precisamos apenas do objeto Partida
    Partida *p = bd_partidas_buscar_por_id(bd, id);
    if (p == NULL) return 0;

    Time* t1 = bd_times_busca_por_id(bd_times, partida_obter_time1_id(p));
    Time* t2 = bd_times_busca_por_id(bd_times, partida_obter_time2_id(p));

    if (t1 && t2) {
        // 1. REVERTE estatísticas antigas
        int g1_antigo = partida_obter_gols_time1(p);
        int g2_antigo = partida_obter_gols_time2(p);
        time_desfaz_partida(t1, g1_antigo, g2_antigo);
        time_desfaz_partida(t2, g2_antigo, g1_antigo);

        // 2. ATUALIZA os dados da partida
        partida_set_gols(p, novo_gols1, novo_gols2); // Requer nova função no partida.c

        // 3. APLICA estatísticas novas
        time_registra_partida(t1, novo_gols1, novo_gols2);
        time_registra_partida(t2, novo_gols2, novo_gols1);
        
        return 1;
    }

    return 0;
}

int bd_partidas_salvar(BDPartidas *bd, const char *nome_arquivo) {
    if (bd == NULL) return 0;

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo de partidas para salvar");
        return 0;
    }

    // Cabeçalho esperado
    fprintf(arquivo, "ID,Time1,Time2,GolsTime1,GolsTime2\n");

    NoPartida *atual = bd->inicio;
    while (atual != NULL) {
        Partida *p = atual->partida;
        fprintf(arquivo, "%d,%d,%d,%d,%d\n",
                partida_obter_id(p),
                partida_obter_time1_id(p),
                partida_obter_time2_id(p),
                partida_obter_gols_time1(p),
                partida_obter_gols_time2(p));
        atual = atual->prox;
    }
    fclose(arquivo);
    return 1;
}