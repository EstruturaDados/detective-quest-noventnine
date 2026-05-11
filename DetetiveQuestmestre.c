#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

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

// Estrutura para tabela hash (associação pista → suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[HASH_SIZE]; // tabela global

/* criarSala() – cria dinamicamente um cômodo */
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

/* Função hash simples baseada na soma dos caracteres */
int hashFunc(const char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % HASH_SIZE;
}

/* inserirNaHash() – insere associação pista/suspeito na tabela hash */
void inserirNaHash(const char* pista, const char* suspeito) {
    int idx = hashFunc(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

/* encontrarSuspeito() – consulta o suspeito correspondente a uma pista */
char* encontrarSuspeito(const char* pista) {
    int idx = hashFunc(pista);
    HashNode* atual = tabelaHash[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/* inserirPista() – insere a pista coletada na árvore BST */
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
        printf("- %s (suspeito: %s)\n", raiz->pista, encontrarSuspeito(raiz->pista));
        exibirPistas(raiz->direita);
    }
}

/* explorarSalas() – navega pela árvore e ativa o sistema de pistas */
void explorarSalas(Sala* atual, PistaNode** pistas) {
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

/* Função auxiliar para contar pistas de um suspeito */
int contarPistasSuspeito(PistaNode* raiz, const char* acusado) {
    if (raiz == NULL) return 0;
    int contador = 0;
    char* suspeito = encontrarSuspeito(raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, acusado) == 0)
        contador++;
    contador += contarPistasSuspeito(raiz->esquerda, acusado);
    contador += contarPistasSuspeito(raiz->direita, acusado);
    return contador;
}

/* verificarSuspeitoFinal() – conduz à fase de julgamento final */
void verificarSuspeitoFinal(PistaNode* pistas) {
    char acusado[50];
    printf("\nDigite o nome do suspeito que você acredita ser o culpado: ");
    scanf(" %[^\n]", acusado);

    int contador = contarPistasSuspeito(pistas, acusado);

    if (contador >= 2)
        printf("\nAcusação válida! Pelo menos duas pistas apontam para %s.\n", acusado);
    else
        printf("\nAcusação fraca. Menos de duas pistas apontam para %s.\n", acusado);
}

int main() {
    // Criando o mapa fixo da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave dourada");
    Sala* salaEstar = criarSala("Sala de Estar", "Carta rasgada");
    Sala* cozinha = criarSala("Cozinha", "Receita misteriosa");
    Sala* biblioteca = criarSala("Biblioteca", "Livro antigo");
    Sala* jardim = criarSala("Jardim", "Relógio quebrado");

    // Conectando as salas
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    // Inserindo pistas e suspeitos na tabela hash
    inserirNaHash("Chave dourada", "Sr. Black");
    inserirNaHash("Carta rasgada", "Sra. White");
    inserirNaHash("Receita misteriosa", "Chef Green");
    inserirNaHash("Livro antigo", "Professor Plum");
    inserirNaHash("Relógio quebrado", "Sr. Black"); // segunda pista para Sr. Black

    // Árvore de pistas inicialmente vazia
    PistaNode* pistas = NULL;

    // Exploração da mansão
    explorarSalas(hall, &pistas);

    // Exibir pistas coletadas
    printf("\nPistas coletadas:\n");
    exibirPistas(pistas);

    // Julgamento final
    verificarSuspeitoFinal(pistas);

    return 0;
}
