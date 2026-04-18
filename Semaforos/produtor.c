#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "global.h"

void* produtor(void* arg) {
    int id = *((int*)arg);
    unsigned int seed = time(NULL) ^ id; 

    while (1) {
        int numero = (rand_r(&seed) % MAX_NUM) + 1;
        
        sem_wait(&sem_empty); 
        sem_wait(&sem_mutex); 
        
        if (processados >= M_TOTAL) {
            sem_post(&sem_mutex); 
            sem_post(&sem_empty); 
            sem_post(&sem_full);  
            break;
        }
        
        buffer[in] = numero;
        in = (in + 1) % N;
        ocupacao_atual++;

        if(ind_hist < TOTAL_OPERACOES) {
            historico[ind_hist++] = ocupacao_atual;
        }
        
        sem_post(&sem_mutex); 
        sem_post(&sem_full);  
    }
    return NULL;
}