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
    for (const auto& row : graph) {
        for (const auto& value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

void escreve_csv(int tempo_ms, int num_vertices){
    const char* saida_csv = "saida_dijkstra.csv";
    FILE* f = fopen(saida_csv, "a");
    fprintf(f, "%d,%d\n" , tempo_ms, num_vertices);
    fclose(f);
    f = NULL;
}


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

void dijkstra(const vector<vector<int>>& graph, int start) {
    // Verifica se o vértice de início é válido
    if (start < 0 || start >= graph.size()) {
        cerr << "Vertice de inicio invalido!" << endl;
        return;
    }
    
    // Vetor de distâncias, inicializado com infinito
    vector<int> dist(graph.size(), numeric_limits<int>::max());
    
    // Vetor para marcar vértices visitados
    vector<bool> visited(graph.size(), false);
    
    // Fila de prioridade para escolher o vértice com menor distância
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // Distância do vértice inicial para ele mesmo é 0
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        // Pega o vértice com menor distância
        int currentDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        // Se já foi visitado, pula
        if (visited[u]) continue;
        
        // Marca como visitado
        visited[u] = true;
        
        // Relaxa todas as arestas adjacentes
        for (int v = 0; v < graph.size(); v++) {
            // Se existe aresta (peso > 0) e o vértice não foi visitado
            if (graph[u][v] > 0 && !visited[v]) {
                int newDist = dist[u] + graph[u][v];
                
                // Se encontrou um caminho melhor
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    pq.push({newDist, v});
                 //   cout << "Atualizando distancia para vertice " << v+1 << ": " << newDist << endl;
                }
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
    // Configurar locale
    setlocale(LC_ALL, "Portuguese");
   
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <caminho_do_arquivo>" << endl;
        return 1;
    }

    int num_linhas = 0;
    vector<vector<int>> graph;
    char* arquivo = argv[1];
    fs::path filePath = arquivo;

    num_linhas = openNReadFile(&graph, filePath);
    
    if (graph.empty()) {
        return 1; // Encerra se o arquivo não pôde ser lido ou estava vazio
    }

    // Início da Medição de Tempo 
    auto start_time = chrono::steady_clock::now();

    // Executa o Dijkstra
    dijkstra(graph, 0);

    // Fim da Medição de Tempo 
    auto end_time = chrono::steady_clock::now();

    // Calcula a duração em microssegundos
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

    // Imprime o tempo na saída de erro padrão (stderr) para não interferir com o diff
   // cout << "Tempo de execução (Dijkstra): " << duration.count() << " microssegundos." << endl;

    escreve_csv(duration.count(), num_linhas);
    return 0;
}

