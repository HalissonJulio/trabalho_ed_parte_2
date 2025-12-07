#ifndef BDPARTIDAS_H
#define BDPARTIDAS_H

#include "partida.h" // Depende do TAD Partida
#include "bdtimes.h" // Necessário para acessar o nome dos times e atualizar estatísticas

#define ARQUIVO_PARTIDAS "bd_partidas.csv" // Nome padrão do arquivo de partidas

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

// Busca uma partida pelo seu ID único
// Retorna o ponteiro para a partida ou NULL se não encontrada
Partida* bd_partidas_buscar_por_id(const BDPartidas *bd, int id);

// Insere uma nova partida, gerando ID automático e atualizando estatísticas dos times
// Retorna 1 em caso de sucesso, 0 caso contrário
int bd_partidas_inserir(BDPartidas *bd, BDTimes *bd_times, int id_time1, int id_time2, int placar1, int placar2);

// Remove uma partida pelo ID e reverte as estatísticas dos times envovidos
// Retorna 1 se removido com sucesso, 0 se ID não encontrado
int bd_partidas_remover(BDPartidas *bd, BDTimes *bd_times, int id);

// Atualiza o placar de uma partida e ajusta as estatísticas (remove antigas, soma novas)
// Retorna 1 se atualizado com sucesso, 0 se ID não encontrado
int bd_partidas_atualizar(BDPartidas *bd, BDTimes *bd_times, int id, int novo_placar1, int novo_placar2);

// Salva as partidas no csv
int bd_partidas_salvar(BDPartidas *bd, const char *nome_arquivo);

#endif // BDPARTIDAS_H