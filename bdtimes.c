#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdtimes.h"
#include "time.h" 

#define MAX_TIMES 10
#define MAX_LINHA_CSV 100 

// Estrutura interna do TAD BDTimes (Vetor Estático)
struct bdtimes {
    Time *times[MAX_TIMES]; 
    int total_times;        
};

// ========================================================================
// Funções de Gerenciamento do BD (Implementação)
// ========================================================================

// Cria e inicializa o gerenciador de times.
BDTimes* bd_times_cria() {
    BDTimes *bd = (BDTimes*) malloc(sizeof(BDTimes));
    if (bd == NULL) {
        perror("[Sistema]\nErro ao alocar BDTimes");
        return NULL;
    }

    bd->total_times = 0;
    for (int i = 0; i < MAX_TIMES; i++) {
        bd->times[i] = NULL;
    }
    return bd;
}

// Destroi o banco de dados e libera a memória de todos os times.
void bd_times_libera(BDTimes *bd) {
    if (bd == NULL) return;
    for (int i = 0; i < MAX_TIMES; i++) {
        if (bd->times[i] != NULL) {
            time_libera(bd->times[i]); 
        }
    }
    free(bd);
}

// Função auxiliar para analisar (parse) a linha CSV do time.
static int analisar_linha_time_csv(char *linha, int *id, char *nome_buffer) {
    char *token;
    int campo = 0;

    char *newline = strchr(linha, '\n');
    if (newline) *newline = '\0';
    
    // Campo 0: ID
    token = strtok(linha, ",");
    if (token == NULL) return 0;
    *id = atoi(token);
    campo++;

    // Campo 1: Nome
    token = strtok(NULL, "\n"); // Lê o resto da linha
    if (token == NULL) return 0;
    strncpy(nome_buffer, token, 49); 
    nome_buffer[49] = '\0';
    campo++;
    
    return campo == 2;
}

// Carrega os dados dos times a partir do arquivo CSV.
int bd_times_carrega_arquivo(BDTimes *bd, const char *nome_arquivo) {
    if (bd == NULL) return -1;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        // Erro silencioso
        return -1;
    }

    char linha[MAX_LINHA_CSV];
    int primeira_linha = 1;
    int contador = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }
        
        char linha_copia[MAX_LINHA_CSV];
        strcpy(linha_copia, linha);

        int id;
        char nome_buffer[50];
        
        if (analisar_linha_time_csv(linha_copia, &id, nome_buffer)) {
            if (id >= 0 && id < MAX_TIMES) {
                if (bd->times[id] != NULL) {
                    time_libera(bd->times[id]); // Libera se já existir
                }
                
                bd->times[id] = time_cria(id, nome_buffer);
                
                if (bd->times[id] != NULL) {
                    contador++;
                }
            } else {
                // Aviso silencioso de ID inválido
            }
        }
    }

    fclose(arquivo);
    bd->total_times = contador;
    return contador;
}


// ========================================================================
// Funcionalidades da Parte I (Implementação)
// ========================================================================

// Imprime as estatísticas de todos os times (Funcionalidade 6).
void bd_times_imprime_classificacao(const BDTimes *bd) {
    if (bd == NULL) return;

    if (bd->total_times == 0) {
        printf("[Sistema]\nNenhum time carregado no sistema.\n");
        return;
    }

    time_imprimir_cabecalho();
    
    for (int i = 0; i < MAX_TIMES; i++) {
        if (bd->times[i] != NULL) {
            time_imprimir(bd->times[i]);
        }
    }
    
    printf("------------------------------------------------------------------------\n");
    printf("Total: %d time(s).\n", bd->total_times);
}

// Consulta e imprime as estatísticas de times por prefixo (Funcionalidade 1).
void bd_times_consulta_por_prefixo(const BDTimes *bd, const char *prefixo) {
    if (bd == NULL || prefixo == NULL) return;
    
    printf("Buscando times com nome que começa com '%s'...\n", prefixo);

    int contador = 0;
    int primeira_impressao = 1;

    for (int i = 0; i < MAX_TIMES; i++) {
        Time* time_atual = bd->times[i];
        
        if (time_atual != NULL && time_nome_comeca_com(time_atual, prefixo)) {
            
            if (primeira_impressao) {
                time_imprimir_cabecalho();
                primeira_impressao = 0;
            }
            
            time_imprimir(time_atual);
            contador++;
        }
    }

    if (contador == 0) {
        printf("Nenhum time encontrado com o nome que começa com '%s'.\n", prefixo);
    } else {
        printf("------------------------------------------------------------------------\n");
        printf("\n%d time(s) encontrado(s).\n", contador);
    }
}


// ========================================================================
// Funções de Acesso e Auxiliares
// ========================================================================

// Retorna o ponteiro para um Time dado o seu ID.
Time* bd_times_busca_por_id(BDTimes *bd, int id) {
    if (bd == NULL) return NULL;

    if (id >= 0 && id < MAX_TIMES) {
        return bd->times[id]; 
    }

    return NULL; // ID fora do range
}