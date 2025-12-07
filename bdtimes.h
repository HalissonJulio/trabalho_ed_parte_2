#ifndef BDTIMES_H
#define BDTIMES_H

#include "time.h" // Depende do TAD Time

#define ARQUIVO_TIMES "times.csv" // Nome padrão do arquivo de times

// Define a estrutura BDTimes (tipo opaco)
typedef struct bdtimes BDTimes;

// --- Funções de Gerenciamento do BD ---

// Cria e inicializa o gerenciador de times
BDTimes* bd_times_cria();

// Destroi o banco de dados e libera a memória de todos os times
void bd_times_libera(BDTimes *bd);

// Carrega os dados dos times a partir do arquivo CSV para a lista encadeada
int bd_times_carrega_arquivo(BDTimes *bd, const char *nome_arquivo);

// Imprime as estatísticas de todos os times (tabela de classificação)
void bd_times_imprime_classificacao(const BDTimes *bd);

// Consulta e imprime as estatísticas de times por prefixo de nome
void bd_times_consulta_por_prefixo(const BDTimes *bd, const char *prefixo);

// --- Funções de Acesso e Auxiliares ---

// Retorna o ponteiro para um Time dado o seu ID (0 a 9)
// Essencial para atualizar estatísticas quando partidas são modificadas
Time* bd_times_busca_por_id(BDTimes *bd, int id);

// Ordena a lista de times por mérito esportivo (PG > V > S > GM)
// Deve ser chamada antes de imprimir a classificação
void bd_times_ordenar(BDTimes *bd);

#endif // BDTIMES_H