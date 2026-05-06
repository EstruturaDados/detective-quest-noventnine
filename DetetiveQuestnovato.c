#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 Estrutura da árvore binária:
 Cada nó representa uma sala da mansão.
*/
struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
};

/* 
 Função criarSala():
 Cria dinamicamente uma sala com o nome informado.
*/
struct Sala* criarSala(const char* nome) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* 
 Função explorarSalas():
 Permite ao jogador navegar pela mansão.
 O jogador começa no Hall de entrada e escolhe:
   - 'e' para ir à esquerda
   - 'd' para ir à direita
   - 's' para sair
 A exploração continua até chegar a um cômodo sem caminhos.
*/
void explorarSalas(struct Sala* atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na %s.\n", atual->nome);

        // Se não há caminhos, fim da exploração
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração!\n");
            break;
        }

        printf("Escolha um caminho: (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há sala à esquerda!\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há sala à direita!\n");
            }
        } else if (escolha == 's') {
            printf("Você decidiu sair da exploração.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/* 
 Função main():
 Monta o mapa da mansão de forma fixa (manual).
 O jogador começa no Hall de entrada.
*/
int main() {
    // Criando as salas
    struct Sala* hall = criarSala("Hall de entrada");
    struct Sala* salaEstar = criarSala("Sala de estar");
    struct Sala* biblioteca = criarSala("Biblioteca");
    struct Sala* cozinha = criarSala("Cozinha");
    struct Sala* jardim = criarSala("Jardim");
    struct Sala* quarto = criarSala("Quarto");

    // Montando a árvore (mapa da mansão)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    biblioteca->direita = quarto;

    // Início da exploração
    printf("Bem-vindo(a) ao Detective Quest!\n");
    printf("Você começará sua exploração pelo Hall de entrada.\n");

    explorarSalas(hall);

    return 0;
}
