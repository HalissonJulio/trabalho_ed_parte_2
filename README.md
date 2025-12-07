# Campeonato Computacional de Futebol (Parte II)

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Status](https://img.shields.io/badge/Status-Completed-green.svg)
![Type](https://img.shields.io/badge/Type-Academic%20Project-orange.svg)

## 📋 Descrição

Este projeto implementa a **Parte II** do sistema de gerenciamento de campeonato de futebol em linguagem C. Diferente da etapa anterior, esta versão evolui a estrutura de dados de vetores estáticos para **Listas Simplesmente Encadeadas**, permitindo alocação dinâmica de memória e manipulação flexível dos dados.

O sistema agora é um gerenciador completo (CRUD), permitindo não apenas a consulta, mas também a **inserção**, **remoção** e **atualização** de partidas, com reflexo imediato e automático na tabela de classificação. Além disso, a tabela final é apresentada **ordenada por mérito esportivo** (Pontos, Vitórias e Saldo de Gols).

### 🎓 Projeto da Disciplina

| Informação | Detalhes |
| :--- | :--- |
| **Disciplina** | Trabalho de Programação |
| **Instituição** | IFES – Campus Serra |
| **Professor** | Thiago M. Paixão |
| **Entrega** | Parte II - Listas Encadeadas e Manutenção de Dados |

---

## 🚀 Como Executar

### Pré-requisitos

* Um compilador C (GCC / MinGW)
* O utilitário `make` (ou `mingw32-make` no Windows)

### Compilação e Execução

1.  **Compile o projeto** (usando o `Makefile` fornecido):
    ```bash
    make
    ```

2.  **Execute o programa:**
    * **Windows:**
        ```bash
        .\main.exe
        ```
    * **Linux/macOS:**
        ```bash
        ./main
        ```

3.  **Limpeza** (opcional - remove os arquivos compilados):
    ```bash
    make clean
    ```

> **Nota Importante:** O sistema espera encontrar os arquivos `bd_times.csv` e `bd_partidas.csv` na pasta raiz para carregar os dados iniciais.

---

## 📂 Estrutura do Projeto

O código foi modularizado seguindo o padrão de Tipos Abstratos de Dados (TADs).

```text
.
├── Makefile                # Automação de compilação
├── README.md               # Documentação do projeto
├── main.c                  # Interface com o usuário (Menus e persistência)
│
├── bd_times.csv            # Banco de dados de Times (Leitura)
├── bd_partidas.csv         # Banco de dados de Partidas (Leitura e Escrita - Persistência)
├── bd_classificacao.csv    # Relatório final gerado automaticamente (Saída)
│
├── time.h / .c             # TAD Time: Dados e estatísticas de uma equipe
├── partida.h / .c          # TAD Partida: Dados de um jogo
│
├── bdtimes.h / .c          # TAD BDTimes: Gerencia a Lista Encadeada de Times e Exportação
└── bdpartidas.h /