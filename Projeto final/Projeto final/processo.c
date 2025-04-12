#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"
#include <time.h>

// Função para calcular a quantidade de dias desde o ajuizamento do processo
int dias_tramitacao(const char *data_ajuizamento) {
    struct tm tm_ajuizamento = {0};
    time_t t_ajuizamento;
    time_t t_atual = time(NULL);

    // Usando sscanf para pegar os componentes da data
    if (sscanf(data_ajuizamento, "%4d-%2d-%2d %2d:%2d:%2d", 
                &tm_ajuizamento.tm_year, &tm_ajuizamento.tm_mon, &tm_ajuizamento.tm_mday, 
                &tm_ajuizamento.tm_hour, &tm_ajuizamento.tm_min, &tm_ajuizamento.tm_sec) != 6) {
        return -1;  // Erro no formato da data
    }

    // Ajustar o ano e o mês conforme o formato tm (ano desde 1900, mês de 0 a 11)
    tm_ajuizamento.tm_year -= 1900;
    tm_ajuizamento.tm_mon -= 1;

    tm_ajuizamento.tm_isdst = -1; // Avisa se é horário de verão (não aplicado aqui)
    t_ajuizamento = mktime(&tm_ajuizamento);

    return difftime(t_atual, t_ajuizamento) / (60 * 60 * 24);  // Retorna a diferença em dias
}

// Função para ler o arquivo CSV e popular a lista de processos
int ler_arquivo(const char *caminho_arquivo, Processo processos[], int max_processos) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo: %s\n", caminho_arquivo);
        return 0;
    }
    printf("Arquivo aberto com sucesso: %s\n", caminho_arquivo);

    char linha[1024];
    int i = 0;

    // Ignorar o cabeçalho do CSV
    fgets(linha, sizeof(linha), arquivo);  // Ignora a primeira linha

    while (fgets(linha, sizeof(linha), arquivo) && i < max_processos) {
        Processo p;
        char *token;
        char numero[256], data_ajuizamento[256], id_assunto[256];

        // Dividir a linha usando strtok() para separar os campos
        token = strtok(linha, ";");
        if (token) p.id = atoi(token);  // Lê o ID

        token = strtok(NULL, ";");
        if (token) strncpy(p.numero, token, sizeof(p.numero) - 1);  // Lê o número

        token = strtok(NULL, ";");
        if (token) strncpy(data_ajuizamento, token, sizeof(data_ajuizamento) - 1);  // Lê a data de ajuizamento

        token = strtok(NULL, ";");
        if (token) p.id_classe = atoi(token);  // Lê o ID da classe

        token = strtok(NULL, ";");
        if (token) strncpy(id_assunto, token, sizeof(id_assunto) - 1);  // Lê os assuntos

        token = strtok(NULL, ";");
        if (token) p.ano_eleicao = atoi(token);  // Lê o ano da eleição

        // Processamento do ID do assunto para remover as chaves se necessário
        id_assunto[strcspn(id_assunto, "\"")] = 0;  // Remover aspas

        // Copiar os dados para o processo
        strcpy(p.id_assunto, id_assunto);
        strcpy(p.data_ajuizamento, data_ajuizamento);
        p.dias_tramitacao = dias_tramitacao(p.data_ajuizamento);

        // Verificação se a linha foi lida corretamente
        if (strlen(p.numero) > 0 && p.id != 0) {
            processos[i++] = p;
        } else {
            printf("Erro na leitura da linha: %s\n", linha);
        }
    }

    fclose(arquivo);
    return i;
}
