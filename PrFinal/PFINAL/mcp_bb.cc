// DANIEL SEGURADO CLIMENT 489791680D
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>
#include <ctime>
#include <cstring>

using namespace std;

struct Nodo {
    int x, y, dificultadAcumulada, estimacion;
    vector<int> camino;

    bool operator>(const Nodo& otro) const {
        return estimacion > otro.estimacion;
    }
};

const int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int n, m;
vector<vector<int>> mapa;
vector<vector<bool>> visitado;

int difMinima = INT_MAX;
int nvisit = 0, nexplored = 0, nleaf = 0, nunfeasible = 0, nnot_promising = 0;
int npromising_but_discarded = 0, nbest_solution_updated_from_leaves = 0, nbest_solution_updated_from_pessimistic_bound = 0;

clock_t t_inicial;
vector<int> mejorCamino;

bool is_valid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

int estimacion_heuristica(int x, int y) {
    return abs(n - 1 - x) + abs(m - 1 - y);
}

void mcp_bb() {
    priority_queue<Nodo, vector<Nodo>, greater<Nodo>> pq;
    pq.push({0, 0, mapa[0][0], mapa[0][0] + estimacion_heuristica(0, 0), {}});

    while (!pq.empty()) {
        Nodo actual = pq.top();
        pq.pop();
        nvisit++;

        if (visitado[actual.x][actual.y]) continue;
        visitado[actual.x][actual.y] = true;
        actual.camino.push_back((actual.x != 0 || actual.y != 0) ? 3 : 0); // agregar el movimiento inicial

        if (actual.x == n - 1 && actual.y == m - 1) {
            nleaf++;
            if (actual.dificultadAcumulada < difMinima) {
                difMinima = actual.dificultadAcumulada;
                mejorCamino = actual.camino;
                nbest_solution_updated_from_leaves++;
            }
            continue;
        }

        for (int i = 0; i < 8; i++) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];
            if (is_valid(nx, ny) && !visitado[nx][ny]) {
                int nuevaDificultad = actual.dificultadAcumulada + mapa[nx][ny];
                if (nuevaDificultad < difMinima) {
                    pq.push({nx, ny, nuevaDificultad, nuevaDificultad + estimacion_heuristica(nx, ny), actual.camino});
                    nexplored++;
                } else {
                    npromising_but_discarded++;
                }
            } else {
                nunfeasible++;
            }
        }
    }
}

void mostrarCamino() {
    vector<vector<char>> path_mapa(n, vector<char>(m, '.'));
    int x = 0, y = 0;
    path_mapa[x][y] = 'x';
    for (auto move : mejorCamino) {
        x += dx[move];
        y += dy[move];
        path_mapa[x][y] = 'x';
    }
    for (auto &row : path_mapa) {
        for (char c : row) cout << c;
        cout << '\n';
    }
    cout << difMinima << '\n';
}

void mostrarCaminoCodificado() {
    string codigo = "<";
    for (auto move : mejorCamino) {
        codigo += to_string(move);
    }
    codigo += ">";
    cout << codigo << '\n';
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: mcp_bb [-p] [--p2D] -f <input_file>\n";
        return 1;
    }

    bool showPath = false, showPath2D = false;
    string inputFile;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            showPath = true;
        } else if (strcmp(argv[i], "--p2D") == 0) {
            showPath2D = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                inputFile = argv[++i];
            } else {
                cerr << "Error: No input file specified\n";
                return 1;
            }
        }
    }

    if (inputFile.empty()) {
        cerr << "Error: No input file specified\n";
        return 1;
    }

    ifstream input(inputFile);
    if (!input.is_open()) {
        cerr << "Error: Cannot open file " << inputFile << "\n";
        return 1;
    }

    input >> n >> m;
    mapa.resize(n, vector<int>(m));
    visitado.resize(n, vector<bool>(m, false));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            input >> mapa[i][j];
        }
    }
    input.close();

    t_inicial = clock();
    mcp_bb();
    double tiempoCPU = (clock() - t_inicial) * 1000.0 / CLOCKS_PER_SEC;

    cout << difMinima << '\n';
    cout << nvisit << ' ' << nexplored << ' ' << nleaf << ' ' << nunfeasible << ' ' << nnot_promising << ' ' << npromising_but_discarded << ' ' << nbest_solution_updated_from_leaves << ' ' << nbest_solution_updated_from_pessimistic_bound << '\n';
    cout << tiempoCPU << '\n';

    if (showPath2D) {
        mostrarCamino();
    }

    if (showPath) {
        mostrarCaminoCodificado();
    }

    return 0;
}
