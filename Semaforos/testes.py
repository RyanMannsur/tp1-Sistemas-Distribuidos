import subprocess
import time
import csv

valores_N = [1, 10, 100, 1000]
combinacoes_threads = [(1,1), (1,2), (1,4), (1,8), (2,1), (4,1), (8,1)]

resultados = []

print("Iniciando os testes...")

for N in valores_N:
    for Np, Nc in combinacoes_threads:
        tempos = []
        print(f"Testando: {Np} Produtor(es) | {Nc} Consumidor(es) | Buffer N={N}")
        
        # Rodar 10 vezes para tirar a média
        for _ in range(10):
            inicio = time.time()
            
            subprocess.run(["./semaforos", str(Np), str(Nc), str(N)], stdout=subprocess.DEVNULL)
            
            fim = time.time()
            tempos.append(fim - inicio)
            
        tempo_medio = sum(tempos) / len(tempos)
        tempo_formatado = f"{tempo_medio:.4f}"
        
        resultados.append([N, f"{Np}p_{Nc}c", tempo_formatado])

# Salvar os resultados em um arquivo CSV
with open("tempos_medios.csv", "w", newline='') as f:
    writer = csv.writer(f)
    writer.writerow(["N", "Threads(Np_Nc)", "Tempo_Medio_Segundos"])
    writer.writerows(resultados)
    
print("\nTestes finalizados com sucesso!")
print("Os resultados foram salvos no arquivo 'tempos_medios.csv'.")