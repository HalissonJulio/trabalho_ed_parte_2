#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bdtimes.h"
#include "bdpartidas.h"

#define MAX_INPUT 256 

void ler_linha_stdin(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

void exibir_menu_principal() {
    printf("\n============================================\n");
    printf("  Sistema de Gerenciamento de Partidas\n");
    printf("============================================\n");
    printf(" 1 - Consultar time\n");
    printf(" 2 - Consultar partidas\n");
    printf(" 3 - Atualizar partida\n");
    printf(" 4 - Remover partida\n");
    printf(" 5 - Inserir partida\n");
    printf(" 6 - Imprimir tabela de classificacao\n");
    printf(" Q - Sair\n");
    printf("--------------------------------------------\n");
    printf("[Sistema]\nEscolha uma opcao: ");
}

void exibir_menu_consulta_partida() {
    printf("\n[Sistema]\n");
    printf("Escolha o modo de consulta:\n");
    printf(" 1 - Por time mandante\n");
    printf(" 2 - Por time visitante\n");
    printf(" 3 - Por time mandante ou visitante\n");
    printf(" 4 - Retornar ao menu principal\n");
    printf("Escolha uma opcao: ");
}

void executar_consulta_time(BDTimes *bd_times) {
    char prefixo[MAX_INPUT];
    printf("\n[Sistema]\nDigite o nome ou prefixo do time: ");
    ler_linha_stdin(prefixo, MAX_INPUT); 
    printf("\n[Sistema]\n"); 
    bd_times_consulta_por_prefixo(bd_times, prefixo);
}

// PARAMETRO NOVO: 'manter_loop'
// Se 1: Fica no loop (para consulta pura).
// Se 0: Executa uma busca e retorna 1 imediatamente (para atualizar/remover).
int executar_consulta_partida(BDPartidas *bd_partidas, BDTimes *bd_times, int manter_loop) {
    char opcao_str[MAX_INPUT]; 
    char prefixo[MAX_INPUT];
    int modo;
    int realizou_busca = 0;

    while (1) { 
        exibir_menu_consulta_partida();
        ler_linha_stdin(opcao_str, MAX_INPUT);
        char opcao_char = opcao_str[0]; 

        // Se escolher 4, é SEMPRE Cancelar/Sair
        if (opcao_char == '4') {
            // Se estava em loop e buscou algo, retorna 1 (só informativo)
            // Se era pra atualizar (loop=0), retorna 0 para abortar.
            if (manter_loop) return realizou_busca;
            else return 0; 
        }

        if (opcao_char < '1' || opcao_char > '3' || opcao_str[1] != '\0') {
            printf("[Sistema]\nOpcao invalida! Tente novamente.\n");
            continue;
        }

        modo = opcao_char - '0'; 

        printf("\n[Sistema]\nDigite o nome: ");
        ler_linha_stdin(prefixo, MAX_INPUT); 
        printf("\n[Sistema]\n");
        bd_partidas_consulta_por_time(bd_partidas, bd_times, prefixo, modo);
        
        realizou_busca = 1;

        printf("\n[Sistema]\nPressione Enter para continuar...");
        ler_linha_stdin(prefixo, MAX_INPUT); 

        // SE NÃO É PARA MANTER O LOOP (Atualizar/Remover), SAI AGORA E VAI PRO ID
        if (!manter_loop) {
            return 1; // Retorna 1 indicando que a busca foi feita e deve prosseguir
        }
    }
}

int executar_inserir_partida(BDPartidas *bd_partidas, BDTimes *bd_times) {
    char buffer[MAX_INPUT];
    int id1, id2, g1, g2;

    printf("\n[Sistema]\nPara inserir um novo registro, digite os valores para\n");
    printf("os campos Time1, Time2, Placar1 e Placar2:\n");

    printf("\nID do Time 1: "); ler_linha_stdin(buffer, MAX_INPUT); id1 = atoi(buffer);
    printf("ID do Time 2: "); ler_linha_stdin(buffer, MAX_INPUT); id2 = atoi(buffer);
    printf("Gols Time 1: "); ler_linha_stdin(buffer, MAX_INPUT); g1 = atoi(buffer);
    printf("Gols Time 2: "); ler_linha_stdin(buffer, MAX_INPUT); g2 = atoi(buffer);

    printf("\n[Sistema]\nConfirma a insercao do registro abaixo? (S/N)\n");
    printf("Time1: %d | Time2: %d | Placar: %d x %d\n", id1, id2, g1, g2);
    
    char confirmacao[MAX_INPUT];
    ler_linha_stdin(confirmacao, MAX_INPUT);

    if (confirmacao[0] == 'S' || confirmacao[0] == 's') {
        if (bd_partidas_inserir(bd_partidas, bd_times, id1, id2, g1, g2)) {
            printf("\n[Sistema]\nO registro foi inserido com sucesso.\n");
        } else {
            printf("\n[Sistema]\nErro ao inserir registro (Verifique IDs dos times).\n");
        }
    } else {
        printf("\n[Sistema]\nOperacao cancelada.\n");
        return 0; 
    }
    return 1;
}

int executar_remover_partida(BDPartidas *bd_partidas, BDTimes *bd_times) {
    printf("\n[Sistema]\nPrimeiro, busque a partida para encontrar seu ID.\n");
    
    // Passa 0: Executa busca uma vez e sai. Se escolher 4, retorna 0.
    if (executar_consulta_partida(bd_partidas, bd_times, 0) == 0) {
        return 0; // Sai da função sem pedir ID
    }

    char buffer[MAX_INPUT];
    printf("\n[Sistema]\nDigite o ID do registro a ser removido (ou -1 para cancelar): ");
    ler_linha_stdin(buffer, MAX_INPUT);
    int id = atoi(buffer);

    if (id == -1) return 0; 

    Partida* p = bd_partidas_buscar_por_id(bd_partidas, id);
    if (!p) {
        printf("\n[Sistema]\nPartida com ID %d nao encontrada.\n", id);
        return 1; 
    }

    printf("\n[Sistema]\nTem certeza de que deseja excluir o registro %d? (S/N)\n", id);
    char confirmacao[MAX_INPUT];
    ler_linha_stdin(confirmacao, MAX_INPUT);

    if (confirmacao[0] == 'S' || confirmacao[0] == 's') {
        if (bd_partidas_remover(bd_partidas, bd_times, id)) {
            printf("\n[Sistema]\nRegistro removido com sucesso.\n");
        } else {
            printf("\n[Sistema]\nErro ao remover registro.\n");
        }
    } else {
        printf("\n[Sistema]\nOperacao cancelada.\n");
        return 0;
    }
    return 1;
}

int executar_atualizar_partida(BDPartidas *bd_partidas, BDTimes *bd_times) {
    printf("\n[Sistema]\nPrimeiro, busque a partida para encontrar seu ID.\n");
    
    // Passa 0: Executa busca uma vez e sai. Se escolher 4, retorna 0.
    if (executar_consulta_partida(bd_partidas, bd_times, 0) == 0) {
        return 0; // Sai da função sem pedir ID
    }

    char buffer[MAX_INPUT];
    printf("\n[Sistema]\nDigite o ID do registro a ser atualizado (ou -1 para cancelar): ");
    ler_linha_stdin(buffer, MAX_INPUT);
    int id = atoi(buffer);

    if (id == -1) return 0; 

    Partida* p = bd_partidas_buscar_por_id(bd_partidas, id);
    if (!p) {
        printf("\n[Sistema]\nPartida com ID %d nao encontrada.\n", id);
        return 1; 
    }

    int g1_atual = partida_obter_gols_time1(p);
    int g2_atual = partida_obter_gols_time2(p);
    int novo_g1, novo_g2;

    printf("\n[Sistema]\nDigite o novo valor para os gols do Time 1 (atual: %d): ", g1_atual);
    ler_linha_stdin(buffer, MAX_INPUT);
    if (buffer[0] == '-' || buffer[0] == '\0') novo_g1 = g1_atual;
    else novo_g1 = atoi(buffer);

    printf("Digite o novo valor para os gols do Time 2 (atual: %d): ", g2_atual);
    ler_linha_stdin(buffer, MAX_INPUT);
    if (buffer[0] == '-' || buffer[0] == '\0') novo_g2 = g2_atual;
    else novo_g2 = atoi(buffer);

    printf("\n[Sistema]\nConfirma os novos valores? (%d x %d) (S/N)\n", novo_g1, novo_g2);
    char confirmacao[MAX_INPUT];
    ler_linha_stdin(confirmacao, MAX_INPUT);

    if (confirmacao[0] == 'S' || confirmacao[0] == 's') {
        if (bd_partidas_atualizar(bd_partidas, bd_times, id, novo_g1, novo_g2)) {
            printf("\n[Sistema]\nRegistro atualizado com sucesso.\n");
        } else {
            printf("\n[Sistema]\nErro ao atualizar registro.\n");
        }
    } else {
        printf("\n[Sistema]\nOperacao cancelada.\n");
        return 0;
    }
    return 1;
}

void executar_imprimir_tabela(BDTimes *bd_times) {
    printf("\n[Sistema]\nImprimindo classificacao...\n");
    bd_times_imprime_classificacao(bd_times);
}

int main() {
    BDTimes *bd_times = bd_times_cria();
    BDPartidas *bd_partidas = bd_partidas_cria();
    
    if (!bd_times || !bd_partidas) {
        fprintf(stderr, "[Sistema]\nErro critico: Nao foi possivel inicializar o sistema.\n");
        return 1;
    }

    bd_times_carrega_arquivo(bd_times, ARQUIVO_TIMES);
    bd_partidas_carrega_arquivo(bd_partidas, bd_times, ARQUIVO_PARTIDAS);

    char opcao_str[MAX_INPUT]; 
    char dummy_buffer[MAX_INPUT]; 

    while (1) {
        exibir_menu_principal();
        ler_linha_stdin(opcao_str, MAX_INPUT);
        char opcao = opcao_str[0]; 

        if (opcao_str[1] != '\0' || opcao_str[0] == '\0') {
             if (opcao != 'Q' && opcao != 'q') opcao = ' '; 
        }

        int pausar = 1;

        switch (opcao) {
            case '1': 
                executar_consulta_time(bd_times); 
                break;

            case '2': 
                // Consulta pura: MANTÉM O LOOP (passa 1)
                executar_consulta_partida(bd_partidas, bd_times, 1); 
                pausar = 0; 
                break;

            case '3': 
                // Atualizar: SEM LOOP (passa 0). Buscou -> Vai pro ID. Cancelou -> Sai.
                if (executar_atualizar_partida(bd_partidas, bd_times) == 0) pausar = 0;
                break;

            case '4': 
                // Remover: SEM LOOP (passa 0).
                if (executar_remover_partida(bd_partidas, bd_times) == 0) pausar = 0;
                break;

            case '5': 
                if (executar_inserir_partida(bd_partidas, bd_times) == 0) pausar = 0;
                break;

            case '6': 
                executar_imprimir_tabela(bd_times); 
                break;

            case 'Q':
            case 'q':
                printf("[Sistema]\nEncerrando o sistema. Adeus!\n");
                bd_times_libera(bd_times);
                bd_partidas_libera(bd_partidas);
                return 0;

            default:
                printf("[Sistema]\nOpcao invalida! Tente novamente.\n");
                break;
        }
        
        if (pausar) {
            printf("\n[Sistema]\nPressione Enter para continuar...");
            ler_linha_stdin(dummy_buffer, MAX_INPUT); 
        }
    }
    return 0;
}