# Campeonato Computacional de Futebol (Parte II)

## Descrição

Este projeto implementa a **Parte II** do sistema de gerenciamento de campeonato de futebol em linguagem C. Diferente da etapa anterior, esta versão evolui a estrutura de dados de vetores estáticos para **Listas Simplesmente Encadeadas**, permitindo alocação dinâmica de memória e manipulação flexível dos dados.

O sistema agora é um gerenciador completo (CRUD), permitindo não apenas a consulta, mas também a **inserção**, **remoção** e **atualização** de partidas, com reflexo imediato e automático na tabela de classificação. Além disso, a tabela final é apresentada **ordenada por mérito esportivo** (Pontos, Vitórias e Saldo de Gols).

### Projeto da Disciplina

> **Disciplina:** Trabalho de Programação
> **Instituição:** IFES – Campus Serra
> **Professor:** Thiago M. Paixão
> **Entrega:** Parte II - Listas Encadeadas e Manutenção de Dados

-----

## Como Executar

### Pré-requisitos

* Um compilador C (GCC / MinGW)
* O utilitário `make` (ou `mingw32-make` no Windows)

### Compilação e Execução

1.  **Compile o projeto** (usando o `Makefile` fornecido):

    ```bash
    make
    ```

2.  **Execute o programa:**

    * No Windows:
      ```bash
      .\main.exe
      ```
    * No Linux/macOS:
      ```bash
      ./main
      ```

3.  **Limpeza** (opcional - remove os arquivos compilados):

    ```bash
    make clean
    ```

**Nota Importante:** O sistema espera encontrar os arquivos `bd_times.csv` e `bd_partidas.csv` na pasta raiz para carregar os dados iniciais.

-----

## Estrutura do Projeto

O código foi modularizado seguindo o padrão de Tipos Abstratos de Dados (TADs).

````

.
├── Makefile               \# Automação de compilação
├── README.md              \# Documentação do projeto
├── main.c                 \# Interface com o usuário (Menus e persistência)
│
├── bd\_times.csv           \# Banco de dados de Times (Leitura)
├── bd\_partidas.csv        \# Banco de dados de Partidas (Leitura e Escrita - Persistência)
├── bd\_classificacao.csv   \# Relatório final gerado automaticamente (Saída)
│
├── time.h / .c            \# TAD Time: Dados e estatísticas de uma equipe
├── partida.h / .c         \# TAD Partida: Dados de um jogo
│
├── bdtimes.h / .c         \# TAD BDTimes: Gerencia a Lista Encadeada de Times e Exportação
└── bdpartidas.h / .c      \# TAD BDPartidas: Gerencia a Lista Encadeada de Partidas e Salvamento

```

-----

## Funcionalidades Implementadas

### 1. Gerenciamento de Dados (CRUD)
Todas as opções do menu estão habilitadas e funcionais:
* **1 - Consultar time:** Busca por prefixo e exibe estatísticas detalhadas.
* **2 - Consultar partidas:** Busca partidas por mandante, visitante ou ambos.
* **3 - Atualizar partida:** Permite alterar o placar de um jogo existente. O sistema recalcula automaticamente os pontos dos times envolvidos (remove a pontuação antiga e aplica a nova).
* **4 - Remover partida:** Exclui um jogo do sistema e reverte as estatísticas (pontos/gols) dos times na tabela.
* **5 - Inserir partida:** Cadastra um novo jogo. O ID é gerado automaticamente e a tabela é atualizada instantaneamente.
* **6 - Imprimir tabela de classificação:** Exibe todos os times ordenados decrescentemente por mérito esportivo.

### 2. Persistência e Exportação (Opção Q - Sair)
Ao encerrar o sistema, duas operações de arquivo são executadas automaticamente:
1.  **Salvamento de Dados:** As alterações na lista de partidas (inserções/remoções) são salvas de volta no arquivo `bd_partidas.csv`.
2.  **Geração de Relatório:** O sistema gera (ou atualiza) o arquivo `bd_classificacao.csv` contendo a tabela final ordenada e formatada, conforme solicitado na especificação de entrega.
    * *Feedback Inteligente:* O sistema informa ao usuário se o arquivo de classificação foi "Gerado" (criado pela primeira vez) ou "Atualizado" (sobrescrito).

-----

## Decisões de Implementação e TADs

### 1. Estruturas de Dados Dinâmicas
Abandonou-se o uso de vetores estáticos. Agora, tanto `BDTimes` quanto `BDPartidas` utilizam **Listas Simplesmente Encadeadas**. Isso permite que o sistema gerencie um número indefinido de partidas sem desperdício de memória.

### 2. Lógica de Atualização de Estatísticas (Undo/Redo)
Para garantir a integridade dos dados durante as operações de edição e remoção, foi implementada uma "Lógica Reversa" no TAD Time (`time_desfaz_partida`).
* **Ao Remover:** O sistema subtrai os pontos e gols da partida excluída.
* **Ao Atualizar:** O sistema primeiro "desfaz" o placar antigo e, em seguida, "registra" o novo.

### 3. Ordenação (Bubble Sort em Lista)
A funcionalidade de imprimir a tabela executa um algoritmo de ordenação antes de exibir os dados. A ordenação respeita a hierarquia: **Pontos (PG) > Vitórias (V) > Saldo de Gols (S)**.

### 4. Usabilidade do Menu
O fluxo de navegação foi aprimorado para evitar loops indesejados. Ao entrar em opções como "Atualizar" ou "Remover", caso o usuário desista e selecione "Retornar" no submenu de busca, o sistema volta imediatamente ao início, sem solicitar IDs.
```