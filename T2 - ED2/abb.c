#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"

No* criarNo(int ordem, int folha) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->numChaves = 0;
    novoNo->folha = folha;
    for (int i = 0; i < 2 * ordem; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

ArvoreB* criarArvoreB(int ordem) {
    ArvoreB *novaArvore = (ArvoreB*)malloc(sizeof(ArvoreB));
    novaArvore->t = ordem;
    novaArvore->raiz = criarNo(ordem, 1);
    return novaArvore;
}

void destruirNo(No *no) {
    if (no != NULL) {
        for (int i = 0; i <= no->numChaves; i++) {
            destruirNo(no->filhos[i]);
        }
        free(no);
    }
}

void destruirArvoreB(ArvoreB *arvore) {
    destruirNo(arvore->raiz);
    free(arvore);
}

void dividirFilho(No *pai, int indiceFilho, int ordem) {
    No *filhoEsquerdo = pai->filhos[indiceFilho];
    No *filhoDireito = criarNo(ordem, filhoEsquerdo->folha);
    filhoDireito->numChaves = ordem - 1;

    for (int indiceChave = 0; indiceChave < ordem - 1; indiceChave++) {
        strcpy(filhoDireito->chaves[indiceChave], filhoEsquerdo->chaves[indiceChave + ordem]);
    }

    if (!filhoEsquerdo->folha) {
        for (int indiceFilhoEsquerdo = 0; indiceFilhoEsquerdo < ordem; indiceFilhoEsquerdo++) {
            filhoDireito->filhos[indiceFilhoEsquerdo] = filhoEsquerdo->filhos[indiceFilhoEsquerdo + ordem];
        }
    }

    filhoEsquerdo->numChaves = ordem - 1;

    for (int indicePaiFilho = pai->numChaves; indicePaiFilho >= indiceFilho + 1; indicePaiFilho--) {
        pai->filhos[indicePaiFilho + 1] = pai->filhos[indicePaiFilho];
    }
    pai->filhos[indiceFilho + 1] = filhoDireito;

    for (int indicePaiChave = pai->numChaves - 1; indicePaiChave >= indiceFilho; indicePaiChave--) {
        strcpy(pai->chaves[indicePaiChave + 1], pai->chaves[indicePaiChave]);
    }

    strcpy(pai->chaves[indiceFilho], filhoEsquerdo->chaves[ordem - 1]);
    pai->numChaves++;

    printf(BLUE "Dividido o filho %d do nó pai.\n" RESET, indiceFilho);
}

void inserirNaoCheio(No *noAtual, const char *chave, int ordem) {
    int indiceChaveAtual = noAtual->numChaves - 1;

    if (noAtual->folha) {
        while (indiceChaveAtual >= 0 && strcmp(chave, noAtual->chaves[indiceChaveAtual]) < 0) {
            strcpy(noAtual->chaves[indiceChaveAtual + 1], noAtual->chaves[indiceChaveAtual]);
            indiceChaveAtual--;
        }
        strcpy(noAtual->chaves[indiceChaveAtual + 1], chave);
        noAtual->numChaves++;
        printf(GREEN "Chave '%s' inserida no nó folha.\n" RESET, chave);
    } else {
        while (indiceChaveAtual >= 0 && strcmp(chave, noAtual->chaves[indiceChaveAtual]) < 0) {
            indiceChaveAtual--;
        }
        indiceChaveAtual++;
        if (noAtual->filhos[indiceChaveAtual]->numChaves == 2 * ordem - 1) {
            dividirFilho(noAtual, indiceChaveAtual, ordem);
            if (strcmp(chave, noAtual->chaves[indiceChaveAtual]) > 0) {
                indiceChaveAtual++;
            }
        }
        inserirNaoCheio(noAtual->filhos[indiceChaveAtual], chave, ordem);
    }
}

void inserir(ArvoreB *arvore, const char *chave) {
    No *raiz = arvore->raiz;
    if (raiz->numChaves == 2 * arvore->t - 1) {
        No *novaRaiz = criarNo(arvore->t, 0);
        arvore->raiz = novaRaiz;
        novaRaiz->filhos[0] = raiz;
        dividirFilho(novaRaiz, 0, arvore->t);
        inserirNaoCheio(novaRaiz, chave, arvore->t);
    } else {
        inserirNaoCheio(raiz, chave, arvore->t);
    }

    printf(BLUE "Árvore-B após inserção:\n" RESET);
    exibirArvoreB(arvore->raiz, 0, 1);
}

void exibirArvoreB(No *no, int nivel, int ehPai) {
    if (no != NULL) {
        int indiceExibir;
        for (indiceExibir = 0; indiceExibir < no->numChaves; indiceExibir++) {
            if (!no->folha) {
                exibirArvoreB(no->filhos[indiceExibir], nivel + 1, 0);
            }
            for (int nivelExibir = 0; nivelExibir < nivel; nivelExibir++) {
                printf("    ");
            }
            if (ehPai) {
                printf(BLUE "%s" RESET "\n", no->chaves[indiceExibir]);
            } else if (!no->folha) {
                printf(GREEN "%s" RESET "\n", no->chaves[indiceExibir]);
            } else {
                printf(YELLOW "%s" RESET "\n", no->chaves[indiceExibir]);
            }
        }
        if (!no->folha) {
            exibirArvoreB(no->filhos[indiceExibir], nivel + 1, 0);
        }
    }
}

ResultadoBusca buscar(No *no, const char *chave, No *pai, No *irmaoEsquerdo, No *irmaoDireito, int ehRaiz) {
    ResultadoBusca resultado = {NULL, -1, 0, 0, 0, "", "", NULL}; 

    int indiceBuscar = 0;
    while (indiceBuscar < no->numChaves && strcmp(chave, no->chaves[indiceBuscar]) > 0) {
        indiceBuscar++;
    }
    
    if (indiceBuscar < no->numChaves && strcmp(chave, no->chaves[indiceBuscar]) == 0) {
        resultado.no = no;
        resultado.pos = indiceBuscar;
        resultado.ehInterno = !no->folha;
        resultado.ehRaiz = ehRaiz;
        resultado.numFilhos = no->folha ? 0 : no->numChaves + 1;
        resultado.pai = pai;
        
        if (irmaoEsquerdo != NULL && irmaoEsquerdo->numChaves > 0) {
            strcpy(resultado.irmaoEsquerdo, irmaoEsquerdo->chaves[irmaoEsquerdo->numChaves - 1]);
        } else {
            resultado.irmaoEsquerdo[0] = '\0';
        }
        
        if (irmaoDireito != NULL && irmaoDireito->numChaves > 0) {
            strcpy(resultado.irmaoDireito, irmaoDireito->chaves[0]);
        } else {
            resultado.irmaoDireito[0] = '\0';
        }
        
        return resultado;
    }
    
    if (no->folha) {
        return resultado; 
    } else {
        int indiceFilhoBuscar = indiceBuscar;
        No *filhoBuscar = no->filhos[indiceFilhoBuscar];
        No *novoIrmaoEsquerdo = (indiceFilhoBuscar > 0) ? no->filhos[indiceFilhoBuscar - 1] : NULL;
        No *novoIrmaoDireito = (indiceFilhoBuscar < no->numChaves) ? no->filhos[indiceFilhoBuscar + 1] : NULL;
        return buscar(filhoBuscar, chave, no, novoIrmaoEsquerdo, novoIrmaoDireito, 0);
    }
}

void carregarArquivo(ArvoreB *arvore, const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        perror(RED "Erro ao abrir o arquivo" RESET);
        return;
    } 
    printf(GREEN "Dados carregados do arquivo com sucesso!\n" RESET); 
    char chave[MAX_CARACTERES];
    while (fscanf(file, "%s", chave) != EOF) {
        inserir(arvore, chave);
    }
    fclose(file);
}

void inserirArquivo(ArvoreB *arvore, const char *nomeArquivo, const char *chave) {
    inserir(arvore, chave);
    FILE *file = fopen(nomeArquivo, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fprintf(file, "%s\n", chave);
    fclose(file);
}

void emOrdem(No *raiz) {
    if (raiz == NULL) {
        return;
    }

     if (raiz->filhos[0] != NULL) {
        emOrdem(raiz->filhos[0]);
    }

     for (int i = 0; i < raiz->numChaves; i++) {
        printf("%s ", raiz->chaves[i]);
        
         if (raiz->filhos[i + 1] != NULL) {
            emOrdem(raiz->filhos[i + 1]);
        }
    }
}

