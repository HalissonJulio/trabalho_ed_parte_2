#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time.h"

// --------------------------------------------------------------------------
// Funções de Construção e Destruição (Implementação)
// --------------------------------------------------------------------------

// Aloca memória e inicializa um novo Time.
Time* time_cria(int id, const char* nome) {
    Time* time = (Time*) malloc(sizeof(Time));
    
    if (time == NULL) {
        perror("[Sistema]\nErro ao alocar Time");
        return NULL;
    }

    // Inicializa os dados de identificação
    time->id = id;
    strncpy(time->nome, nome, sizeof(time->nome) - 1);
    time->nome[sizeof(time->nome) - 1] = '\0'; // Garante terminação

    // Inicializa as estatísticas com zero
    time->vitorias = 0;
    time->empates = 0;
    time->derrotas = 0;
    time->gols_marcados = 0;
    time->gols_sofridos = 0;

    return time;
}

// Libera a memória alocada para um time.
void time_libera(Time* time) {
    if (time != NULL) {
        free(time);
    }
}

// --------------------------------------------------------------------------
// Funções de Acesso e Cálculo (Implementação)
// --------------------------------------------------------------------------

// Calcula os pontos ganhos (PG) de um time (3*V + E).
int time_obter_pontos(const Time* time) {
    if (time == NULL) return 0;
    return (3 * time->vitorias) + time->empates;
}

// Calcula o saldo de gols (S) de um time (GM - GS).
int time_obter_saldo(const Time* time) {
    if (time == NULL) return 0;
    return time->gols_marcados - time->gols_sofridos;
}

// Retorna o nome do time.
const char* time_obter_nome(const Time* time) {
    if (time == NULL) return "N/A";
    return time->nome;
}

// Retorna o ID do time.
int time_obter_id(const Time* time) {
    return (time != NULL) ? time->id : -1;
}

int time_obter_vitorias(const Time* time) {
    return (time != NULL) ? time->vitorias : 0;
}

int time_obter_gols_marcados(const Time* time) {
    return (time != NULL) ? time->gols_marcados : 0;
}

int time_obter_empates(const Time* time) {
    return (time != NULL) ? time->empates : 0;
}

int time_obter_derrotas(const Time* time) {
    return (time != NULL) ? time->derrotas : 0;
}

int time_obter_gols_sofridos(const Time* time) {
    return (time != NULL) ? time->gols_sofridos : 0;
}

// --------------------------------------------------------------------------
// Funções de Manipulação
// --------------------------------------------------------------------------

// Atualiza as estatísticas de um time com base no resultado de um jogo.
void time_registra_partida(Time* time, int gols_marcados, int gols_sofridos) {
    if (time == NULL) return;

    time->gols_marcados += gols_marcados;
    time->gols_sofridos += gols_sofridos;

    if (gols_marcados > gols_sofridos) {
        time->vitorias++;
    } else if (gols_marcados < gols_sofridos) {
        time->derrotas++;
    } else {
        time->empates++;
    }
}

// Reverte as estatísticas subtraindo um resultado de partida anterior.
void time_desfaz_partida(Time* time, int gols_marcados, int gols_sofridos) {
    if (time == NULL) return;

    // Subtrai os gols
    time->gols_marcados -= gols_marcados;
    time->gols_sofridos -= gols_sofridos;

    // Decrementa o contador apropriado baseado no resultado original
    if (gols_marcados > gols_sofridos) {
        time->vitorias--;
    } else if (gols_marcados < gols_sofridos) {
        time->derrotas--;
    } else {
        time->empates--;
    }
}

// --------------------------------------------------------------------------
// Funções Auxiliares (Implementação)
// --------------------------------------------------------------------------

// Imprime o cabeçalho da tabela de classificação.
void time_imprimir_cabecalho() {
    printf("------------------------------------------------------------------------\n");
    printf("%-3s %-12s %-3s %-3s %-3s %-4s %-4s %-4s %-3s\n",
           "ID", "Time", "V", "E", "D", "GM", "GS", "S", "PG");
    printf("------------------------------------------------------------------------\n");
}

// Imprime os dados de um time formatados para a tabela.
void time_imprimir(const Time* time) {
    if (time == NULL) return;

    printf("%-3d %-12s %-3d %-3d %-3d %-4d %-4d %-4d %-3d\n",
           time->id,
           time->nome,
           time->vitorias,
           time->empates,
           time->derrotas,
           time->gols_marcados,
           time->gols_sofridos,
           time_obter_saldo(time), 
           time_obter_pontos(time) 
           );
}

// Função auxiliar (CASE-SENSITIVE) para comparar o início de duas strings.
static int str_prefix_cmp(const char* str, const char* prefix) {
    while (*prefix) {
        if (*str != *prefix) {
            return 1; // Não bate
        }
        str++;
        prefix++;
    }
    return 0; // Bate (é um prefixo)
}

// Verifica se o nome de um time começa com um determinado prefixo.
int time_nome_comeca_com(const Time* time, const char* prefixo) {
    if (time == NULL || prefixo == NULL) {
        return 0;
    }
    return str_prefix_cmp(time->nome, prefixo) == 0;
}