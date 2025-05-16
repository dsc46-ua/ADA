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
#include <cstring>
#include <algorithm> // Required for std::max
#include <cstdlib>   // Required for std::abs (for int)

using namespace std;

// Tipo enumérico para las 8 direcciones posibles
enum Step { N = 1, NE = 2, E = 3, SE = 4, S = 5, SW = 6, W = 7, NW = 8 };

// Variables globales
vector<vector<int>> maze;
int n, m;
vector<vector<bool>> best_path;
vector<Step> best_moves;
int best_path_length = INT_MAX;

// Estructura para almacenar posiciones en el camino
struct Position {
    int x, y;
};
vector<Position> path_positions; // Stores the coordinates of the best path found

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

// Heuristic: Chebyshev distance to destination
int chebyshev_distance(int r1, int c1, int r2, int c2) {
    return std::max(std::abs(r1 - r2), std::abs(c1 - c2));
}

// Función principal de backtracking
void maze_bt(int row, int col, int current_path_len, vector<vector<bool>>& current_path_grid, vector<Step>& current_moves_seq, vector<Position>& current_positions_seq) {
    // Si llegamos a una hoja (destino)
    if (is_destination(row, col)) {
        visited_leaf_nodes++;
        if (current_path_len < best_path_length) {
            best_path_length = current_path_len;
            best_path = current_path_grid;
            best_moves = current_moves_seq;
            path_positions = current_positions_seq; // Save the positions of the best path
        }
        return;
    }
    
    // Expandir el nodo actual
    for (auto it = steps_inc.begin(); it != steps_inc.end(); ++it) {
        visited_nodes++;
        
        Step step = it->first;
        tuple<int, int> inc = it->second;
        
        int new_row = row + get<0>(inc);
        int new_col = col + get<1>(inc);
        
        // Verificar si es factible
        bool factible = is_valid(new_row, new_col) && !current_path_grid[new_row][new_col];
        
        if (factible) {
            int path_len_to_new_node = current_path_len + 1;
            
            // Calculate Chebyshev distance from (new_row, new_col) to destination (n-1, m-1)
            int heuristic_val = chebyshev_distance(new_row, new_col, n - 1, m - 1);
            
            // Verificar si es prometedor using the heuristic
            // The path to the new node + heuristic estimate must be less than the best known path
            bool prometedor = (path_len_to_new_node + heuristic_val) < best_path_length;
            
            if (prometedor) {
                explored_nodes++;
                
                current_path_grid[new_row][new_col] = true;
                current_moves_seq.push_back(step);
                current_positions_seq.push_back({new_row, new_col});
                
                maze_bt(new_row, new_col, path_len_to_new_node, current_path_grid, current_moves_seq, current_positions_seq);
                
                current_positions_seq.pop_back();
                current_moves_seq.pop_back();
                current_path_grid[new_row][new_col] = false;
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
        cerr << "Error: can't open file: " << filename << endl;
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

// Mostrar el camino en formato 2D (implementación adaptada)
void mostrarCamino() {
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

// Mostrar el camino codificado (implementación adaptada)
void mostrarCaminoCodificado() {
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
    if (argc < 3) {
        cerr << "ERROR: missing filename." << endl;
        cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    string filename;
    bool print_coded = false, print_2D = false;
    
    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-p") == 0) {
            print_coded = true;
        } else if (strcmp(argv[i], "--p2D") == 0) {
            print_2D = true;
        } else {
            cerr << "ERROR: unknown option " << argv[i] << endl;
            cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename." << endl;
        cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }
    
    // Leer el laberinto
    if (!read_maze(filename)) {
        cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }
    
    // Inicializar estructuras para el algoritmo
    best_path.resize(n, vector<bool>(m, false));
    vector<vector<bool>> current_path_grid(n, vector<bool>(m, false));
    vector<Step> current_moves_seq;
    vector<Position> current_positions_seq; 
    
    // Medir el tiempo de ejecución
    auto start_time = chrono::high_resolution_clock::now();
    
    // Si la entrada es accesible, empezamos el algoritmo
    if (is_valid(0, 0)) {
        current_path_grid[0][0] = true;
        current_positions_seq.push_back({0, 0}); 
        maze_bt(0, 0, 1, current_path_grid, current_moves_seq, current_positions_seq);
    }
    
    // Calcular tiempo transcurrido
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
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
    if (print_2D) {
        mostrarCamino();
    }
    
    if (print_coded) {
        mostrarCaminoCodificado();
    }
    
    return 0;
}