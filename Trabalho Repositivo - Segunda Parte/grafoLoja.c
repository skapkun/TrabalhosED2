#include <stdio.h>
#include <limits.h>
#include <stdlib.h> 
#include <stdbool.h>
#include "grafoLoja.h"

void inicializarGrafo(GrafoLoja *grafo, int numSecoes) {
    grafo->numSecoes = numSecoes;
    grafo->adj = malloc(numSecoes * sizeof(int *)); 
    if (grafo->adj == NULL) {
        perror("Erro ao alocar memória para a matriz de adjacência");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numSecoes; i++) {
        grafo->adj[i] = malloc(numSecoes * sizeof(int)); 

        if (grafo->adj[i] == NULL) {
            perror("Erro ao alocar memória para uma linha da matriz de adjacência");
             for (int j = 0; j < i; j++) {
                free(grafo->adj[j]);
            }
            free(grafo->adj);
            exit(EXIT_FAILURE);
        }

         for (int j = 0; j < numSecoes; j++) {
            grafo->adj[i][j] = (i == j) ? 0 : INT_MAX;  
        }
    }
}

void liberarGrafo(GrafoLoja *grafo) {
    for (int i = 0; i < grafo->numSecoes; i++) {
        free(grafo->adj[i]);
    }
    free(grafo->adj);
}

void carregarMapa(GrafoLoja *grafo, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int numSecoes;
    if (fscanf(arquivo, "%d", &numSecoes) != 1) {
        printf(RED "Erro ao ler o número de seções do arquivo.\n" RESET);
        fclose(arquivo);
        return;
    }

     if (grafo->adj != NULL) {
        liberarGrafo(grafo);
    }
 
     inicializarGrafo(grafo, numSecoes);

    int origem, destino, tempo;
    while (fscanf(arquivo, "%d %d %d", &origem, &destino, &tempo) == 3) {
        adicionarCaminho(grafo, origem, destino, tempo);
    }

    fclose(arquivo);
    printf(GREEN "Mapa carregado com sucesso!\n" RESET, nomeArquivo);
}


 void salvarCaminhoNoArquivo(const char *nomeArquivo, int origem, int destino, int tempo) {
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf(RED "Erro ao abrir o arquivo %s.\n" RESET, nomeArquivo);
        return;
    }
    
    fprintf(arquivo, "%d %d %d\n", origem, destino, tempo);
    fclose(arquivo);
}

 

 void adicionarCaminho(GrafoLoja *grafo, int origem, int destino, int peso) {
    grafo->adj[origem][destino] = peso;
}
 

void imprimirGrafo(GrafoLoja *grafo) {
    int numVertices = grafo->numSecoes;
    printf(CYAN "Mapa de Seções:\n" RESET);
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (grafo->adj[i][j] == INT_MAX) {
                 printf(RED "%4s" RESET, "X");
            } else {
                 printf(BLUE "%4d" RESET, grafo->adj[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

 int minDistancia(int dist[], bool sptSet[], int numVertices) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < numVertices; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(GrafoLoja *grafo, int origem, int dist[], int antecessor[]) {
    int numVertices = grafo->numSecoes;
    bool sptSet[numVertices];
    
    // Inicializa dist e sptSet
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        antecessor[i] = -1;
    }
    
    dist[origem] = 0;
    
    for (int count = 0; count < numVertices - 1; count++) {
        int u = minDistancia(dist, sptSet, numVertices);
        sptSet[u] = true;
        
        for (int v = 0; v < numVertices; v++) {
            if (!sptSet[v] && grafo->adj[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + grafo->adj[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo->adj[u][v];
                antecessor[v] = u;
            }
        }
    }
}

 void caminhoMaisCurto(GrafoLoja *grafo, int origem, int destino) {
    int dist[MAX_SECOES];
    int antecessor[MAX_SECOES];
    
    dijkstra(grafo, origem, dist, antecessor);
    
     if (dist[destino] == INT_MAX) {
        printf(RED "Não há caminho acessível do vértice %d para o vértice %d.\n" RESET, origem, destino);
        return;
    }
    
     printf(GREEN "Distância mais curta da seção %d para seção %d é %d\n" RESET, origem, destino, dist[destino]);
    
     printf(BLUE "\nPor onde passar: " RESET);
    int caminho[MAX_SECOES];
    int index = 0;
    
    for (int v = destino; v != -1; v = antecessor[v]) {
        if (dist[v] != INT_MAX) { 
            caminho[index++] = v;
        }
    }
    
     for (int i = index - 1; i >= 0; i--) {
        printf(YELLOW"%d"RESET, caminho[i]);
        if (i > 0) printf(CYAN" -> "RESET);
    }
    printf("\n");
    
     printf(YELLOW "\nDistâncias atuais:\n" RESET);
    bool encontrouCaminho = false;
    for (int i = 0; i < grafo->numSecoes; i++) {
        if (dist[i] != INT_MAX) {
            printf(BOLD YELLOW"Distância para o seção %d: %d\n"RESET, i, dist[i]);
            encontrouCaminho = true;
        }
    }
    if (!encontrouCaminho) {
        printf(RED "Não há caminhos acessíveis a partir do vértice %d.\n" RESET, origem);
    }
    
    printf("\n");
}