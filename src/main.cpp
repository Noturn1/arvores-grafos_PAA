#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <sstream>

using namespace std;

namespace fs = filesystem;

void printGraph(const vector<vector<int>>& graph) {
    for (const auto& row : graph) {
        for (const auto& value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

void openNReadFile(vector<vector<int>>* graph, const fs::path& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        return;
    }

    // Limpar a matriz antes de ler novos dados
    graph->clear();

    string line;
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

void dijkstra(const vector<vector<int>>& graph, int start) {
    vector<int> dist(graph.size(), INT_MAX);
    dist[start - 1] = 0;
    vector<bool> visited(graph.size(), false);
    for (size_t i = 0; i < graph.size(); ++i) {
        int u = -1;
        int minDist = INT_MAX;

        // Escolhe o vertice com a menor distância
        for (size_t j = 0; j < graph.size(); ++j) {
            if(!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break; // Todos os vértices visitados ou inacessíveis

        visited[u] = true;

        // Atualiza as distâncias dos vizinhos de u
        for(int v = 0; v < graph[u].size(); ++v) {
            if( graph[u][v] > 0 && !visited[v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    // Exibe distâncias
    cout<<"\nDistâncias minimas a partir do vértice " << start << ":\n";
    for(int i = 0;i < graph.size(); i++) {
        cout << "Vértice " << i + 1 << ": " << dist[i] << endl;
        if (dist[i] == INT_MAX) {
            cout << "Inacessível\n" << i << ": ";  
        } else {
            cout << "Acessível, distância: " << dist[i] << endl;
        }
    }
}

int main() {
    fs::path path = "grafos";

    if (fs::exists(path)) {
        cout << "O diretório existe." << endl;
    } else {
        cout << "O diretório não existe." << endl;
        return 1;
    }

    vector<vector<int>> graph;

    fs::path filePath = path / "Entrada 10.txt";

    openNReadFile(&graph, filePath);
    printGraph(graph);
    dijkstra(graph, 2);

    return 0;
}