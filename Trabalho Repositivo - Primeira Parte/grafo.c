#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

void gerarGrafoParaArquivo(const char *nomeArquivo, int numeroVertices, int numeroArestas) {
    if (numeroArestas < numeroVertices - 1 || numeroArestas > numeroVertices * (numeroVertices - 1) / 2) {
        printf("Número de arestas inválido para o número de vértices.\n");
        return;
    }

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        perror(RED "Erro ao abrir o arquivo" RESET);
        return;
    }

    fprintf(arquivo, "%d\n", numeroVertices);

    srand((unsigned int)time(NULL));

    int *vertices = (int *)malloc(numeroVertices * sizeof(int));
    if (vertices == NULL) {
        perror(RED "Erro ao alocar memória para os vértices" RESET);
        fclose(arquivo);
        return;
    }

     for (int i = 0; i < numeroVertices; i++) {
        vertices[i] = i;
    }

     for (int i = 0; i < numeroVertices; i++) {
        int indiceAleatorio = rand() % numeroVertices;
        int temp = vertices[i];
        vertices[i] = vertices[indiceAleatorio];
        vertices[indiceAleatorio] = temp;
    }

    int *arestas = (int *)calloc(numeroVertices * numeroVertices, sizeof(int));
    if (arestas == NULL) {
        perror(RED "Erro ao alocar memória para as arestas" RESET);
        free(vertices);
        fclose(arquivo);
        return;
    }

    int arestasGeradas = 0;

     for (int i = 1; i < numeroVertices; i++) {
        int verticeAdjacente = rand() % i;
        float pesoAresta = (float)(rand() % 1000 + 1) / 100;  
        fprintf(arquivo, "%d %d %.2f\n", vertices[i], vertices[verticeAdjacente], pesoAresta);
        arestas[vertices[i] * numeroVertices + vertices[verticeAdjacente]] = 1;
        arestas[vertices[verticeAdjacente] * numeroVertices + vertices[i]] = 1;
        arestasGeradas++;
    }

     while (arestasGeradas < numeroArestas) {
        int verticeOrigem = rand() % numeroVertices;
        int verticeDestino = rand() % numeroVertices;
        if (verticeOrigem != verticeDestino && !arestas[vertices[verticeOrigem] * numeroVertices + vertices[verticeDestino]] && !arestas[vertices[verticeDestino] * numeroVertices + vertices[verticeOrigem]]) {
            float pesoAresta = (float)(rand() % 1000 + 1) / 100; 
            fprintf(arquivo, "%d %d %.2f\n", vertices[verticeOrigem], vertices[verticeDestino], pesoAresta);
            arestas[vertices[verticeOrigem] * numeroVertices + vertices[verticeDestino]] = 1;
            arestas[vertices[verticeDestino] * numeroVertices + vertices[verticeOrigem]] = 1;
            arestasGeradas++;
        }
    }

    free(arestas);
    free(vertices);
    fclose(arquivo);
    printf(GREEN "Arquivo %s criado :D\n" RESET, nomeArquivo);
}



Grafo* criarGrafo(int numVertices) {
    Grafo *grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->listaAdjacencia = (NoAdjacencia**)malloc(numVertices * sizeof(NoAdjacencia*));

    for (int i = 0; i < numVertices; i++) {
        grafo->listaAdjacencia[i] = NULL;
    }

    return grafo;
}
 
 void adicionarAresta(Grafo *grafo, int vertice1, int vertice2, float peso) {
    NoAdjacencia *novoNo = (NoAdjacencia*)malloc(sizeof(NoAdjacencia));
    novoNo->vertice = vertice2;
    novoNo->peso = peso;
    novoNo->proximo = grafo->listaAdjacencia[vertice1];
    grafo->listaAdjacencia[vertice1] = novoNo;

     novoNo = (NoAdjacencia*)malloc(sizeof(NoAdjacencia));
    novoNo->vertice = vertice1;
    novoNo->peso = peso;
    novoNo->proximo = grafo->listaAdjacencia[vertice2];
    grafo->listaAdjacencia[vertice2] = novoNo;
}

Grafo* carregarGrafo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror(RED"Erro ao abrir o arquivo"RESET);
        return NULL;
    }

    int numVertices;
    fscanf(arquivo, "%d", &numVertices);

    Grafo *grafo = criarGrafo(numVertices);
    if (grafo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    int vertice1, vertice2;
    float peso;
    while (fscanf(arquivo, "%d %d %f", &vertice1, &vertice2, &peso) != EOF) {
        adicionarAresta(grafo, vertice1, vertice2, peso);
    }

    fclose(arquivo);
    return grafo;
}


void destruirGrafo(Grafo *grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        NoAdjacencia *adj = grafo->listaAdjacencia[i];
        while (adj != NULL) {
            NoAdjacencia *temp = adj;
            adj = adj->proximo;
            free(temp);
        }
    }
    free(grafo->listaAdjacencia);
    free(grafo);
}
 
 

Fila* criarFila(int capacidade) {
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->capacidade = capacidade;
    fila->dados = (int *)malloc(capacidade * sizeof(int));
    fila->frente = 0;
    fila->tras = -1;
    return fila;
}

bool estaVazia(Fila *fila) {
    return fila->frente > fila->tras;
}

void enfileirar(Fila *fila, int valor) {
    fila->dados[++fila->tras] = valor;
}

int desenfileirar(Fila *fila) {
    return fila->dados[fila->frente++];
}

void destruirFila(Fila *fila) {
    free(fila->dados);
    free(fila);
}

 
void reconstruirCaminho(int* predecessores, int vertice, int verticeInicial) {
    if (vertice == verticeInicial) {
        printf(GREEN "%d" RESET, vertice);
        return;
    }
    if (predecessores[vertice] == -1) {
        printf("Não alcançável");
        return;
    }
    reconstruirCaminho(predecessores, predecessores[vertice], verticeInicial);
    printf(" -> " GREEN "%d" RESET, vertice);
}


 

void imprimirVizinhos(NoAdjacencia *adj) {
    printf(YELLOW "Vizinhos: " RESET);
    while (adj != NULL) {
        printf("%d ", adj->vertice);
        adj = adj->proximo;
    }
    printf("\n");
}

void buscaEmLargura(Grafo *grafo, int verticeInicial) {
    int *visitado = (int*)calloc(grafo->numVertices, sizeof(int));
    Fila *fila = criarFila(grafo->numVertices);
    int *ordemVisita = (int*)malloc(grafo->numVertices * sizeof(int));
    int indiceOrdem = 0;

    visitado[verticeInicial] = 1;
    enfileirar(fila, verticeInicial);

    printf(BOLD CYAN "Início em %d:\n" RESET, verticeInicial);

    while (!estaVazia(fila)) {
        int vertice = desenfileirar(fila);
        ordemVisita[indiceOrdem++] = vertice; 
        printf(CYAN "Visitando %d\n" RESET, vertice);
 
        NoAdjacencia *adj = grafo->listaAdjacencia[vertice];
        if (adj != NULL) {
            imprimirVizinhos(adj);
        }
 
        adj = grafo->listaAdjacencia[vertice];
        while (adj != NULL) {
            int adjacente = adj->vertice;
            if (!visitado[adjacente]) {
                visitado[adjacente] = 1;
                enfileirar(fila, adjacente);
            }
            adj = adj->proximo;
        }
    }
 
    printf(BOLD YELLOW "Ordem de visita: " RESET);
    for (int i = 0; i < indiceOrdem; i++) {
        printf("%d ", ordemVisita[i]);
    }
    printf("\n");

    destruirFila(fila);
    free(visitado);
    free(ordemVisita);
}
 
void dfsRecursivo(Grafo *grafo, int vertice, int *visitado, int *ordemVisitados, int *indice) {
    visitado[vertice] = 1;
    printf(CYAN "Visitando %d\n" RESET, vertice);
    ordemVisitados[*indice] = vertice;
    (*indice)++;

    NoAdjacencia *adj = grafo->listaAdjacencia[vertice];
    if (adj != NULL) {
        imprimirVizinhos(adj);
    }

    adj = grafo->listaAdjacencia[vertice];
    while (adj != NULL) {
        int adjacente = adj->vertice;
        if (!visitado[adjacente]) { 
            dfsRecursivo(grafo, adjacente, visitado, ordemVisitados, indice);
        }
        adj = adj->proximo;
    }
}

void buscaEmProfundidade(Grafo *grafo, int verticeInicial) {
    int *visitado = (int*)calloc(grafo->numVertices, sizeof(int));
    int *ordemVisitados = (int*)malloc(grafo->numVertices * sizeof(int));
    int indice = 0;

    printf(BOLD CYAN "Início em %d:\n" RESET, verticeInicial);
    dfsRecursivo(grafo, verticeInicial, visitado, ordemVisitados, &indice);

    printf(BOLD YELLOW "Ordem de visita: " RESET);
    for (int i = 0; i < indice; i++) {
        printf("%d ", ordemVisitados[i]);
    }
    printf("\n");

    free(visitado);
    free(ordemVisitados);
}

void caminhosMinimosBFS(Grafo *grafo, int verticeOrigem) {
    if (verticeOrigem < 0 || verticeOrigem >= grafo->numVertices) {
        printf(RED BOLD "Vértice de origem inválido.\n" RESET);
        return;
    }

    int *distancias = malloc(grafo->numVertices * sizeof(int));
    int *visitados = malloc(grafo->numVertices * sizeof(int));
    int *predecessores = malloc(grafo->numVertices * sizeof(int));

    if (distancias == NULL || visitados == NULL || predecessores == NULL) {
        perror(RED "Erro ao alocar memória" RESET);
        free(distancias);
        free(visitados);
        free(predecessores);
        return;
    }
    
    for (int i = 0; i < grafo->numVertices; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
        predecessores[i] = -1;
    }
    
    distancias[verticeOrigem] = 0;

    int *fila = malloc(grafo->numVertices * sizeof(int));
    int inicio = 0, fim = 0;

    if (fila == NULL) {
        perror(RED "Erro ao alocar memória para a fila" RESET);
        free(distancias);
        free(visitados);
        free(predecessores);
        return;
    }

    fila[fim++] = verticeOrigem;
    visitados[verticeOrigem] = 1;

    while (inicio < fim) {
        int verticeAtual = fila[inicio++];
        
        NoAdjacencia *adjacente = grafo->listaAdjacencia[verticeAtual];
        while (adjacente != NULL) {
            int verticeAdjacente = adjacente->vertice;
            if (!visitados[verticeAdjacente]) {
                distancias[verticeAdjacente] = distancias[verticeAtual] + 1;
                visitados[verticeAdjacente] = 1;
                predecessores[verticeAdjacente] = verticeAtual;
                fila[fim++] = verticeAdjacente;
            }
            adjacente = adjacente->proximo;
        }
    }

    printf(BOLD CYAN "Caminhos mínimos a partir do vértice %d:\n" RESET, verticeOrigem);
    for (int i = 0; i < grafo->numVertices; i++) {
        printf(BOLD BLUE "Para o vértice " GREEN "%d" BLUE ": "RESET,   i);
        if (distancias[i] == INT_MAX) {
            printf(YELLOW "Inalcançável\n" RESET);
        } else {
            printf(YELLOW"\nCaminho: "RESET);
            reconstruirCaminho(predecessores, i, verticeOrigem);
            printf(BOLD YELLOW " Distância: " YELLOW "%d" BLUE "\n" RESET, distancias[i]);
        }
    }

    free(distancias);
    free(visitados);
    free(predecessores);
    free(fila);
}

 

void imprimirGrafo(Grafo *grafo) {
    printf(BOLD YELLOW "Estrutura do Grafo:\n" RESET);
    for (int i = 0; i < grafo->numVertices; i++) {
        printf(BOLD BLUE " %d" RESET ":", i);
        NoAdjacencia *adj = grafo->listaAdjacencia[i];
        int count = 0;
        while (adj != NULL) {
          
            printf(CYAN " -> %d" RESET YELLOW "(peso: %d)" RESET, adj->vertice, adj->peso);
            adj = adj->proximo;
             
        }
        printf("\n");
    }
}

 
void arvoreGeradoraMinimaPrim(Grafo *grafo) {
    int *custosMinimos = (int *)malloc(grafo->numVertices * sizeof(int));
    int *pais = (int *)malloc(grafo->numVertices * sizeof(int));
    bool *estaNaMST = (bool *)malloc(grafo->numVertices * sizeof(bool));
    int totalCusto = 0;

    if (custosMinimos == NULL || pais == NULL || estaNaMST == NULL) {
        printf(RED"Erro ao alocar memória.\n"RESET);
        return;
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        custosMinimos[i] = INT_MAX;
        estaNaMST[i] = false;
        pais[i] = -1;
    }

    custosMinimos[0] = 0;

    for (int contador = 0; contador < grafo->numVertices - 1; contador++) {
        int verticeMinimo = -1;
        for (int vertice = 0; vertice < grafo->numVertices; vertice++) {
            if (!estaNaMST[vertice] && (verticeMinimo == -1 || custosMinimos[vertice] < custosMinimos[verticeMinimo])) {
                verticeMinimo = vertice;
            }
        }

        if (verticeMinimo == -1) {
            printf(RED"Erro: Grafo desconexo.\n"RESET);
            free(custosMinimos);
            free(pais);
            free(estaNaMST);
            return;
        }

        estaNaMST[verticeMinimo] = true;

        NoAdjacencia *adjacente = grafo->listaAdjacencia[verticeMinimo];
        while (adjacente != NULL) {
            int verticeAdjacente = adjacente->vertice;
            int pesoAresta = adjacente->peso;
            if (!estaNaMST[verticeAdjacente] && pesoAresta < custosMinimos[verticeAdjacente]) {
                custosMinimos[verticeAdjacente] = pesoAresta;
                pais[verticeAdjacente] = verticeMinimo;
            }
            adjacente = adjacente->proximo;
        }
    }

    for (int i = 1; i < grafo->numVertices; i++) {
        printf(CYAN "(%d, %d)" RESET " - " YELLOW "Custo:"RESET GREEN" %d\n" RESET, pais[i], i, custosMinimos[i]);
        totalCusto += custosMinimos[i];
    }

    printf(BOLD YELLOW "Custo total da Árvore Geradora Mínima: %d\n" RESET, totalCusto);

    free(custosMinimos);
    free(pais);
    free(estaNaMST);
}

  
void caminhosMinimosDijkstra(Grafo *grafo, int verticeInicial) {
    int *distancias = (int *)malloc(grafo->numVertices * sizeof(int));
    bool *visitados = (bool *)malloc(grafo->numVertices * sizeof(bool));
    int *predecessores = (int *)malloc(grafo->numVertices * sizeof(int));

    for (int i = 0; i < grafo->numVertices; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = false;
        predecessores[i] = -1;  
    }

    distancias[verticeInicial] = 0;

    for (int contador = 0; contador < grafo->numVertices - 1; contador++) {
        int verticeMaisProximo = -1;
        for (int vertice = 0; vertice < grafo->numVertices; vertice++) {
            if (!visitados[vertice] && (verticeMaisProximo == -1 || distancias[vertice] < distancias[verticeMaisProximo])) {
                verticeMaisProximo = vertice;
            }
        }

        visitados[verticeMaisProximo] = true;

        NoAdjacencia *adjacente = grafo->listaAdjacencia[verticeMaisProximo];
        while (adjacente != NULL) {
            int verticeAdjacente = adjacente->vertice;
            int pesoAresta = adjacente->peso;
            if (!visitados[verticeAdjacente] && distancias[verticeMaisProximo] != INT_MAX && distancias[verticeMaisProximo] + pesoAresta < distancias[verticeAdjacente]) {
                distancias[verticeAdjacente] = distancias[verticeMaisProximo] + pesoAresta;
                predecessores[verticeAdjacente] = verticeMaisProximo;  
            }
            adjacente = adjacente->proximo;
        }
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        printf(BOLD BLUE" Para o vértice " GREEN "%d" BLUE ": " YELLOW, i);
        if (distancias[i] == INT_MAX) {
            printf("Infinito\n" RESET);
        } else {
            printf("Distância: %0.0f\n" RESET, (float)distancias[i] * 1.0);  
        }
        printf(YELLOW"Caminho: "RESET);
        if (distancias[i] < INT_MAX) {
            reconstruirCaminho(predecessores, i, verticeInicial);
        }
        printf("\n");
    }

    free(distancias);
    free(visitados);
    free(predecessores);
}