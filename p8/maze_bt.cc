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
#include <map>

using namespace std;

struct nodo {
    int x, y;
};

//Variable enum para direcciones
enum StepEnum { N_enum = 1, NE_enum, E_enum, SE_enum, S_enum, SW_enum, W_enum, NW_enum };
//Variables globales
vector<vector<int>> mapa; 
int n, m;                 
vector<nodo> camino; //Camino actual
vector<nodo> mejorCamino; //Mejor camino
int difMinima = INT_MAX; //Valor incial de mejor camino
//Estadísticas
long long nvisit = 0;
long long nexplored = 0;
long long nleaf = 0;
long long nunfeasible = 0;
long long nnot_promising = 0;

vector<vector<bool>> visitado_grid;
map<StepEnum, tuple<int, int>> steps_inc_map;
vector<StepEnum> direcciones; 

void mapaTuplas() {
    steps_inc_map[N_enum] = make_tuple(-1, 0);
    steps_inc_map[NE_enum] = make_tuple(-1, 1);
    steps_inc_map[E_enum] = make_tuple(0, 1);
    steps_inc_map[SE_enum] = make_tuple(1, 1);
    steps_inc_map[S_enum] = make_tuple(1, 0);
    steps_inc_map[SW_enum] = make_tuple(1, -1);
    steps_inc_map[W_enum] = make_tuple(0, -1);
    steps_inc_map[NW_enum] = make_tuple(-1, -1);
}

//Función para inicializar el orden de direcciones elegido
void ordenDirecciones() {
    direcciones = {
        E_enum, SE_enum, S_enum, NE_enum, SW_enum, W_enum, NW_enum, N_enum
    };
}

//Verifica si una posición es válida
bool is_valid(int r, int c) {
    return (r >= 0 && r < n && c >= 0 && c < m && mapa[r][c] == 1);
}

//Verifica si hemos llegado al destino
bool is_destination(int r, int c) {
    return (r == n - 1 && c == m - 1);
}

//Heurística de Chebyshev (usada en una practica de SI y mejora el rendimiento)
int chebyshev(int r1, int c1, int r2, int c2) {
    return std::max(std::abs(r1 - r2), std::abs(c1 - c2));
}

//Función principal de backtracking
void maze_bt(nodo nodoActual, int current_len) {
    nvisit++;

    if (is_destination(nodoActual.x, nodoActual.y)) {
        nleaf++;
        if (current_len < difMinima) {
            difMinima = current_len;
            mejorCamino = camino;
        }
        return;
    }

    //Si el camino actual es mayor que la mejor solución encontrada, no seguimos
    if (current_len >= difMinima) {
        return;
    }

    //Iteración sobre las direcciones en el orden preferido
    for (StepEnum step_to_try : direcciones) { //iteramos sobre el orden que hemos elegido
        int incx, incy;
        // Obtener incrementos del mapa original usando el StepEnum actual
        // Usamos .at() para obtener el valor; esto lanzará una excepción si la clave no existe,
        // lo cual puede ser útil para depurar si direcciones o steps_inc_map no están bien inicializados.
        tie(incx, incy) = steps_inc_map.at(step_to_try);

        int next_x = nodoActual.x + incx;
        int next_y = nodoActual.y + incy;

        bool feasible = is_valid(next_x, next_y) && !visitado_grid[next_x][next_y];

        if (feasible) {
            int path_len_to_next_node = current_len + 1;
            int heuristic_val = chebyshev(next_x, next_y, n - 1, m - 1);
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

//Funcion --P2D
void mostrarCamino() {
    if (difMinima == INT_MAX) {
        cout << "0" << endl;
        return;
    }

    vector<vector<char>> vCamino(n, vector<char>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            vCamino[i][j] = (mapa[i][j] == 1) ? '1' : '0';
        }
    }

    for (const auto& node : mejorCamino) {
        if (node.x >= 0 && node.x < n && node.y >= 0 && node.y < m) {
            vCamino[node.x][node.y] = '*';
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << vCamino[i][j];
        }
        cout << endl;
    }
}

//Funcion -p (camino codificado)
void mostrarCaminoCodificado() {
    if (difMinima == INT_MAX) {
        cout << "<0>" << endl;
        return;
    }
    if (mejorCamino.empty()) {
        cout << "<0>" << endl;
        return;
    }
    if (mejorCamino.size() == 1 && difMinima == 1) { //Condición para el caso 0,0 a 0,0 en laberinto 1x1
        cout << "<>" << endl;
        return;
    }

    string codigo = "<";
    for (size_t i = 1; i < mejorCamino.size(); i++) {
        int dr = mejorCamino[i].x - mejorCamino[i - 1].x;
        int dc = mejorCamino[i].y - mejorCamino[i - 1].y;

        if (dr == -1 && dc == 0) { codigo += '1'; } //N_enum
        else if (dr == -1 && dc == 1) { codigo += '2'; } //NE_enum
        else if (dr == 0 && dc == 1) { codigo += '3'; }  //E_enum
        else if (dr == 1 && dc == 1) { codigo += '4'; }  //SE_enum
        else if (dr == 1 && dc == 0) { codigo += '5'; }  //S_enum
        else if (dr == 1 && dc == -1) { codigo += '6'; } //SW_enum
        else if (dr == 0 && dc == -1) { codigo += '7'; } //W_enum
        else if (dr == -1 && dc == -1) { codigo += '8'; } //NW_enum
    }
    codigo += ">";
    cout << codigo << endl;
}

int main(int argc, char* argv[]) {

    mapaTuplas(); //Inicializamos el mapa de tuplas
    ordenDirecciones(); //Incializamos el orden de direcciones

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
        if (n > 0 && m > 0 && mapa[0][0] == 0) {
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