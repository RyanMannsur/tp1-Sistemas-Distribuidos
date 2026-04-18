#include <stdio.h>
#include "global.h"

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void* consumidor(void* arg) {
    while (1) {
        sem_wait(&sem_full);  
        sem_wait(&sem_mutex); 
        
        if (processados >= M_TOTAL) {
            sem_post(&sem_mutex);
            sem_post(&sem_full);  
            sem_post(&sem_empty); 
            break;
        }
        
        int numero = buffer[out];
        out = (out + 1) % N;
        processados++;
        ocupacao_atual--;

        if(ind_hist < TOTAL_OPERACOES) {
            historico[ind_hist++] = ocupacao_atual;
        }
        
        sem_post(&sem_mutex); 
        sem_post(&sem_empty); 
        
        bool primo = is_prime(numero);
        if (primo) {
            //printf("Consumidor processou %d: É primo.\n", numero);
        } else {
            //printf("Consumidor processou %d: Não é primo.\n", numero);
        }
    }
    return NULL;
}