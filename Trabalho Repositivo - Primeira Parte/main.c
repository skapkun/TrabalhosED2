#include "grafo.h"
#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>

 

void exibirMenu() {
    printf("\n");
    printf(BOLD GREEN "=======================================\n" RESET);
    printf(BOLD CYAN "                GRAFOS                  \n" RESET);
    printf(BOLD GREEN "=======================================\n" RESET);
    printf("1. " BLUE "Gerar grafo em arquivo texto" RESET "\n");
    printf("2. " BLUE "Carregar grafo de um arquivo texto" RESET "\n");
    printf("3. " BLUE "Imprimir grafo" RESET "\n");
    printf("4. " BLUE "Busca em Largura (BFS)" RESET "\n");
    printf("5. " BLUE "Busca em Profundidade (DFS)" RESET "\n");
    printf("6. " BLUE "Árvore Geradora Mínima (Prim)" RESET "\n");
    printf("7. " BLUE "Caminhos Mínimos – Busca em Largura (BFS)" RESET "\n");
    printf("8. " BLUE "Caminhos Mínimos – Dijkstra" RESET "\n");
    printf("9. " BLUE "Sair" RESET "\n");
    printf(BOLD GREEN "=======================================\n" RESET);
    printf(CYAN"Escolha uma opção: "RESET);
}

int main() {
    system("chcp 65001");
    system ("cls");
    
    Grafo *grafo = NULL;
    int opcao, verticeOrigem, numVertices, numArestas;
    const char *nomeArquivo = "grafo_1.txt";

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf(BLUE"Digite o número de vértices: "RESET);
                scanf("%d", &numVertices);
                printf(BLUE"Digite o número de arestas: "RESET);
                scanf("%d", &numArestas);
                gerarGrafoParaArquivo(nomeArquivo, numVertices, numArestas);
                printf(GREEN "Grafo salvo no arquivo '%s' :D.\n" RESET, nomeArquivo);
                break;
            case 2:
                grafo = carregarGrafo(nomeArquivo);
                if (grafo != NULL) {
                    printf(GREEN "Grafo carregado com sucesso.\n" RESET);
                } else {
                    printf(RED "Erro ao carregar o grafo.\n" RESET);
                }
                break;
            case 3:
                if (grafo != NULL) {
                    imprimirGrafo(grafo);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 4:
                if (grafo != NULL) {
                    printf(BLUE"Entre com o vértice de origem: "RESET);
                    scanf("%d", &verticeOrigem);
                    buscaEmLargura(grafo, verticeOrigem);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 5:
                if (grafo != NULL) {
                    printf(BLUE"Entre com o vértice de origem: "RESET);
                    scanf("%d", &verticeOrigem);
                    buscaEmProfundidade(grafo, verticeOrigem);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 6:
                if (grafo != NULL) {
                     arvoreGeradoraMinimaPrim(grafo);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 7:
                if (grafo != NULL) {
                    printf(BLUE"Digite o vértice de origem: "RESET);
                    scanf("%d", &verticeOrigem);
                    caminhosMinimosBFS(grafo, verticeOrigem);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 8:
                if (grafo != NULL) {
                    printf(BLUE"Digite o vértice de origem: "RESET);
                    scanf("%d", &verticeOrigem);
                    caminhosMinimosDijkstra(grafo, verticeOrigem);
                } else {
                    printf(RED "Grafo não carregado.\n" RESET);
                }
                break;
            case 9:
                printf(CYAN "Fim! :D \n" RESET);
                break;
            default:
                printf(RED "Opção inválida. Tente novamente! \n" RESET);
                break;
        }
    } while (opcao != 9);

    if (grafo != NULL) {
        destruirGrafo(grafo);
    }

    return 0;
}
