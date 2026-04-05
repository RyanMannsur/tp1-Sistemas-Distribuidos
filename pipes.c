#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

#define BUFFER_SIZE 20
#define READ_END 0
#define WRITE_END 1

// Função para verificar se um número é primo
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // O produtor tem como parâmetro o número de números a serem gerados
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <quantidade_de_numeros>\n", argv[0]);
        return 1;
    }

    int num_gerar = atoi(argv[1]);
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    pid_t pid;
    int fd[2];

    // Cria o pipe
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    // Cria um processo filho com fork
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) {  // Processo pai (PRODUTOR)
        // Fecha a extremidade de leitura do pipe 
        close(fd[READ_END]);

        srand(time(NULL));
        int n_atual = 1;

        for (int i = 0; i < num_gerar; i++) {
            // Formata o número para ter exatos 20 bytes (preenche com zeros à esquerda se precisar)
            snprintf(write_msg, BUFFER_SIZE, "%019d", n_atual);

            // Escreve os 20 bytes exatos no pipe
            write(fd[WRITE_END], write_msg, BUFFER_SIZE);
            //printf("Produtor: Gerou e enviou o número %d\n", n_atual);

            // Calcula o próximo número
            int delta = (rand() % 100) + 1;
            n_atual += delta;
        }

        // Envia o zero para terminar a execução do consumidor
        snprintf(write_msg, BUFFER_SIZE, "%019d", 0);
        write(fd[WRITE_END], write_msg, BUFFER_SIZE);

        // Fecha a extremidade de escrita do pipe 
        close(fd[WRITE_END]);

        // Aguarda o filho terminar para não criar um processo zumbi
        wait(NULL);
    }
    else { // Processo filho (CONSUMIDOR)
        // Fecha a extremidade de escrita do pipe
        close(fd[WRITE_END]);

        // A função read bloqueia enquanto houver dados sendo escritos no pipe
        while (read(fd[READ_END], read_msg, BUFFER_SIZE) > 0) {
            int numero_recebido = atoi(read_msg);

            // O processo consumidor deve terminar quando receber o número 0
            if (numero_recebido == 0) {
                break;
            }

            // Verifica se o mesmo é primo, imprimindo o resultado
            if (is_prime(numero_recebido)) {
                printf("Consumidor: %d é PRIMO.\n", numero_recebido);
            }
            else {
                printf("Consumidor: %d não é primo.\n", numero_recebido);
            }
        }

        // Fecha a extremidade de leitura do pipe
        close(fd[READ_END]);
    }

    return 0;
}