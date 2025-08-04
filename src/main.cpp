#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>

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

int main() {
    fs::path path = "grafos ponderados nao direcionados";

    if (fs::exists(path)) {
        cout << "O diretório existe." << endl;
        for (const auto& entry : fs::directory_iterator(path)) {
            cout << "Found file: " << entry.path() << endl;
        }
    } else {
        cout << "O diretório não existe." << endl;
        return 1;
    }

    vector<vector<int>> graph;

    
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            ifstream file(entry.path());
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
                graph.push_back(row);
            }
        }
        printGraph(graph);
        break;
    }

    return 0;
}