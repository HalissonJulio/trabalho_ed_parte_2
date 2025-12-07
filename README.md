
# Campeonato Computacional de Futebol (Parte II)

## Descrição

Este projeto implementa a Parte II do sistema de gerenciamento de campeonato de futebol em linguagem C. Diferente da etapa anterior, esta versão evolui a estrutura de dados de vetores estáticos para Listas Simplesmente Encadeadas, permitindo alocação dinâmica de memória e manipulação flexível de um número indefinido de registros.

O sistema agora é um gerenciador completo (CRUD), permitindo a inserção, remoção e atualização de partidas, com reflexo imediato e automático na tabela de classificação. Além disso, a tabela final é apresentada e exportada ordenada por mérito esportivo (Pontos, Vitórias e Saldo de Gols).

## Projeto da Disciplina

| Informação | Detalhes |
| :--- | :--- |
| **Disciplina** | Trabalho de Programação |
| **Instituição** | IFES - Campus Serra |
| **Professor** | Thiago M. Paixão |
| **Entrega** | Parte II - Listas Encadeadas e Manutenção de Dados |
| **Valor** | 25 pontos |

---

## Como Executar

### Pré-requisitos

- Um compilador C (GCC / MinGW)  
- O utilitário make (ou mingw32-make no Windows)

### Compilação e Execução

1. **Compile o projeto** (usando o Makefile fornecido):
   ```bash
   make


2. **Execute o programa:**

   * **Windows:**

     ```bash
     .\main.exe
     ```
   * **Linux/macOS:**

     ```bash
     ./main
     ```

3. **Limpeza** (opcional – remove os arquivos compilados):

   ```bash
   make clean
   ```

> **Nota Importante:** O sistema espera encontrar os arquivos `bd_times.csv` e `bd_partidas.csv` na pasta raiz para carregar os dados iniciais.

---

## Estrutura do Projeto

O código foi modularizado seguindo o padrão de Tipos Abstratos de Dados (TADs) para garantir o encapsulamento e a organização.

```
.
├── Makefile                # Automação de compilação
├── README.md               # Documentação do projeto
├── main.c                  # Interface com o usuário (Menus e persistência)
│
├── bd_times.csv            # Banco de dados de Times (Leitura)
├── bd_partidas.csv         # Banco de dados de Partidas (Leitura e Escrita - Persistência)
├── bd_classificacao.csv    # Relatório final gerado automaticamente (Saída)
│
├── time.h / .c             # TAD Time: Estrutura de dados e métricas da equipe
├── partida.h / .c          # TAD Partida: Estrutura de dados do jogo
│
├── bdtimes.h / .c          # TAD BDTimes: Lista Encadeada de Times e ordenação
└── bdpartidas.h / .c       # TAD BDPartidas: Lista Encadeada de Partidas e manipulação
```

---

## Funcionalidades Implementadas

### 1. Gerenciamento de Dados (CRUD)

Conforme especificado no menu principal (`main.c`), as seguintes operações estão disponíveis:

1. Consultar time: Busca por prefixo e exibe estatísticas detalhadas (V, E, D, GM, GS, S, PG).
2. Consultar partidas: Busca flexível (por mandante, visitante ou ambos) utilizando prefixos.
3. Atualizar partida: Permite alterar o placar de um jogo existente. O usuário busca a partida, identifica o ID e insere os novos valores.
4. Remover partida: Exclui um jogo do sistema permanentemente após confirmação do usuário.
5. Inserir partida: Cadastra um novo jogo. O sistema valida a existência dos times e gera um ID automaticamente.
6. Imprimir tabela de classificação: Exibe no terminal todos os times ordenados decrescentemente.

### 2. Persistência e Exportação (Opção Q – Sair)

A persistência dos dados ocorre apenas no encerramento do programa para otimizar o I/O:

* **Salvamento de Partidas:** O arquivo `bd_partidas.csv` é sobrescrito com a lista atualizada de jogos (incluindo novas inserções e removendo as excluídas).
* **Geração de Classificação:** O sistema gera o arquivo `bd_classificacao.csv` contendo a tabela final ordenada.
* **Feedback:** O sistema informa se o arquivo foi criado ("gerado") ou apenas sobrescrito ("atualizado").

---

## Decisões de Implementação e TADs

### 1. Estruturas de Dados Dinâmicas (Linked List)

Para atender ao requisito de alocação dinâmica, foram implementadas estruturas de nós internos nos TADs gerenciadores:

* **BDTimes:** utiliza `struct NoTime` para encadear as estruturas `Time`.
* **BDPartidas:** utiliza `struct NoPartida` para encadear as estruturas `Partida`.

Isso permite que o sistema cresça conforme a necessidade, sem `realloc`s complexos ou limites fixos de vetores.

### 2. Consistência de Dados (Undo/Redo Logic)

Para manter a integridade da tabela de classificação durante edições, implementou-se uma lógica de reversão no TAD Time (`time.c`):

* **Remoção:** utiliza-se a função `time_desfaz_partida` para subtrair os gols e pontos da partida removida.
* **Atualização:** o sistema executa `time_desfaz_partida` com os dados antigos e, em seguida, `time_registra_partida` com os novos dados, garantindo que o saldo e pontuação estejam sempre corretos.

### 3. Algoritmo de Ordenação

A função `bd_times_ordenar` implementa um Bubble Sort adaptado para listas encadeadas.
A ordenação é estritamente decrescente e respeita os critérios de desempate do PDF:

* **PG:** Pontos Ganhos
* **V:** Número de Vitórias
* **S:** Saldo de Gols

### 4. Geração de IDs (Auto-incremento)

Na inserção de novas partidas, o sistema não solicita o ID ao usuário.
A função interna `obter_proximo_id` percorre a lista existente, encontra o maior ID cadastrado e soma +1, garantindo unicidade sem necessidade de controle externo.

---