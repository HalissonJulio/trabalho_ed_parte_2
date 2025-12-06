#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdpartidas.h"
#include "partida.h"
#include "bdtimes.h" 
#include "time.h"    

#define MAX_PARTIDAS 45
#define MAX_LINHA_CSV 100

// Estrutura interna do TAD BDPartidas (Vetor Estático)
struct bdpartidas {
    Partida *partidas[MAX_PARTIDAS]; 
    int total_partidas;
};

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
    
    bd->total_partidas = 0;
    for (int i = 0; i < MAX_PARTIDAS; i++) {
        bd->partidas[i] = NULL;
    }
    return bd;
}

// Libera a memória do gerenciador de partidas e todas as partidas contidas
void bd_partidas_libera(BDPartidas *bd) {
    if (bd == NULL) return;

    for (int i = 0; i < MAX_PARTIDAS; i++) {
        if (bd->partidas[i] != NULL) {
            partida_libera(bd->partidas[i]);
        }
    }
    free(bd);
}

// Função auxiliar para inserir uma partida no vetor
static int bd_partidas_inserir(BDPartidas *bd, Partida *p) {
    if (bd == NULL || p == NULL) return 0;
    
    int id = partida_obter_id(p); // Usando a função traduzida
    
    if (id >= 0 && id < MAX_PARTIDAS) {
        if (bd->partidas[id] != NULL) {
            partida_libera(bd->partidas[id]); 
        }
        bd->partidas[id] = p;
        bd->total_partidas++; 
        return 1;
    } else {
        return 0; // ID inválido, falha silenciosa
    }
}

// Função auxiliar para analisar (parse) a linha CSV da partida
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

// Carrega o arquivo de partidas
int bd_partidas_carrega_arquivo(BDPartidas *bd, BDTimes *bd_times, const char *nome_arquivo) {
    if (bd == NULL || bd_times == NULL) return 0;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        // Erro silencioso, conforme solicitado
        return 0;
    }

    char linha[MAX_LINHA_CSV];
    int primeira_linha = 1;
    int partidas_carregadas = 0;
    bd->total_partidas = 0; // Reseta o contador

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }

        if (strlen(linha) <= 1) { // Pula linhas vazias
            continue;
        }

        char linha_copia[MAX_LINHA_CSV];
        strcpy(linha_copia, linha);

        Partida *nova_partida = NULL;
        int id, t1_id, t2_id, g1, g2;
        
        if (analisar_linha_partida_csv(linha_copia, &id, &t1_id, &t2_id, &g1, &g2)) {
            
            nova_partida = partida_cria(id, t1_id, t2_id, g1, g2);
            if (nova_partida == NULL) {
                continue;
            }

            if (bd_partidas_inserir(bd, nova_partida)) {
                partidas_carregadas++;

                // ATUALIZA ESTATÍSTICAS
                Time* time1 = bd_times_busca_por_id(bd_times, t1_id);
                Time* time2 = bd_times_busca_por_id(bd_times, t2_id);

                if (time1 && time2) {
                    time_registra_partida(time1, g1, g2);
                    time_registra_partida(time2, g2, g1);
                } 
            } else {
                partida_libera(nova_partida);
            }
        }
    }

    fclose(arquivo);
    return partidas_carregadas; 
}

// ========================================================================
// Funcionalidades da Parte I
// ========================================================================

// Consulta partidas por nome de time (mandante, visitante ou ambos)
void bd_partidas_consulta_por_time(const BDPartidas *bd, const BDTimes *bd_times, const char *prefixo, int modo) {
    if (bd == NULL || bd_times == NULL || prefixo == NULL) return;

    printf("[Sistema]\nBuscando partidas (Modo: %d, Prefixo: '%s')...\n", modo, prefixo);

    int contador = 0;
    int primeira_impressao = 1;

    for (int i = 0; i < MAX_PARTIDAS; i++) {
        Partida* p = bd->partidas[i];
        if (p == NULL) continue; 

        Time* t1 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time1_id(p));
        Time* t2 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time2_id(p));

        if (t1 == NULL || t2 == NULL) continue; 

        int match1 = time_nome_comeca_com(t1, prefixo);
        int match2 = time_nome_comeca_com(t2, prefixo);

        int imprimir = 0;
        switch (modo) {
            case 1: 
                if (match1) imprimir = 1;
                break;
            case 2: 
                if (match2) imprimir = 1;
                break;
            case 3: 
                if (match1 || match2) imprimir = 1;
                break;
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

    if (contador == 0) {
        printf("Nenhuma partida encontrada para a busca.\n");
    } else {
        printf("--------------------------------------------------\n");
        printf("\n%d partida(s) encontrada(s).\n", contador);
    }
}

// ========================================================================
// Funções Auxiliares
// ========================================================================

// Retorna o número total de partidas carregadas
int bd_partidas_total(const BDPartidas *bd) {
    return (bd != NULL) ? bd->total_partidas : 0;
}