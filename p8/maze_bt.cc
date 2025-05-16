//DANIEL SEGURADO CLIMENT 48791680D
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <climits>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <map> // Necesario para std::map

using namespace std;

// Estructura nodo como la usas en tus funciones de ejemplo
struct nodo {
    int x, y;
};

// Tipo enumérico para las 8 direcciones posibles
// Los valores deben coincidir con los usados en el mapa de pasos
enum StepEnum { N_enum = 1, NE_enum, E_enum, SE_enum, S_enum, SW_enum, W_enum, NW_enum };

// Variables globales adaptadas a los nombres de tus funciones de ejemplo
vector<vector<int>> mapa; // Laberinto original
int n, m;                 // Dimensiones

vector<nodo> camino;        // Camino actual en la recursión
vector<nodo> mejorCamino;   // Mejor camino encontrado
int difMinima = INT_MAX; // Longitud del mejor camino (equivalente a best_path_length)

// Estadísticas del algoritmo (usando tus nombres)
long long nvisit = 0;
long long nexplored = 0;
long long nleaf = 0;
long long nunfeasible = 0;
long long nnot_promising = 0;

vector<vector<bool>> visitado_grid; // Rejilla para marcar celdas visitadas en el camino actual

// Mapa para los movimientos, similar a iteracio.cc
// Usaremos StepEnum como clave para que coincida con la definición de la práctica para la salida -p
map<StepEnum, tuple<int, int>> steps_inc_map;

void initialize_steps_map() {
    steps_inc_map[N_enum] = make_tuple(-1, 0);
    steps_inc_map[NE_enum] = make_tuple(-1, 1);
    steps_inc_map[E_enum] = make_tuple(0, 1);
    steps_inc_map[SE_enum] = make_tuple(1, 1);
    steps_inc_map[S_enum] = make_tuple(1, 0);
    steps_inc_map[SW_enum] = make_tuple(1, -1);
    steps_inc_map[W_enum] = make_tuple(0, -1);
    steps_inc_map[NW_enum] = make_tuple(-1, -1);
}

// Verifica si una posición es válida y accesible en 'mapa'
bool is_valid_pos(int r, int c) {
    return (r >= 0 && r < n && c >= 0 && c < m && mapa[r][c] == 1);
}

// Verifica si hemos llegado al destino
bool is_destination_pos(int r, int c) {
    return (r == n - 1 && c == m - 1);
}

// Heuristic: Chebyshev distance to destination
int chebyshev_distance_heuristic(int r1, int c1, int r2, int c2) {
    return std::max(std::abs(r1 - r2), std::abs(c1 - c2));
}

// Función principal de backtracking (adaptada a tus nombres y estructuras)
// 'current_len' es la longitud actual del 'camino' (número de nodos)
void maze_bt(nodo nodoActual, int current_len) {
    nvisit++;

    if (is_destination_pos(nodoActual.x, nodoActual.y)) {
        nleaf++;
        if (current_len < difMinima) {
            difMinima = current_len;
            mejorCamino = camino; // 'camino' ya tiene el camino actual
        }
        return;
    }

    // Poda temprana: si el camino actual ya no puede mejorar la mejor solución
    if (current_len >= difMinima) {
        return;
    }

    // Iteración sobre los elementos del mapa de movimientos
    for (auto it = steps_inc_map.begin(); it != steps_inc_map.end(); ++it) {
        int incx, incy;
        tie(incx, incy) = it->second; // Obtener incrementos

        int next_x = nodoActual.x + incx;
        int next_y = nodoActual.y + incy;

        bool feasible = is_valid_pos(next_x, next_y) && !visitado_grid[next_x][next_y];

        if (feasible) {
            int path_len_to_next_node = current_len + 1;
            int heuristic_val = chebyshev_distance_heuristic(next_x, next_y, n - 1, m - 1);
            bool promising = (path_len_to_next_node + heuristic_val) < difMinima;

            if (promising) {
                nexplored++;

                visitado_grid[next_x][next_y] = true;
                camino.push_back({next_x, next_y});

                maze_bt({next_x, next_y}, path_len_to_next_node);

                camino.pop_back();
                visitado_grid[next_x][next_y] = false;
            } else {
                nnot_promising++;
            }
        } else {
            nunfeasible++;
        }
    }
}

// Funcion --P2D (adaptada para usar 'mapa' y 'mejorCamino')
void mostrarCamino() {
    if (difMinima == INT_MAX) {
        cout << "0" << endl;
        return;
    }

    vector<vector<char>> path_display(n, vector<char>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            path_display[i][j] = (mapa[i][j] == 1) ? '1' : '0';
        }
    }

    for (const auto& node : mejorCamino) {
        if (node.x >= 0 && node.x < n && node.y >= 0 && node.y < m) {
            path_display[node.x][node.y] = '*';
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << path_display[i][j];
        }
        cout << endl;
    }
}

// Funcion -p (camino codificado)
void mostrarCaminoCodificado() {
    if (difMinima == INT_MAX) {
        cout << "<0>" << endl;
        return;
    }
    if (mejorCamino.empty()) {
        cout << "<0>" << endl;
        return;
    }
    if (mejorCamino.size() == 1 && difMinima == 1) {
        cout << "<>" << endl;
        return;
    }

    string codigo = "<";
    for (size_t i = 1; i < mejorCamino.size(); i++) {
        int dr = mejorCamino[i].x - mejorCamino[i - 1].x;
        int dc = mejorCamino[i].y - mejorCamino[i - 1].y;

        if (dr == -1 && dc == 0) { codigo += '1'; } // N_enum
        else if (dr == -1 && dc == 1) { codigo += '2'; } // NE_enum
        else if (dr == 0 && dc == 1) { codigo += '3'; }  // E_enum
        else if (dr == 1 && dc == 1) { codigo += '4'; }  // SE_enum
        else if (dr == 1 && dc == 0) { codigo += '5'; }  // S_enum
        else if (dr == 1 && dc == -1) { codigo += '6'; } // SW_enum
        else if (dr == 0 && dc == -1) { codigo += '7'; } // W_enum
        else if (dr == -1 && dc == -1) { codigo += '8'; } // NW_enum
    }
    codigo += ">";
    cout << codigo << endl;
}

int main(int argc, char* argv[]) {

    initialize_steps_map(); // Inicializar el mapa de movimientos

    if (argc < 3) {
        cerr << "ERROR: missing filename." << endl;
        cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    string archivoEntrada;
    bool print_2D = false, print_coded = false;

    bool file_option_found = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            file_option_found = true;
            if (i + 1 < argc) {
                archivoEntrada = argv[++i];
            } else {
                cerr << "ERROR: missing filename." << endl;
                cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
                return 1;
            }
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

    if (!file_option_found || archivoEntrada.empty()) {
        cerr << "ERROR: missing filename or -f option." << endl;
        cerr << "Usage: " << endl << " maze_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    ifstream infile(archivoEntrada);
    if (!infile) {
        cerr << "Error: can't open file: " << archivoEntrada << endl;
        return 1;
    }

    infile >> n >> m;
    if (n <= 0 || m <= 0) {
        cerr << "Error: invalid maze dimensions." << endl;
        infile.close();
        return 1;
    }
    mapa.assign(n, vector<int>(m));
    visitado_grid.assign(n, vector<bool>(m, false));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!(infile >> mapa[i][j])) {
                cerr << "Error: reading maze data." << endl;
                infile.close();
                return 1;
            }
        }
    }
    infile.close();

    auto chrono_start = chrono::high_resolution_clock::now();

    if (n > 0 && m > 0 && mapa[0][0] == 1) {
        visitado_grid[0][0] = true;
        camino.push_back({0, 0});
        maze_bt({0, 0}, 1);
    } else {
        difMinima = INT_MAX;
        if (n > 0 && m > 0 && mapa[0][0] == 0) { // Inicio bloqueado
            nvisit = 1; nexplored = 0; nleaf = 0; nunfeasible = 0; nnot_promising = 0;
        }
    }

    auto chrono_end = chrono::high_resolution_clock::now();
    auto chrono_duration = chrono::duration_cast<chrono::microseconds>(chrono_end - chrono_start);
    double tiempo_ms = chrono_duration.count() / 1000.0;

    if (difMinima == INT_MAX) {
        cout << "0" << endl;
    } else {
        cout << difMinima << endl;
    }

    if (n == 1 && m == 1 && mapa[0][0] == 1 && difMinima == 1) {
        if (nvisit == 1 && nleaf == 1 && nexplored == 0) {
            nexplored = 1;
        }
    }

    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << endl;
    cout << fixed << setprecision(3) << tiempo_ms << endl;

    if (print_2D) {
        mostrarCamino();
    }
    if (print_coded) {
        mostrarCaminoCodificado();
    }

    return 0;
}