#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdtimes.h"
#include "time.h" 

#define MAX_LINHA_CSV 100 

// ========================================================================
// Estruturas de Dados (Lista Encadeada)
// ========================================================================

// Nó da lista encadeada de times
typedef struct NoTime {
    Time *time;
    struct NoTime *prox;
} NoTime;

// Estrutura interna do TAD BDTimes (Agora dinâmica)
struct bdtimes {
    NoTime *inicio;
    int total_times;        
};

// ========================================================================
// Funções Auxiliares (Privadas)
// ========================================================================

// Insere um time no final da lista encadeada
static void inserir_time_na_lista(BDTimes *bd, Time *t) {
    NoTime *novo = (NoTime*) malloc(sizeof(NoTime));
    if (!novo) return;
    
    novo->time = t;
    novo->prox = NULL;

    if (bd->inicio == NULL) {
        bd->inicio = novo;
    } else {
        NoTime *atual = bd->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    bd->total_times++;
}

// Analisa a linha CSV do time (Mantida da Parte 1)
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
    token = strtok(NULL, "\n"); 
    if (token == NULL) return 0;
    strncpy(nome_buffer, token, 49); 
    nome_buffer[49] = '\0';
    campo++;
    
    return campo == 2;
}

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

    bd->inicio = NULL;
    bd->total_times = 0;
    return bd;
}

// Destroi o banco de dados e libera a memória de todos os times.
void bd_times_libera(BDTimes *bd) {
    if (bd == NULL) return;
    
    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        NoTime *temp = atual;
        atual = atual->prox;
        
        // Libera o objeto Time e o nó da lista
        if (temp->time) time_libera(temp->time);
        free(temp);
    }
    free(bd);
}

// Carrega os dados dos times a partir do arquivo CSV.
int bd_times_carrega_arquivo(BDTimes *bd, const char *nome_arquivo) {
    if (bd == NULL) return -1;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        return -1; // Erro silencioso
    }

    char linha[MAX_LINHA_CSV];
    int primeira_linha = 1;
    int contador = 0;

    // Limpa lista anterior se houver (para evitar duplicatas em recargas)
    // (Opcional, assumindo carregamento único na inicialização)

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
            Time *novo_time = time_cria(id, nome_buffer);
            if (novo_time) {
                inserir_time_na_lista(bd, novo_time);
                contador++;
            }
        }
    }

    fclose(arquivo);
    return contador;
}

// ========================================================================
// Funcionalidades da Parte II (Ordenação)
// ========================================================================

// Ordena a lista de times por mérito esportivo (PG > V > S)
void bd_times_ordenar(BDTimes *bd) {
    if (bd == NULL || bd->inicio == NULL || bd->inicio->prox == NULL) return;

    int trocou;
    do {
        trocou = 0;
        NoTime *atual = bd->inicio;

        while (atual->prox != NULL) {
            Time *t1 = atual->time;
            Time *t2 = atual->prox->time;
            int deve_trocar = 0;

            // Critérios de Classificação (Decrescente):
            // 1. Pontos Ganhos (PG)
            if (time_obter_pontos(t2) > time_obter_pontos(t1)) {
                deve_trocar = 1;
            } 
            else if (time_obter_pontos(t2) == time_obter_pontos(t1)) {
                // 2. Vitórias (V)
                if (time_obter_vitorias(t2) > time_obter_vitorias(t1)) {
                    deve_trocar = 1;
                }
                else if (time_obter_vitorias(t2) == time_obter_vitorias(t1)) {
                    // 3. Saldo de Gols (S)
                    if (time_obter_saldo(t2) > time_obter_saldo(t1)) {
                        deve_trocar = 1;
                    }
                }
            }

            if (deve_trocar) {
                // Troca apenas os dados (ponteiros de Time) para simplificar
                Time *temp = atual->time;
                atual->time = atual->prox->time;
                atual->prox->time = temp;
                trocou = 1;
            }
            atual = atual->prox;
        }
    } while (trocou);
}

// ========================================================================
// Funcionalidades da Parte I (Adaptadas para Lista Encadeada)
// ========================================================================

// Imprime as estatísticas de todos os times (Funcionalidade 6).
void bd_times_imprime_classificacao(const BDTimes *bd) {
    if (bd == NULL) return;

    if (bd->total_times == 0) {
        printf("[Sistema]\nNenhum time carregado no sistema.\n");
        return;
    }

    // ORDENAÇÃO: Requisito da Parte 2
    // Como o parâmetro é const, fazemos um cast para ordenar internamente antes de exibir
    // (Ou a função deveria receber sem const, mas para manter compatibilidade fazemos assim)
    bd_times_ordenar((BDTimes*)bd);

    time_imprimir_cabecalho();
    
    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        time_imprimir(atual->time);
        atual = atual->prox;
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

    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        if (time_nome_comeca_com(atual->time, prefixo)) {
            if (primeira_impressao) {
                time_imprimir_cabecalho();
                primeira_impressao = 0;
            }
            time_imprimir(atual->time);
            contador++;
        }
        atual = atual->prox;
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

    NoTime *atual = bd->inicio;
    while (atual != NULL) {
        if (time_obter_id(atual->time) == id) {
            return atual->time;
        }
        atual = atual->prox;
    }

    return NULL; // Não encontrado
}