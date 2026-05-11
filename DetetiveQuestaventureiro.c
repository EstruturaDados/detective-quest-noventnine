#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para representar um nó da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* criarSala() – cria dinamicamente um cômodo com ou sem pista */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        strcpy(novaSala->pista, "");
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/* inserirPista() – insere uma nova pista na árvore de pistas (BST) */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

/* exibirPistas() – imprime a árvore de pistas em ordem alfabética */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* explorarSalasComPistas() – controla a navegação entre salas e coleta de pistas */
void explorarSalasComPistas(Sala* atual, PistaNode** pistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está no %s.\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("Escolha o caminho: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e')
            atual = atual->esquerda;
        else if (escolha == 'd')
            atual = atual->direita;
        else if (escolha == 's')
            break;
        else
            printf("Opção inválida.\n");
    }
}

int main() {
    // Criando o mapa fixo da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave dourada");
    Sala* salaEstar = criarSala("Sala de Estar", "Carta rasgada");
    Sala* cozinha = criarSala("Cozinha", "Receita misteriosa");
    Sala* biblioteca = criarSala("Biblioteca", "Livro antigo");
    Sala* jardim = criarSala("Jardim", NULL);

    // Conectando as salas (árvore binária da mansão)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Árvore de pistas inicialmente vazia
    PistaNode* pistas = NULL;

    // Exploração da mansão
    explorarSalasComPistas(hall, &pistas);

    // Exibir pistas coletadas
    printf("\nPistas coletadas em ordem alfabética:\n");
    exibirPistas(pistas);

    return 0;
}
