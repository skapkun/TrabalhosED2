#ifndef ABB_H
#define ABB_H

#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAX_CHAVES 5 // Número máximo de chaves em um nó
#define MAX_CARACTERES 21 // Tamanho máximo de uma chave (20 + '\0')

 typedef struct No {
    int numChaves; 
    char chaves[MAX_CHAVES][MAX_CARACTERES];  
    struct No *filhos[MAX_CHAVES + 1];  
    int folha; // 1 se o nó é folha, 0 caso contrário
} No;

 typedef struct ArvoreB {
    No *raiz;
    int t;  
} ArvoreB;

 typedef struct {
    No *no;  
    int pos;  
    int ehInterno;  
    int ehRaiz;  
    int numFilhos;  
    char irmaoEsquerdo[MAX_CARACTERES];  
    char irmaoDireito[MAX_CARACTERES];  
    No *pai; 
} ResultadoBusca;

// Funções para a Árvore-B
ArvoreB* criarArvoreB(int ordem);
void destruirArvoreB(ArvoreB *arvore);
void inserir(ArvoreB *arvore, const char *chave);
void exibirArvoreB(No *no, int nivel, int ehPai);
ResultadoBusca buscar(No *no, const char *chave, No *pai, No *irmaoEsquerdo, No *irmaoDireito, int ehRaiz);
void carregarArquivo(ArvoreB *arvore, const char *nomeArquivo);
void inserirArquivo(ArvoreB *arvore, const char *nomeArquivo, const char *chave);
void emOrdem(No *raiz);

#endif  
