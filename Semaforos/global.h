#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_NUM 10000000 // 10^7
#define M_TOTAL 100000   // 10^5
#define TOTAL_OPERACOES 200000

extern int *historico;
extern int ind_hist;

extern int *buffer;
extern int N;
extern int Np, Nc;
extern int in, out;
extern int processados;
extern int ocupacao_atual;

// Semáforos
extern sem_t sem_empty;
extern sem_t sem_full;
extern sem_t sem_mutex;

// Funções
bool is_prime(int n);
void* produtor(void* arg);
void* consumidor(void* arg);

#endif