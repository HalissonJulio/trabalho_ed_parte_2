#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bdtimes.h"
#include "bdpartidas.h"

#define MAX_INPUT 256 

// Lê uma linha completa do stdin de forma segura
void ler_linha_stdin(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin)) {
        // Substitui o '\n' (quebra de linha) por '\0' (fim da string)
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        // Garante que o buffer esteja vazio em caso de erro
        buffer[0] = '\0';
    }
}

// Exibe o menu principal do sistema
void exibir_menu_principal() {
    printf("\n============================================\n");
    printf("  Sistema de Gerenciamento de Partidas\n");
    printf("============================================\n");
    printf(" 1 - Consultar time\n");
    printf(" 2 - Consultar partidas\n");
    printf(" 3 - Atualizar partida (DESABILITADO)\n");
    printf(" 4 - Remover partida (DESABILITADO)\n");
    printf(" 5 - Inserir partida (DESABILITADO)\n");
    printf(" 6 - Imprimir tabela de classificacao\n");
    printf(" Q - Sair\n");
    printf("--------------------------------------------\n");
    printf("[Sistema]\nEscolha uma opcao: ");
}

// Exibe o sub-menu para a consulta de partidas
void exibir_menu_consulta_partida() {
    printf("\n[Sistema]\n");
    printf("Escolha o modo de consulta:\n");
    printf(" 1 - Por time mandante\n");
    printf(" 2 - Por time visitante\n");
    printf(" 3 - Por time mandante ou visitante\n");
    printf(" 4 - Retornar ao menu principal\n");
    printf("Escolha uma opcao: ");
}

// Executa a lógica para "Consultar time" (Opção 1)
void executar_consulta_time(BDTimes *bd_times) {
    char prefixo[MAX_INPUT];
    
    printf("\n[Sistema]\n");
    printf("Digite o nome ou prefixo do time: ");
    ler_linha_stdin(prefixo, MAX_INPUT); 
    printf("\n[Sistema]\n"); 
    bd_times_consulta_por_prefixo(bd_times, prefixo);
}

// Executa a lógica para "Consultar partidas" (Opção 2)
void executar_consulta_partida(BDPartidas *bd_partidas, BDTimes *bd_times) {
    char opcao_str[MAX_INPUT]; 
    char prefixo[MAX_INPUT];
    int modo;

    while (1) { // Loop do sub-menu
        exibir_menu_consulta_partida();
        
        ler_linha_stdin(opcao_str, MAX_INPUT);
        char opcao_char = opcao_str[0]; 

        if (opcao_char == '4') {
            return; // Retorna ao menu principal
        }

        if (opcao_char < '1' || opcao_char > '3' || opcao_str[1] != '\0') {
            printf("[Sistema]\nOpcao invalida! Tente novamente.\n");
            continue;
        }

        modo = opcao_char - '0'; 

        printf("\n[Sistema]\n");
        printf("Digite o nome: ");
        ler_linha_stdin(prefixo, MAX_INPUT); 
        printf("\n[Sistema]\n");
        bd_partidas_consulta_por_time(bd_partidas, bd_times, prefixo, modo);

        printf("\n[Sistema]\nPressione Enter para realizar outra consulta de partida...");
        ler_linha_stdin(prefixo, MAX_INPUT); // Pausa
    }
}

// Executa a lógica para "Imprimir tabela" (Opção 6)
void executar_imprimir_tabela(BDTimes *bd_times) {
    printf("\n[Sistema]\n");
    printf("Imprimindo classificacao...\n");
    bd_times_imprime_classificacao(bd_times);
}

// Função Principal
int main() {
    // Inicializa os TADs
    BDTimes *bd_times = bd_times_cria();
    BDPartidas *bd_partidas = bd_partidas_cria();
    
    if (!bd_times || !bd_partidas) {
        fprintf(stderr, "[Sistema]\nErro critico: Nao foi possivel inicializar o sistema.\n");
        bd_times_libera(bd_times);
        bd_partidas_libera(bd_partidas);
        return 1;
    }

    // Carrega os dados dos arquivos CSV
    // As mensagens de sucesso/erro de carregamento são tratadas dentro das funções de TAD
    bd_times_carrega_arquivo(bd_times, ARQUIVO_TIMES);
    bd_partidas_carrega_arquivo(bd_partidas, bd_times, ARQUIVO_PARTIDAS);

    char opcao_str[MAX_INPUT]; 
    char dummy_buffer[MAX_INPUT]; 

    // Loop principal do sistema
    while (1) {
        exibir_menu_principal();
        
        ler_linha_stdin(opcao_str, MAX_INPUT);
        char opcao = opcao_str[0]; 

        // Validação da entrada do menu
        if (opcao_str[1] != '\0' || opcao_str[0] == '\0') {
             if (opcao != 'Q' && opcao != 'q') {
                opcao = ' '; // Força ser "Opção Inválida"
             }
        }

        switch (opcao) {
            case '1':
                executar_consulta_time(bd_times);
                break;
                
            case '2':
                executar_consulta_partida(bd_partidas, bd_times);
                continue; // Pula a pausa no final do loop

            case '6':
                executar_imprimir_tabela(bd_times);
                break;

            case '3':
            case '4':
            case '5':
                printf("[Sistema]\nOpcao desabilitada nesta fase do trabalho (Parte I).\n");
                break;

            case 'Q':
            case 'q':
                printf("[Sistema]\nEncerrando o sistema. Adeus!\n");
                bd_times_libera(bd_times);
                bd_partidas_libera(bd_partidas);
                return 0; // Encerra o programa

            default:
                printf("[Sistema]\nOpcao invalida! Tente novamente.\n");
                break;
        }
        
        // Pausa "Pressione Enter para continuar"
        printf("\n[Sistema]\nPressione Enter para continuar...");
        ler_linha_stdin(dummy_buffer, MAX_INPUT); 
    }

    // Limpeza final
    bd_times_libera(bd_times);
    bd_partidas_libera(bd_partidas);
    return 0;
}