# tp1-Sistemas-Distribuidos

## Como rodar

> Recomendado usar Linux/WSL.

### 1) Pipe

No diretório raiz do projeto, compile e execute:

- `gcc pipes.c -o pipes`
- `./pipes 20`

O de pipe é direto: compila e roda no próprio diretório raiz.

---

### 2) Produtor-Consumidor com Semáforos

Para esse caso, **entre na pasta correta** antes de compilar/executar:

- `cd Semaforos`
- `gcc main.c produtor.c consumidor.c -o semaforos -pthread`
- `./semaforos 2 2 100`

Parâmetros:

- `Np`: número de produtores
- `Nc`: número de consumidores
- `N`: tamanho do buffer

Exemplo: `./semaforos 1 4 10`