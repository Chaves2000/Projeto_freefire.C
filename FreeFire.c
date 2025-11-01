#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10
#define MAX_COMPONENTES 20

// =============================================
// ESTRUTURAS BÁSICAS
// =============================================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// =============================================
// VARIÁVEIS GLOBAIS
// =============================================
int comparacoes = 0;

// =============================================
// FUNÇÕES PARA A MOCHILA COM VETOR
// =============================================
void inserirItemVetor(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("\n⚠️ Mochila cheia! Remova um item antes de inserir outro.\n");
        return;
    }
    Item novo;
    printf("Nome: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*qtd] = novo;
    (*qtd)++;
    printf("\n✅ Item inserido com sucesso!\n");
}

void removerItemVetor(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("\n⚠️ Mochila vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    for (int i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *qtd - 1; j++)
                mochila[j] = mochila[j + 1];
            (*qtd)--;
            printf("\n🗑️ Item removido.\n");
            return;
        }
    }
    printf("\n❌ Item não encontrado.\n");
}

void listarItensVetor(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("\n⚠️ Mochila vazia.\n");
        return;
    }
    printf("\n=== 🎒 Itens na Mochila ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d. %s | %s | %d\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// =============================================
// MÓDULO TORRE DE FUGA — COMPONENTES E ORDENAÇÃO
// =============================================

// Bubble Sort por nome
void bubbleSortNome(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

// Insertion Sort por tipo
void insertionSortTipo(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        while (j >= 0 && strcmp(v[j].tipo, chave.tipo) > 0) {
            v[j + 1] = v[j];
            j--;
            comparacoes++;
        }
        v[j + 1] = chave;
    }
}

// Selection Sort por prioridade
void selectionSortPrioridade(Componente v[], int n) {
    comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (v[j].prioridade < v[min].prioridade)
                min = j;
        }
        if (min != i) {
            Componente temp = v[i];
            v[i] = v[min];
            v[min] = temp;
        }
    }
}

// Busca binária por nome (após ordenação)
int buscaBinariaPorNome(Componente v[], int n, char nome[]) {
    int ini = 0, fim = n - 1;
    int comps = 0;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        comps++;
        int cmp = strcmp(v[meio].nome, nome);
        if (cmp == 0) {
            printf("Comparações na busca binária: %d\n", comps);
            return meio;
        }
        if (cmp < 0)
            ini = meio + 1;
        else
            fim = meio - 1;
    }
    printf("Comparações na busca binária: %d\n", comps);
    return -1;
}

void mostrarComponentes(Componente v[], int n) {
    printf("\n=== 🏗️ Componentes da Torre ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. Nome: %-20s | Tipo: %-15s | Prioridade: %d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

// =============================================
// MENU DA TORRE DE FUGA
// =============================================
void torreDeFuga() {
    Componente componentes[MAX_COMPONENTES];
    int qtd = 0, opc;
    clock_t inicio, fim;
    double tempo;

    do {
        printf("\n=== 🚀 TORRE DE FUGA ===\n");
        printf("1. Cadastrar componente\n");
        printf("2. Listar componentes\n");
        printf("3. Ordenar (Bubble/Nome)\n");
        printf("4. Ordenar (Insertion/Tipo)\n");
        printf("5. Ordenar (Selection/Prioridade)\n");
        printf("6. Buscar componente (binária por nome)\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        if (opc == 1) {
            if (qtd >= MAX_COMPONENTES) {
                printf("Limite máximo atingido!\n");
                continue;
            }
            printf("Nome: ");
            fgets(componentes[qtd].nome, sizeof(componentes[qtd].nome), stdin);
            componentes[qtd].nome[strcspn(componentes[qtd].nome, "\n")] = 0;
            printf("Tipo: ");
            fgets(componentes[qtd].tipo, sizeof(componentes[qtd].tipo), stdin);
            componentes[qtd].tipo[strcspn(componentes[qtd].tipo, "\n")] = 0;
            printf("Prioridade (1-10): ");
            scanf("%d", &componentes[qtd].prioridade);
            getchar();
            qtd++;
        }

        else if (opc == 2)
            mostrarComponentes(componentes, qtd);

        else if (opc == 3) {
            inicio = clock();
            bubbleSortNome(componentes, qtd);
            fim = clock();
            tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\nOrdenado por NOME usando Bubble Sort.\n");
            printf("Comparações: %d | Tempo: %.6f s\n", comparacoes, tempo);
            mostrarComponentes(componentes, qtd);
        }

        else if (opc == 4) {
            inicio = clock();
            insertionSortTipo(componentes, qtd);
            fim = clock();
            tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\nOrdenado por TIPO usando Insertion Sort.\n");
            printf("Comparações: %d | Tempo: %.6f s\n", comparacoes, tempo);
            mostrarComponentes(componentes, qtd);
        }

        else if (opc == 5) {
            inicio = clock();
            selectionSortPrioridade(componentes, qtd);
            fim = clock();
            tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\nOrdenado por PRIORIDADE usando Selection Sort.\n");
            printf("Comparações: %d | Tempo: %.6f s\n", comparacoes, tempo);
            mostrarComponentes(componentes, qtd);
        }

        else if (opc == 6) {
            if (qtd == 0) {
                printf("Cadastre componentes primeiro!\n");
                continue;
            }
            char nomeBusca[30];
            printf("Nome do componente-chave: ");
            fgets(nomeBusca, sizeof(nomeBusca), stdin);
            nomeBusca[strcspn(nomeBusca, "\n")] = 0;
            int pos = buscaBinariaPorNome(componentes, qtd, nomeBusca);
            if (pos != -1)
                printf("\n✅ Componente encontrado: %s | %s | Prioridade %d\n",
                       componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
            else
                printf("\n❌ Componente não encontrado.\n");
        }

    } while (opc != 0);
}

// =============================================
// PROGRAMA PRINCIPAL
// =============================================
int main() {
    Item mochilaVetor[MAX_ITENS];
    int qtdMochila = 0;
    int opc;

    do {
        printf("\n=== 🎮 MENU PRINCIPAL ===\n");
        printf("1️⃣  Gerenciar Mochila\n");
        printf("2️⃣  Missão Torre de Fuga\n");
        printf("0️⃣  Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);
        getchar();

        if (opc == 1) {
            int sub;
            do {
                printf("\n--- Mochila (Vetor) ---\n");
                printf("1. Inserir item\n2. Remover item\n3. Listar itens\n0. Voltar\nEscolha: ");
                scanf("%d", &sub);
                getchar();

                if (sub == 1) inserirItemVetor(mochilaVetor, &qtdMochila);
                else if (sub == 2) removerItemVetor(mochilaVetor, &qtdMochila);
                else if (sub == 3) listarItensVetor(mochilaVetor, qtdMochila);
            } while (sub != 0);
        }

        else if (opc == 2) {
            torreDeFuga();
        }

    } while (opc != 0);

    printf("\n👋 Encerrando o programa...\n");
    return 0;
}

