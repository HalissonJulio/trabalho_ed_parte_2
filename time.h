#ifndef TIME_H
#define TIME_H

// Define a estrutura Time
typedef struct time Time;

// Estrutura interna de um Time 
struct time {
  int id;
  char nome[50];
  int vitorias;
  int empates;
  int derrotas;
  int gols_marcados;
  int gols_sofridos;
};

// --- Funções de Construção e Destruição ---

// Aloca memória e inicializa um novo Time (com estatísticas zeradas)
Time* time_cria(int id, const char* nome);

// Libera a memória alocada para a estrutura Time
void time_libera(Time* time);

// --- Funções de Acesso e Cálculo ---

// Calcula os pontos ganhos (PG) do time (3*V + E)
int time_obter_pontos(const Time* time);

// Calcula o saldo de gols (S) do time (GM - GS)
int time_obter_saldo(const Time* time);

// Retorna o nome do time
const char* time_obter_nome(const Time* time);

// Retorna o ID do time
int time_obter_id(const Time* time);

// Retorna o número de vitórias (Critério de desempate 1)
int time_obter_vitorias(const Time* time);

// Retorna o número de gols marcados (Critério de desempate secundário/informativo)
int time_obter_gols_marcados(const Time* time);

// --- Funções de Manipulação ---

// Atualiza as estatísticas (V, E, D, GM, GS) somando um resultado de partida
void time_registra_partida(Time* time, int gols_marcados, int gols_sofridos);

// --- NOVA FUNÇÃO (Parte 2 - Necessária para Remover ou Atualizar) ---

// Reverte as estatísticas subtraindo um resultado de partida anterior.
void time_desfaz_partida(Time* time, int gols_marcados, int gols_sofridos);

// --- Funções Auxiliares de Impressão e Busca ---

// Imprime o cabeçalho da tabela de classificação
void time_imprimir_cabecalho();

// Imprime os dados de um time formatados para a tabela
void time_imprimir(const Time* time);

// Verifica se o nome de um time começa com um prefixo (case-sensitive)
int time_nome_comeca_com(const Time* time, const char* prefixo);

#endif // TIME_H