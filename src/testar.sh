#!/bin/bash

# Diretório onde os executáveis estão localizados
SRC_DIR="src" 

# Diretório onde os grafos estão localizados
GRAFOS_DIR="grafos"

# Verifica se o diretório de grafos existe
if [ ! -d "$GRAFOS_DIR" ]; then
    echo "Erro: Diretório de grafos '$GRAFOS_DIR' não encontrado."
    echo "Execute este script a partir do diretório raiz do seu projeto."
    exit 1
fi

# Usa 'find' para listar os arquivos no diretório de grafos.
# O 'sort -V' garante a mesma ordenação por versão.
find "$GRAFOS_DIR" -type f -name "Entrada*.txt" | sort -V | while IFS= read -r ARQUIVO_RELATIVO; do
    # Converte o caminho relativo (ex: grafos/Entrada.txt) 
    ARQUIVO_ABSOLUTO=$(realpath "$ARQUIVO_RELATIVO")

    echo "Processando $ARQUIVO_ABSOLUTO para bellmanFord"
    ./"$SRC_DIR"/bf "$ARQUIVO_ABSOLUTO" > saida_bf.txt

    echo "Processando $ARQUIVO_ABSOLUTO para dijkstra"
    ./"$SRC_DIR"/dij "$ARQUIVO_ABSOLUTO" > saida_dij.txt

    echo "Comparando saidas"
    diff saida_bf.txt saida_dij.txt
done

echo "Todos os grafos foram processados."
