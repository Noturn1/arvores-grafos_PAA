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

void openNReadFile(vector<vector<int>>* graph, const fs::path& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        return;
    }

    // Limpar a matriz antes de ler novos dados
    graph->clear();
    // Pular primeira linha do .txt
    string line;
    getline(file, line);  // Lê e descarta a primeira linha
    // Ler o restante do arquivo e preencher a matriz
    while (getline(file, line)) {
        vector<int> row;
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
        for (int u = 0; u < graph.size(); u++) {
            for (int v = 0; v < graph.size(); v++) {
                if (graph[u][v] != 0 && dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
             //       cout << "Atualizando distancia para vertice " << v+1 << ": " << dist[v] << endl;
                }
            }
        }
    }
    
    // Verifica por ciclos negativos
    for (int u = 0; u < graph.size(); u++) {
        for (int v = 0; v < graph.size(); v++) {
            if (graph[u][v] != 0 && dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                cerr << "Grafo contem ciclo negativo!" << endl;
                return;}
        }
    }
    
    // Imprime o resultado final
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


int main(int argc, char* argv[]) {
    // Configurar locale
    setlocale(LC_ALL, "Portuguese");
    
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <caminho_do_arquivo>" << endl;
        return 1;
    }

    vector<vector<int>> graph;
    char* arquivo = argv[1];
    fs::path filePath = arquivo;

    openNReadFile(&graph, filePath);

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
    cerr << "Tempo de execução (Bellman-Ford): " << duration.count() << " microssegundos." << endl;

    return 0;
}
