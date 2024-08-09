#ifndef GRAFO_H
#define GRAFO_H
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"

typedef struct NoAdjacencia {
    int vertice;
    int peso;  
    struct NoAdjacencia *proximo;
} NoAdjacencia;

typedef struct Fila {
    int *dados;
    int frente, tras, capacidade;
} Fila;

typedef struct Grafo {
    int numVertices;
    NoAdjacencia **listaAdjacencia;
} Grafo;

Grafo* carregarGrafo(const char *nomeArquivo);
void imprimirGrafo(Grafo *grafo);
void destruirGrafo(Grafo *grafo);
void buscaEmLargura(Grafo *grafo, int verticeInicial);
void buscaEmProfundidade(Grafo *grafo, int verticeInicial);
void arvoreGeradoraMinimaPrim(Grafo *grafo);
void caminhosMinimosBFS(Grafo *grafo, int verticeInicial);
void caminhosMinimosDijkstra(Grafo *grafo, int verticeInicial);
void gerarGrafoParaArquivo(const char *nomeArquivo, int numVertices, int numArestas);
#endif
