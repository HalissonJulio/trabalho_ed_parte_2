#ifndef PARTIDA_H
#define PARTIDA_H

#include <stdlib.h>
#include "bdtimes.h" // Necessário para imprimir os nomes

// Define a estrutura Partida (tipo opaco)
typedef struct partida Partida;

// Estrutura interna de uma Partida (definição completa)
struct partida {
    int id;
    int time1_id;     // ID do Time Mandante
    int time2_id;     // ID do Time Visitante
    int gols_time1;   // Gols marcados pelo Mandante
    int gols_time2;   // Gols marcados pelo Visitante
};

// --- Funções de Construção e Destruição ---

// Aloca memória e inicializa uma nova Partida
Partida* partida_cria(int id, int time1_id, int time2_id, int gols_time1, int gols_time2);

// Libera a memória alocada para a estrutura Partida
void partida_libera(Partida* partida);

// --- Funções de Acesso (Getters) ---

// Retorna o ID da partida
int partida_obter_id(const Partida* p);

// Retorna o ID do Time Mandante (Time1)
int partida_obter_time1_id(const Partida* p);

// Retorna o ID do Time Visitante (Time2)
int partida_obter_time2_id(const Partida* p);

// Retorna os gols marcados pelo Time Mandante
int partida_obter_gols_time1(const Partida* p);

// Retorna os gols marcados pelo Time Visitante
int partida_obter_gols_time2(const Partida* p);

// --- Funções Auxiliares (Para a Consulta 2) ---

// Imprime o cabeçalho da consulta de partidas
void partida_imprimir_cabecalho();

// Imprime uma partida formatada (ID Time1 Gols x Gols Time2)
void partida_imprimir(const Partida* p, const BDTimes* bd_times);

#endif // PARTIDA_H