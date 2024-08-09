#ifndef GRAFOLOJA_H
#define GRAFOLOJA_H
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
 #define MAX_SECOES 100 

 typedef struct {
    int **adj;  
    int numSecoes;  
} GrafoLoja;

 
void inicializarGrafo(GrafoLoja *grafo, int numSecoes);
void adicionarCaminho(GrafoLoja *grafo, int origem, int destino, int tempo);
void imprimirGrafo(GrafoLoja *grafo);
void dijkstra(GrafoLoja *grafo, int origem, int dist[], int antecessor[]);
void caminhoMaisCurto(GrafoLoja *grafo, int origem, int destino);
void carregarMapa(GrafoLoja *grafo, const char *nomeArquivo);
void salvarCaminhoNoArquivo(const char *nomeArquivo, int origem, int destino, int tempo);
void liberarGrafo(GrafoLoja *grafo);
#endif
