#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <queue>
#include <limits>
#include <iomanip>
#include <locale>
//#include <windows.h>

using namespace std;

namespace fs = filesystem;

int openNReadFile(vector<vector<int>>* graph, const fs::path& filePath) {
    int num_linhas = 0;
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        return -1;
    }

    // Limpar a matriz antes de ler novos dados
    graph->clear();
    // Pular primeira linha do .txt
    string line;
    getline(file, line);  // Lê e descarta a primeira linha
    // Ler o restante do arquivo e preencher a matriz
    while (getline(file, line)) {
        vector<int> row;
        num_linhas++;
        stringstream ss(line);
        int value;
        while (ss >> value) {
            row.push_back(value);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        graph->push_back(row);
    }
    return num_linhas;
}



void bellmanFord(const vector<vector<int>>& graph, int start) {
    // Verifica se o vértice de início é válido
    if (start < 0 || start >= graph.size()) {
        cerr << "Vertice de inicio invalido!" << endl;
        return;
    }
    
    // Vetor de distâncias, inicializado com infinito
    vector<int> dist(graph.size(), numeric_limits<int>::max());
    dist[start] = 0;  // Distância do vértice inicial para ele mesmo é 0
    
    // Relaxa todas as arestas |V| - 1 vezes
    for (int i = 0; i < graph.size() - 1; i++) {
        bool updated = false;  // Flag para verificar atualizações
        
        for (int u = 0; u < graph.size(); u++) {
            for (int v = 0; v < graph.size(); v++) {
                if (graph[u][v] != 0 && dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    updated = true;  // Marca que houve atualização
                }
            }
        }
        
        // Parada antecipada se não houve atualização nesta iteração
        if (!updated) {
            //cout << "Parada antecipada na iteracao " << i+1 << endl;
            break;
        }
    }
    
    // Verifica por ciclos negativos (mantido igual)
    for (int u = 0; u < graph.size(); u++) {
        for (int v = 0; v < graph.size(); v++) {
            if (graph[u][v] != 0 && dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                cerr << "Grafo contem ciclo negativo!" << endl;
                return;
            }
        }
    }
    
    // Imprime o resultado final (mantido igual)
    cout << "\nDistancias minimas a partir do vertice " << start+1 << ":" << endl;
    cout << "Vertice\tDistancia" << endl;
    cout << "----------------" << endl;
    
    for (int i = 0; i < graph.size(); i++) {
        cout << i+1 << "\t";
        if (dist[i] == numeric_limits<int>::max()) {
            cout << "infinito (inalcancavel)" << endl;
        } else {
            cout << dist[i] << endl;
        }
    }
}


void escreve_csv(int tempo_ms, int num_vertices){
    const char* saida_csv = "saida_bellmanford.csv"; 
    FILE* f = fopen(saida_csv, "a");
    fprintf(f, "%d,%d\n" , tempo_ms, num_vertices);
    fclose(f);
    f = NULL;
}

int main(int argc, char* argv[]) {
    // Configurar locale
    int num_linhas = 0;
    setlocale(LC_ALL, "Portuguese");
    
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <caminho_do_arquivo>" << endl;
        return 1;
    }

    vector<vector<int>> graph;
    char* arquivo = argv[1];
    fs::path filePath = arquivo;

    num_linhas = openNReadFile(&graph, filePath);

    if (graph.empty()) {
        return 1; // Encerra se o arquivo não pôde ser lido ou estava vazio
    }
    
    // Início da Medição de Tempo 
    auto start_time = chrono::steady_clock::now();

    // Executa o algoritmo de Bellman-Ford
    bellmanFord(graph, 0);

    // Fim da Medição de Tempo 
    auto end_time = chrono::steady_clock::now();

    // Calcula a duração em microssegundos
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    // Imprime o tempo na saída de erro padrão (stderr) para não interferir com o diff
 //   cerr << "Tempo de execução (Bellman-Ford): " << duration.count() << " microssegundos." << endl;
    escreve_csv(duration.count(), num_linhas);
    return 0;
}
