//DANIEL SEGURADO CLIMENT 48791680D 
#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <ctime>
#include <cstring>

using namespace std;

struct nodo {
    int x, y, difficulty;
};

const int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int n, m;
vector<vector<int>> mapa;
vector<vector<bool>> visitado;
vector<nodo> camino;
vector<nodo> mejorCamino;

int difMinima = INT_MAX;
int nvisit = 0, nexplored = 0, nleaf = 0, nunfeasible = 0, nnot_promising = 0;

clock_t t_inicial;

bool is_valid(int x, int y) {
    if (x >= 0 && x < n && y >= 0 && y < m) {
        return true;
    } else {
        return false;
    }
}

bool is_feasible(int x, int y) {
    if (is_valid(x, y) && !visitado[x][y]) {
        return true;
    } else {
        return false;
    }
}

bool is_promising(int x, int y, int difActual) {
    if (difActual < difMinima) {
        return true;
    } else {
        return false;
    }
}


//Funcion principal backtrack
void mcp_bt(nodo nodoActual, int difActual) {
    nvisit++;
    if (nodoActual.x == n - 1 && nodoActual.y == m - 1) {
        nleaf++;
        if (difActual < difMinima) {
            difMinima = difActual;
            mejorCamino = camino;
        }
        return;
    }

    if (difActual >= difMinima) {
        nunfeasible++;
        return;
    }

    for (int i = 0; i < 8; i++) {
        int nx = nodoActual.x + dx[i];
        int ny = nodoActual.y + dy[i];
        if (is_feasible(nx, ny) && is_promising(nx, ny, difActual + mapa[nx][ny])) {
            nexplored++;
            visitado[nx][ny] = true;
            camino.push_back({nx, ny, mapa[nx][ny]});
            mcp_bt({nx, ny, mapa[nx][ny]}, difActual + mapa[nx][ny]);
            camino.pop_back();
            visitado[nx][ny] = false;
        } else {
            nnot_promising++;
        }
    }
}

//Funcion --P2D
void mostrarCamino() {
    vector<vector<char>> path_mapa(n, vector<char>(m, '.'));

    for (size_t i = 0; i < mejorCamino.size(); ++i) {
        int x = mejorCamino[i].x;
        int y = mejorCamino[i].y;
        path_mapa[x][y] = 'x';
    }
    for (size_t i = 0; i < path_mapa.size(); ++i) {
        for (size_t j = 0; j < path_mapa[i].size(); ++j) {
            cout << path_mapa[i][j];
        }
        cout << '\n';
    }
    cout << difMinima << '\n';
}


//Funcion -p (camino codificado)
void mostrarCaminoCodificado() {
    string codigo = "<";
    for (size_t i = 1; i < mejorCamino.size(); i++) {
        int dx = mejorCamino[i].x - mejorCamino[i-1].x;
        int dy = mejorCamino[i].y - mejorCamino[i-1].y;
        if (dx == -1 && dy == -1) {codigo += '8';}
        else if (dx == -1 && dy == 0) {codigo += '1';}
        else if (dx == -1 && dy == 1) {codigo += '2';}
        else if (dx == 0 && dy == 1) {codigo += '3';}
        else if (dx == 1 && dy == 1) {codigo += '4';}
        else if (dx == 1 && dy == 0) {codigo += '5';}
        else if (dx == 1 && dy == -1) {codigo += '6';}
        else if (dx == 0 && dy == -1) {codigo += '7';}
    }
    codigo += ">";
    cout << codigo << '\n';
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage: " << endl << " mcp_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    string archivoEntrada;
    bool print_2D = false, print_coded = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            archivoEntrada = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-p") == 0) {
            print_coded = true;
        } else if (strcmp(argv[i], "--p2D") == 0) {
            print_2D = true;
        } else {
            cerr << "ERROR: unknown option " << argc << endl;
            cerr << "Usage: " << endl << " mcp_bt [ -p] [ --p2D] -f file" << endl;
            return 1;
        }
    }

    if (archivoEntrada.empty()) {
        cerr << "ERROR: missing filename." << endl;
        cerr << "Usage: " << endl << " mcp_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    ifstream infile(archivoEntrada);
    if (!infile) {
        cerr << "Error: can’t open file:  " << archivoEntrada << endl;
        cerr << "Usage: " << endl << " mcp_bt [ -p] [ --p2D] -f file" << endl;
        return 1;
    }

    infile >> n >> m;
    mapa.resize(n, vector<int>(m));
    visitado.resize(n, vector<bool>(m, false));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            infile >> mapa[i][j];
        }
    }

    infile.close();

    t_inicial = clock();
    visitado[0][0] = true;
    camino.push_back({0, 0, mapa[0][0]});
    mcp_bt({0, 0, mapa[0][0]}, mapa[0][0]);
    double tiempo = (clock() - t_inicial) / (double)CLOCKS_PER_SEC * 1000;

    cout << difMinima << '\n';
    cout << nvisit << ' ' << nexplored << ' ' << nleaf << ' ' << nunfeasible << ' ' << nnot_promising << '\n';
    cout << tiempo << '\n';

    if (print_2D) {
        mostrarCamino();
    }
    if (print_coded) {
        mostrarCaminoCodificado();
    }

    return 0;
}
