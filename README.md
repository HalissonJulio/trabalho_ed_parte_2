# Campeonato Computacional de Futebol (Parte I)

## Descrição

Este projeto implementa a Parte I de um sistema simplificado em linguagem C para o gerenciamento de dados de um campeonato de futebol. Os dados são lidos de arquivos `.csv` (`times.csv` e `partidas.csv`) e carregados em estruturas de dados em memória (TADs) baseadas em **vetores estáticos**, conforme especificado.

O sistema permite **consultar times** por prefixo, **consultar partidas** (por mandante, visitante ou ambos) e **imprimir a tabela de classificação** completa. As estatísticas (Vitórias, Empates, Derrotas, Gols) são calculadas dinamicamente ao carregar os resultados das partidas.

### Projeto da Disciplina

> **Disciplina:** Trabalho de Programação
> **Instituição:** IFES – Campus Serra
> **Professor:** Thiago M. Paixão

-----

## Como Executar

### Pré-requisitos

  * Um compilador C (GCC / MinGW)
  * O utilitário `make`

### Instalação e Execução

1.  **Compile o projeto** (usando o `Makefile`):

    ```bash
    make
    ```

2.  **Execute o programa:**

      * No Windows (MinGW/MSYS2):
        ```bash
        ./campeonato.exe
        ```
      * No Linux/macOS:
        ```bash
        ./campeonato
        ```

3.  **Limpeza** (remove os arquivos compilados):

    ```bash
    make clean
    ```

### Cenários de Teste

O programa é projetado para ler `times.csv` e `partidas.csv`. Para testar os três cenários de teste especificados:

1.  **Renomeie** o arquivo de cenário desejado (`partidas_vazio.csv`, `partidas_parcial.csv` ou `partidas_completo.csv`) para `partidas.csv`.
2.  Execute o programa. O sistema carregará o arquivo renomeado.

-----

## Estrutura do Projeto

```
.
├── Makefile               # Arquivo de compilação
├── README.md              # Esta documentação
├── main.c                 # Ponto de entrada e interface do usuário
│
├── times.csv              # Dados dos 10 times
├── partidas_vazio.csv     # Cenário 1: Campeonato Vazio
├── partidas_parcial.csv   # Cenário 2: Campeonato em Andamento
└── partidas_completo.csv  # Cenário 3: Campeonato Finalizado
│
├── time.h                 # Interface do TAD Time
├── time.c                 # Implementação do TAD Time
│
├── partida.h              # Interface do TAD Partida
├── partida.c              # Implementação do TAD Partida
│
├── bdtimes.h              # Interface do TAD BDTimes (Gerenciador de Times)
├── bdtimes.c              # Implementação do TAD BDTimes
│
├── bdpartidas.h           # Interface do TAD BDPartidas (Gerenciador de Partidas)
└── bdpartidas.c           # Implementação do TAD BDPartidas
```

-----

## Funcionalidades (Parte I)

O menu principal oferece as seguintes operações:

  * **1 - Consultar time:** Permite buscar times pelo prefixo do nome (case-sensitive) e exibe suas estatísticas, conforme a Figura 3 da especificação.
  * **2 - Consultar partidas:** Permite buscar partidas por prefixo de time (mandante, visitante ou ambos) e exibe os placares, conforme a Figura 4.
  * **6 - Imprimir tabela de classificação:** Exibe a tabela completa com todos os times e suas estatísticas (V, E, D, GM, GS, S, PG), ordenados por ID (conforme Figura 5).
  * **Q - Sair:** Encerra o programa.
  * **3, 4, 5 - (DESABILITADO):** Funções de escrita (Atualizar, Remover, Inserir) não implementadas nesta fase, conforme especificado.

-----

## TADs (Tipos Abstratos de Dados)

### 1\. TAD Time (`time.h` / `time.c`)

Representa uma única equipe e suas estatísticas de desempenho.

**Estrutura (`time.h`):**

```c
struct time {
    int id;
    char nome[50];
    int vitorias;
    int empates;
    int derrotas;
    int gols_marcados;
    int gols_sofridos;
};
```

### 2\. TAD Partida (`partida.h` / `partida.c`)

Representa um único jogo realizado.

**Estrutura (`partida.h`):**

```c
struct partida {
    int id;
    int time1_id;     // ID do Time Mandante
    int time2_id;     // ID do Time Visitante
    int gols_time1;   // Gols marcados pelo Mandante
    int gols_time2;   // Gols marcados pelo Visitante
};
```

### 3\. TAD BDTimes (`bdtimes.h` / `bdtimes.c`)

Gerencia a coleção de todos os times. Conforme a especificação, utiliza um **vetor estático** de ponteiros.

**Estrutura interna (`bdtimes.c`):**

```c
#define MAX_TIMES 10

struct bdtimes {
    Time *times[MAX_TIMES]; // Vetor de ponteiros (0 a 9)
    int total_times;
};
```

### 4\. TAD BDPartidas (`bdpartidas.h` / `bdpartidas.c`)

Gerencia a coleção de todas as partidas lidas. Também utiliza um **vetor estático**.

**Estrutura interna (`bdpartidas.c`):**

```c
#define MAX_PARTIDAS 45 // C(10, 2) = 45 partidas totais

struct bdpartidas {
    Partida *partidas[MAX_PARTIDAS]; // Vetor de ponteiros (0 a 44)
    int total_partidas;
};
```

-----