#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "partida.h"
#include "bdtimes.h" // Necessário para imprimir os nomes

// --------------------------------------------------------------------------
// Funções de Construção e Destruição (Implementação)
// --------------------------------------------------------------------------

// Aloca memória e inicializa uma nova Partida
Partida* partida_cria(int id, int time1_id, int time2_id, int gols_time1, int gols_time2) {
    Partida* p = (Partida*) malloc(sizeof(Partida));
    if (p == NULL) {
        perror("[Sistema]\nErro ao alocar Partida");
        return NULL;
    }
    p->id = id;
    p->time1_id = time1_id;
    p->time2_id = time2_id;
    p->gols_time1 = gols_time1;
    p->gols_time2 = gols_time2;
    return p;
}

// Libera a memória alocada para a estrutura Partida
void partida_libera(Partida* partida) {
    if (partida != NULL) {
        free(partida);
    }
}

// --------------------------------------------------------------------------
// Funções de Acesso (Implementação)
// --------------------------------------------------------------------------

// Retorna o ID da partida
int partida_obter_id(const Partida* p) { 
    return (p != NULL) ? p->id : -1; 
}

// Retorna o ID do Time Mandante (Time1)
int partida_obter_time1_id(const Partida* p) { 
    return (p != NULL) ? p->time1_id : -1; 
}

// Retorna o ID do Time Visitante (Time2)
int partida_obter_time2_id(const Partida* p) { 
    return (p != NULL) ? p->time2_id : -1; 
}

// Retorna os gols marcados pelo Time Mandante
int partida_obter_gols_time1(const Partida* p) { 
    return (p != NULL) ? p->gols_time1 : -1; 
}

// Retorna os gols marcados pelo Time Visitante
int partida_obter_gols_time2(const Partida* p) { 
    return (p != NULL) ? p->gols_time2 : -1; 
}

// --------------------------------------------------------------------------
// Funções Auxiliares (Implementação)
// --------------------------------------------------------------------------

// Imprime o cabeçalho da consulta de partidas (estilo Figura 4)
void partida_imprimir_cabecalho() {
    printf("--------------------------------------------------\n");
    printf("%-3s %-12s %-5s %-12s\n", "ID", "Time1", "Placar", "Time2");
    printf("--------------------------------------------------\n");
}

// Imprime uma partida formatada (ID Time1 Gols x Gols Time2)
void partida_imprimir(const Partida* p, const BDTimes* bd_times) {
    if (p == NULL || bd_times == NULL) return;

    // Busca os nomes dos times usando as funções de acesso traduzidas
    Time* t1 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time1_id(p));
    Time* t2 = bd_times_busca_por_id((BDTimes*)bd_times, partida_obter_time2_id(p));

    // Obtém os nomes (assumindo que o getter do Time foi traduzido para time_obter_nome)
    const char* nome1 = (t1 != NULL) ? time_obter_nome(t1) : "N/A";
    const char* nome2 = (t2 != NULL) ? time_obter_nome(t2) : "N/A";

    // Formata a string do placar
    char placar[10]; // "G1 x G2"
    sprintf(placar, "%d x %d", partida_obter_gols_time1(p), partida_obter_gols_time2(p));

    // Imprime formatado
    printf("%-3d %-12s %-5s %-12s\n",
           p->id,
           nome1,
           placar,
           nome2);
}