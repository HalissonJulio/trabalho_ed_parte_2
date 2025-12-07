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

**Nota:** O sistema carrega automaticamente os arquivos `times.csv` e `partidas.csv` presentes na pasta raiz. Ao encerrar o programa (Opção Q), as alterações realizadas nas partidas são salvas de volta no arquivo `partidas.csv`.

-----

## Estrutura do Projeto

O código foi modularizado seguindo o padrão de Tipos Abstratos de Dados (TADs).