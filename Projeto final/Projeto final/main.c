#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

#define MAX_PROCESSOS 1000

int comparar_id(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return p1->id - p2->id;
}

int comparar_data_ajuizamento(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return strcmp(p1->data_ajuizamento, p2->data_ajuizamento);
}

int contar_por_classe(Processo processos[], int total_processos, int id_classe) {
    int count = 0;
    for (int i = 0; i < total_processos; i++) {
        if (processos[i].id_classe == id_classe) {
            count++;
        }
    }
    return count;
}

int contar_assuntos_unicos(Processo processos[], int total_processos) {
    int count = 0;
    char assuntos_unicos[MAX_PROCESSOS][255];
    for (int i = 0; i < total_processos; i++) {
        int encontrado = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(processos[i].id_assunto, assuntos_unicos[j]) == 0) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            strcpy(assuntos_unicos[count], processos[i].id_assunto);
            count++;
        }
    }
    return count;
}

void listar_processos_multiplos_assuntos(Processo processos[], int total_processos) {
    for (int i = 0; i < total_processos; i++) {
        if (strchr(processos[i].id_assunto, ',')) {  // Verifica se o id_assunto contém mais de um valor
            printf("Processo ID: %d, Número: %s, Assuntos: %s\n", processos[i].id, processos[i].numero, processos[i].id_assunto);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Por favor, forneça o caminho do arquivo como argumento.\n");
        return 1;
    }

    const char *caminho_arquivo = argv[1];
    Processo processos[MAX_PROCESSOS];

    int total_processos = ler_arquivo(caminho_arquivo, processos, MAX_PROCESSOS);
    printf("Total de processos lidos: %d\n", total_processos);

    if (total_processos == 0) {
        printf("Erro ao ler o arquivo ou nenhum processo encontrado.\n");
        return 1;
    }

    qsort(processos, total_processos, sizeof(Processo), comparar_id);
    printf("\nProcessos ordenados por ID:\n");
    for (int i = 0; i < total_processos; i++) {
        printf("ID: %d, Número: %s\n", processos[i].id, processos[i].numero);
    }

    qsort(processos, total_processos, sizeof(Processo), comparar_data_ajuizamento);
    printf("\nProcessos ordenados por data de ajuizamento:\n");
    for (int i = 0; i < total_processos; i++) {
        printf("ID: %d, Número: %s, Data: %s\n", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
    }

    int id_classe = 11778;  // Exemplo de ID de classe
    int total_classe = contar_por_classe(processos, total_processos, id_classe);
    printf("\nTotal de processos na classe %d: %d\n", id_classe, total_classe);

    int total_assuntos = contar_assuntos_unicos(processos, total_processos);
    printf("\nTotal de assuntos únicos: %d\n", total_assuntos);

    printf("\nProcessos com mais de um assunto:\n");
    listar_processos_multiplos_assuntos(processos, total_processos);

    return 0;
}
