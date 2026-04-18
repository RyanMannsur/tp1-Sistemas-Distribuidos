#include <stdio.h>
#include <stdlib.h>
#include "global.h"

int *buffer;
int N;
int Np, Nc;
int in = 0, out = 0;
int processados = 0; 
int ocupacao_atual = 0;

int *historico;
int ind_hist = 0;

sem_t sem_empty; 
sem_t sem_full;  
sem_t sem_mutex; 

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N_produtores> <N_consumidores> <Tamanho_Buffer_N>\n", argv[0]);
        return 1;
    }

    Np = atoi(argv[1]);
    Nc = atoi(argv[2]);
    N = atoi(argv[3]);

    buffer = (int *)malloc(N * sizeof(int));

    sem_init(&sem_empty, 0, N);
    sem_init(&sem_full, 0, 0); 
    sem_init(&sem_mutex, 0, 1);

    pthread_t th_produtores[Np];
    pthread_t th_consumidores[Nc];
    int id_produtores[Np];

    historico = (int *)malloc(TOTAL_OPERACOES * sizeof(int));

    for (int i = 0; i < Np; i++) {
        id_produtores[i] = i;
        pthread_create(&th_produtores[i], NULL, produtor, &id_produtores[i]);
    }
    for (int i = 0; i < Nc; i++) {
        pthread_create(&th_consumidores[i], NULL, consumidor, NULL);
    }

    for (int i = 0; i < Np; i++) {
        pthread_join(th_produtores[i], NULL);
    }
    for (int i = 0; i < Nc; i++) {
        pthread_join(th_consumidores[i], NULL);
    }

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_mutex);
    free(buffer);

    printf("Processamento concluido! %d itens processados.\n", processados);

    FILE *f = fopen("ocupacao.csv", "w");
    for(int i = 0; i < ind_hist; i++) {
        fprintf(f, "%d\n", historico[i]);
    }
    fclose(f);
    free(historico);

    return 0;
}