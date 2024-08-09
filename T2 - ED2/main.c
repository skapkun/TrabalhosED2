#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
 
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"

void menu() {
    printf("\n");
    printf(BOLD GREEN "=======================================\n" RESET);
    printf(BOLD CYAN "          ÁRVORE-B IMPLEMENTAÇÃO       \n" RESET);
    printf(BOLD GREEN "=======================================\n" RESET);
    printf("1. " BLUE "Carregar dados do arquivo" RESET "\n");
    printf("2. " BLUE "Inserir chave" RESET "\n");
    printf("3. " BLUE "Exibir árvore" RESET "\n");
    printf("4. " BLUE "Imprimir em ordem" RESET "\n");
    printf("5. " BLUE "Buscar chave" RESET "\n");
    printf("6. " BLUE "Destruir árvore" RESET "\n");
    printf("7. " BLUE "Sair" RESET "\n");
    printf(BOLD GREEN "=======================================\n" RESET);
    printf("Escolha uma opção: ");
}

 
int main() {
    system("chcp 65001");
    system ("cls");
    ArvoreB *arvore = criarArvoreB(3);  
    char nomeArquivo[] = "testeB.txt";
    int opcao;
    char chave[MAX_CARACTERES];
    
    do {
        menu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                carregarArquivo(arvore, nomeArquivo); 
                break;
            case 2:
                printf("Digite a chave (até 20 caracteres, sem espaços): ");
                scanf("\n%s", chave);
                inserirArquivo(arvore, nomeArquivo, chave);
                printf(GREEN "Chave inserida com sucesso!\n" RESET);
                break;
            case 3:
                printf("Árvore-B:\n");
                exibirArvoreB(arvore->raiz, 0, 1); // Passa 1 para o nó raiz
                break;
            case 4:
                printf("Árvore-B em ordem:\n");
                emOrdem(arvore->raiz);
                printf("\n");
                break;
            case 5:
                printf("Digite a chave a buscar: ");
                scanf("%s", chave);
                ResultadoBusca resultado = buscar(arvore->raiz, chave, NULL, NULL, NULL, 1);
                if (resultado.no != NULL) {
                    printf(GREEN "\nChave %s encontrada!\n" RESET, chave);
                    if (resultado.ehRaiz) {
                        printf("Nó: Raiz\n");
                    } else {
                        printf("Nó: %s\n", resultado.ehInterno ? "Interno" : "Folha");
                    }
                    printf("Pai: %s\n", (resultado.pai != NULL) ? resultado.pai->chaves[0] : "Sem pai");
                    if (strlen(resultado.irmaoEsquerdo) > 0) {
                        printf("Irmão esquerdo: %s\n", resultado.irmaoEsquerdo);
                    } else {
                        printf("Sem irmão esquerdo.\n");
                    }
                    if (strlen(resultado.irmaoDireito) > 0) {
                        printf("Irmão direito: %s\n", resultado.irmaoDireito);
                    } else {
                        printf("Sem irmão direito.\n");
                    }
                } else {
                    printf(RED "Chave %s não encontrada.\n" RESET, chave);
                }
                break;
            case 6:
                printf(GREEN "Destruindo a árvore...\n" RESET);
                destruirArvoreB(arvore);
                arvore = criarArvoreB(3);
                printf(GREEN "Árvore destruída com sucesso!\n" RESET);
                break;
            case 7:
                printf(GREEN"Encerrando o programa. Tenha um bom dia!\n"RESET);
                break;
            default:
                printf(RED "Opção inválida!\n" RESET);
        }
    } while (opcao != 7);
    
    destruirArvoreB(arvore);
    return 0;
}
