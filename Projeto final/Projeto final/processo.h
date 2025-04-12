#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[20]; // Ajuste de tamanho se necessário
    int id_classe;
    char id_assunto[255];  // Agora 'id_assunto' como string
    int ano_eleicao;
    int dias_tramitacao;
} Processo;

int dias_tramitacao(const char *data_ajuizamento);
int ler_arquivo(const char *caminho_arquivo, Processo processos[], int max_processos);

#endif
