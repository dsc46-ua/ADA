//DANIEL SEGURADO CLIMENT 48791680D
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <chrono>
#include <climits>
#include <iomanip>

using namespace std;

// Tipo enumérico para las 8 direcciones posibles
enum Step { N = 1, NE = 2, E = 3, SE = 4, S = 5, SW = 6, W = 7, NW = 8 };

// Variables globales
vector<vector<int>> maze;
int n, m;
vector<vector<bool>> best_path;
vector<Step> best_moves;
int best_path_length = INT_MAX;

// Estadísticas del algoritmo
int visited_nodes = 0;
int explored_nodes = 0;
int visited_leaf_nodes = 0;
int no_feasible_discarded_nodes = 0;
int no_promising_discarded_nodes = 0;

// Mapa de incrementos para cada dirección
map<Step, tuple<int,int>> steps_inc = {
    {N, make_tuple(-1, 0)},
    {NE, make_tuple(-1, 1)},
    {E, make_tuple(0, 1)},
    {SE, make_tuple(1, 1)},
    {S, make_tuple(1, 0)},
    {SW, make_tuple(1, -1)},
    {W, make_tuple(0, -1)},
    {NW, make_tuple(-1, -1)}
};

// Verifica si una posición es válida y accesible
bool is_valid(int row, int col) {
    return (row >= 0 && row < n && col >= 0 && col < m && maze[row][col] == 1);
}

// Verifica si hemos llegado al destino
bool is_destination(int row, int col) {
    return (row == n-1 && col == m-1);
}

// Función principal de backtracking
void maze_bt(int row, int col, int path_length, vector<vector<bool>>& current_path, vector<Step>& current_moves) {
    // Si llegamos a una hoja (destino)
    if (is_destination(row, col)) {
        visited_leaf_nodes++;
        if (path_length < best_path_length) {
            best_path_length = path_length;
            best_path = current_path;
            best_moves = current_moves;
        }
        return;
    }
    
    // Expandir el nodo actual
    for (auto& [step, inc] : steps_inc) {
        visited_nodes++;
        
        int new_row = row + get<0>(inc);
        int new_col = col + get<1>(inc);
        
        // Verificar si es factible
        bool factible = is_valid(new_row, new_col) && !current_path[new_row][new_col];
        
        if (factible) {
            // Verificar si es prometedor
            bool prometedor = path_length + 1 < best_path_length;
            
            if (prometedor) {
                explored_nodes++;
                
                // Marcar esta casilla como parte del camino
                current_path[new_row][new_col] = true;
                current_moves.push_back(step);
                
                // Llamada recursiva
                maze_bt(new_row, new_col, path_length + 1, current_path, current_moves);
                
                // Backtracking - deshacer cambios
                current_path[new_row][new_col] = false;
                current_moves.pop_back();
            } else {
                no_promising_discarded_nodes++;
            }
        } else {
            no_feasible_discarded_nodes++;
        }
    }
}

// Leer el laberinto desde un archivo
bool read_maze(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: No se puede abrir el archivo " << filename << endl;
        return false;
    }
    
    file >> n >> m;
    maze.resize(n, vector<int>(m));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file >> maze[i][j];
        }
    }
    
    file.close();
    return true;
}

// Mostrar el camino en formato 2D
void print_path_2D() {
    if (best_path_length == INT_MAX) {
        cout << "0" << endl;
        return;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (best_path[i][j]) {
                cout << '*';
            } else {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
}

// Mostrar el camino codificado
void print_path_coded() {
    if (best_path_length == INT_MAX) {
        cout << "<0>" << endl;
        return;
    }
    
    cout << "<";
    for (auto step : best_moves) {
        cout << static_cast<int>(step);
    }
    cout << ">" << endl;
}

int main(int argc, char* argv[]) {
    bool show_path = false;
    bool show_path_2D = false;
    string filename;
    
    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "-p") {
            show_path = true;
        } else if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "-f") {
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                cerr << "Error: La opción -f requiere un nombre de archivo." << endl;
                cerr << "Uso: " << argv[0] << " [ -p] [ --p2D] -f fichero_entrada" << endl;
                return 1;
            }
        } else {
            cerr << "Error: Opción desconocida: " << arg << endl;
            cerr << "Uso: " << argv[0] << " [ -p] [ --p2D] -f fichero_entrada" << endl;
            return 1;
        }
    }
    
    // Verificar que se proporcionó un nombre de archivo
    if (filename.empty()) {
        cerr << "Error: Debe proporcionar un nombre de archivo con la opción -f." << endl;
        cerr << "Uso: " << argv[0] << " [ -p] [ --p2D] -f fichero_entrada" << endl;
        return 1;
    }
    
    // Leer el laberinto
    if (!read_maze(filename)) {
        return 1;
    }
    
    // Inicializar estructuras para el algoritmo
    best_path.resize(n, vector<bool>(m, false));
    vector<vector<bool>> current_path(n, vector<bool>(m, false));
    vector<Step> current_moves;
    
    // Medir el tiempo de ejecución
    auto start = chrono::high_resolution_clock::now();
    
    // Si la entrada es accesible, empezamos el algoritmo
    if (is_valid(0, 0)) {
        current_path[0][0] = true;  // Marcar la casilla inicial
        maze_bt(0, 0, 1, current_path, current_moves);
    }
    
    // Calcular tiempo transcurrido
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    double time_ms = duration.count() / 1000.0;
    
    // Mostrar resultados básicos
    if (best_path_length == INT_MAX) {
        cout << "0" << endl;
    } else {
        cout << best_path_length << endl;
    }
    
    cout << visited_nodes << " " << explored_nodes << " " << visited_leaf_nodes << " " 
         << no_feasible_discarded_nodes << " " << no_promising_discarded_nodes << endl;
    
    cout << fixed << setprecision(3) << time_ms << endl;
    
    // Mostrar el camino según las opciones solicitadas
    if (show_path_2D) {
        print_path_2D();
    }
    
    if (show_path) {
        print_path_coded();
    }
    
    return 0;
}