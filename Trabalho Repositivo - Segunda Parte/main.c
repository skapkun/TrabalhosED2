#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "grafoLoja.h"

 

 void exibirMenu() {
    printf("\n");
    printf(BOLD GREEN "╔═════════════════════════════════╗\n" RESET);
    printf(BOLD CYAN "║          MAPA DE ESTOQUE         ║\n" RESET);
    printf(BOLD GREEN "╠═════════════════════════════════╣\n" RESET);
    printf("║ 1. " BLUE "Carregar mapa de arquivo texto" RESET "  ║\n");
    printf("║ 2. " BLUE "Adicionar caminho entre seções" RESET "  ║\n");
    printf("║ 3. " BLUE "Imprimir mapa das seções" RESET "    ║\n");
    printf("║ 4. " BLUE "Encontrar o caminho mais curto" RESET " ║\n");
    printf("║ 5. " BLUE "Sair" RESET "                      ║\n");
    printf(BOLD GREEN "╚═════════════════════════════════╝\n" RESET);
    printf(CYAN "Escolha uma opção: " RESET);
}

 

 int main() {
    GrafoLoja grafo;
    int opcao, origem, destino, tempo;
    const char *nomeArquivo = "mapa.txt";

    system("chcp 65001");
    system("cls");

   

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                carregarMapa(&grafo, nomeArquivo);
                break;
            case 2:
                printf(BLUE "Digite a seção de origem: " RESET);
                scanf("%d", &origem);
                printf(BLUE "Digite a seção de destino: " RESET);
                scanf("%d", &destino);
                printf(BLUE "Digite o tempo de transito entre as seções: " RESET);
                scanf("%d", &tempo);
                adicionarCaminho(&grafo, origem, destino, tempo);
                salvarCaminhoNoArquivo(nomeArquivo, origem, destino, tempo);
                printf(GREEN "Caminho adicionado!\n" RESET);
                break;
            case 3:
                printf(BLUE "Exibindo o mapa das seções...\n" RESET);
                imprimirGrafo(&grafo);
                break;
            case 4:
                printf(BLUE "Digite a seção de origem: " RESET);
                scanf("%d", &origem);
                printf(BLUE "Digite a seção de destino: " RESET);
                scanf("%d", &destino);
                printf(YELLOW "\nCalculando o caminho mais curto...\n" RESET);
                caminhoMaisCurto(&grafo, origem, destino);
                break;
            case 5:
                liberarGrafo(&grafo);
                printf(CYAN "Fim de expediente! :D\n" RESET);
                break;
            default:
                printf(RED "Opção inválida. Tente novamente!\n" RESET);
                break;
        }
    } while (opcao != 5);
    
    return 0;
}
