#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h" // Depende do TAD Partida
#include "bdtimes.h" // Necessário para acessar o nome dos times

#define ARQUIVO_PARTIDAS "partidas.csv" // Nome padrão do arquivo de partidas

// Define a estrutura BDPartidas (tipo opaco)
typedef struct bdpartidas BDPartidas;

// Cria e aloca memória para o gerenciador de partidas
BDPartidas* bd_partidas_cria();

// Libera a memória do gerenciador de partidas e todas as partidas contidas
void bd_partidas_libera(BDPartidas *bd);

// Carrega o arquivo CSV de partidas e atualiza as estatísticas dos times
int bd_partidas_carrega_arquivo(BDPartidas *bd, BDTimes *bd_times, const char *nome_arquivo);

// Consulta partidas por nome de time (mandante, visitante ou ambos)
void bd_partidas_consulta_por_time(const BDPartidas *bd, const BDTimes *bd_times, const char *nome_prefixo, int modo);

// Retorna o número total de partidas carregadas
int bd_partidas_total(const BDPartidas *bd);

#endif // BDPARTIDAS_H