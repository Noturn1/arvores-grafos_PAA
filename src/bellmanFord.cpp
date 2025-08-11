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

void printGraph(const vector<vector<int>>& graph) {
 //   for (const auto& row : graph) {
 //       for (const auto& value : row) {
 //           cout << value << " ";
 //       }
 //       cout << endl;
 //   }
}

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
    
    //cout << "\nExecutando algoritmo de Bellman-Ford a partir do vertice " << start+1 << ":\n" << endl;
    
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
            if (graph[u][v] > 0 && dist[u] != numeric_limits<int>::max() && dist[u] + graph[u][v] < dist[v]) {
                cerr << "Grafo contem ciclo negativo!" << endl;
                return;
            }
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
    // Configurar locale e codificação para suporte a acentos no Windows
    setlocale(LC_ALL, "Portuguese");
    //SetConsoleOutputCP(CP_UTF8);
    //SetConsoleCP(CP_UTF8);
    
    fs::path path = "grafos";

    if (fs::exists(path)) {
     //   cout << "O diretorio existe." << endl;
    } else {
    //    cout << "O diretorio nao existe." << endl;
       return 1;
    }

    vector<vector<int>> graph;
    char* arquivo = argv[1];
    fs::path filePath = path / arquivo;

    openNReadFile(&graph, filePath);
   // cout << "Matriz de adjacencia do grafo:" << endl;
  //  printGraph(graph);
    
    // Executa o algoritmo de Dijkstra a partir do vértice 0
    bellmanFord(graph, 0);
    return 0;
}
