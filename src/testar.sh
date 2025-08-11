
# Obter lista de arquivos ordenada numericamente em um array
mapfile -t ARQUIVOS < <(ls -v ./grafos/)

for ARQUIVO in "${ARQUIVOS[@]}"; do
    echo "Processando $ARQUIVO para bellmanFord"
    ./bf "$ARQUIVO" > "saida_bf.txt"
    echo "Processando $ARQUIVO para dijkstra"
    ./dij "$ARQUIVO" > "saida_dij.txt"
    echo "comparando saidas"
    diff saida_bf.txt saida_dij.txt
done
