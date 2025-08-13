import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Carregar os dados
dijkstra_df = pd.read_csv('saida_dijkstra.csv')
bellman_df = pd.read_csv('saida_bellmanford.csv')

# Calcular médias agrupadas por número de vértices
dijkstra_means = dijkstra_df.groupby('num_vertices')['tempo_us'].mean().reset_index()
bellman_means = bellman_df.groupby('num_vertices')['tempo_us'].mean().reset_index()

# Juntar os dados em um único DataFrame
merged_df = pd.merge(dijkstra_means, bellman_means, on='num_vertices', suffixes=('_dijkstra', '_bellman'))
vertices = merged_df['num_vertices']

# Configurações do gráfico
bar_width = 0.4  # Largura maior para separar as barras
index = np.arange(len(vertices))
spacing = 0.1  # Espaçamento adicional entre grupos de barras

plt.figure(figsize=(14, 8))

# Criar as barras com espaçamento
plt.bar(index - bar_width/2 - spacing/2, merged_df['tempo_us_dijkstra'], 
        width=bar_width, color='blue', label='Dijkstra')
plt.bar(index + bar_width/2 + spacing/2, merged_df['tempo_us_bellman'], 
        width=bar_width, color='green', label='Bellman-Ford')

# Configurar eixos e rótulos
plt.xlabel('Número de Vértices', fontsize=12)
plt.ylabel('Tempo de Execução (µs)', fontsize=12)
plt.title('Comparação de Desempenho: Dijkstra vs Bellman-Ford', fontsize=14)
plt.xticks(index, vertices, rotation=45, fontsize=10)
plt.yscale('log')
plt.legend(loc = "upper left", fontsize=12)

# Desativar notação científica no eixo Y
#plt.ticklabel_format(axis='y', style='plain', useOffset=False)

# Adicionar valores nas barras (opcional)
for i, val in enumerate(merged_df['tempo_us_dijkstra']):
    plt.text(i - bar_width/2 - spacing/2, val + 0.02*val, f'{int(val)}', 
             ha='center', va='bottom', fontsize=8, rotation=45)
    
for i, val in enumerate(merged_df['tempo_us_bellman']):
    plt.text(i + bar_width/2 + spacing/2, val + 0.02*val, f'{int(val)}', 
             ha='center', va='bottom', fontsize=8, rotation=45)

# Ajustar limites do eixo Y
max_val = max(merged_df[['tempo_us_dijkstra', 'tempo_us_bellman']].max())
plt.ylim(0, 1000000)  # 20% de espaço extra acima do maior valor

# Melhorar a grade
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.grid(axis='x', alpha=0)  # Desativar grade vertical

# Mostrar o gráfico
plt.tight_layout()
plt.show()