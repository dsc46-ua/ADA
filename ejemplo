//MARTÍN AZNAR GARCÍA, 51797315G

#include<iostream>
#include<fstream>
#include<vector>
#include<math.h> //Para min
#include <climits> // Para INT_MAX
#include <ctime>
#include <algorithm> // find, vector

// x =v^; y= <>

//Hacer cota optimista, calculo distancia desde el punto (linea recta) hasta el punto final
//Cota pesimista

//Pagina 14

#define MAX_VALOR 9999

using namespace std;

int dim_n;
int dim_m;

bool p2D=false;
bool b_file=false;
bool p=false;

int visitados = 0;
int explorados = 0;
int hojas_visitadas = 0;
int descartados_no_factibles = 0;
int descartados_no_prometedores = 0;

int mejorPeso=MAX_VALOR;
vector<pair<int, int>> mejorCamino;
vector<int> mejor_camino_pasos;

vector<pair<int, int>> camino;
vector<int> pasos;

const int orden[8] = {4, 3, 5, 2, 6, 8, 1, 7};
const int mov_x[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int mov_y[8] = {0, 1, 1, 1, 0, -1, -1, -1};

void dibujar_mapa(vector< vector<int>> &mapa){
    for(int i=0;i<dim_n;i++){
        for(int j=0;j<dim_m;j++){
            cout<<mapa[i][j]<<" ";
        }
        cout<<endl;
    }
}

void dibujar_p2D(vector< vector<int>> &mapa){
    for(int i=0;i<dim_n;i++){
        for(int j=0;j<dim_m;j++){
            pair<int,int> pos = {i,j};

            if(find(mejorCamino.begin(), mejorCamino.end(), pos) != mejorCamino.end()){
                cout<<"*";
            }
            else{
                cout<<mapa[i][j];
            }
        }
        cout<<endl;
    }
}

void mostrar_pasos(){
    cout<<"<";
    for(int i=0;i<mejor_camino_pasos.size();i++){
        cout<<mejor_camino_pasos[i]+1;
    }

    cout<<">"<<endl;
}

int cotaOptimista(int x, int y) {
    int dx = abs(dim_n-1 - x);
    int dy = abs(dim_m-1 - y);
    return max(dx,dy);
}

int cotaPesimista(int x, int y,vector< vector<int>> &mapa) {
    int nodos_posibles;
    for(int i=x;i<dim_n;i++){
        for(int j=y;j<dim_m;j++){
            if(mapa[i][j]!=0){
                nodos_posibles++;
            }
        }
    }
    return nodos_posibles;
}

void backtrack(int x, int y, vector< vector<int>> &mapa, vector< vector<bool>> &visitado, vector<pair<int,int>> &camino, vector<int> &pasos){
    

    if(x == dim_n-1 && y == dim_m-1){
        hojas_visitadas++;

        if( camino.size() < mejorPeso || camino.size() == mejorPeso && pasos.size() < mejor_camino_pasos.size()){
            mejorPeso = camino.size();
            mejorCamino = camino;
            mejor_camino_pasos = pasos;
        }

        return;
    }
    visitados++;
    /*int mayorCamino = camino.size() + cotaPesimista(x, y,mapa);
    if (mayorCamino >= mejorPeso) {
        descartados_no_prometedores++;
        return;
    }*/

    if(camino.size() >= mejorPeso){
        descartados_no_factibles++;
        return;
    }

    for(int i=0;i<8;i++){
        int sig_x = x + mov_x[orden[i]-1];
        int sig_y = y + mov_y[orden[i]-1];

        if(sig_x<0 || sig_y<0 || sig_x>=dim_n || sig_y>=dim_m || visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
            continue;
        }

        camino.emplace_back(sig_x,sig_y);
        visitado[sig_x][sig_y]=true;
        pasos.push_back(orden[i]-1);
        explorados++;

        int coste = camino.size();
        int menorCamino = coste + cotaOptimista(sig_x,sig_y);

        if(menorCamino < mejorPeso){
            backtrack(sig_x,sig_y, mapa, visitado, camino, pasos);
        }
        else{
            descartados_no_prometedores++;
        }

        visitado[sig_x][sig_y]=false;
        pasos.pop_back();
        camino.pop_back();
    }
}

//void backtrack(int x, int y, vector< vector<int>> &mapa, vector< vector<bool>> &visitado, vector<pair<int,int>> &camino, vector<int> &pasos);

/*void reguistro(int sig_x, int sig_y, vector< vector<int>> &mapa, vector< vector<bool>> &visitado, int pos){
    camino.emplace_back(sig_x,sig_y);
    visitado[sig_x][sig_y]=true;

    pasos.push_back(pos);
    explorados++;

    int coste = camino.size();
    int menorCamino = coste + cotaOptimista(sig_x,sig_y);

    if(menorCamino < mejorPeso){
        backtrack(sig_x,sig_y, mapa, visitado, camino, pasos);
    }
    else{
        descartados_no_prometedores++;
    }

    visitado[sig_x][sig_y]=false;
    pasos.pop_back();
    camino.pop_back();
}

void backtrack(int x, int y, vector< vector<int>> &mapa, vector< vector<bool>> &visitado, vector<pair<int,int>> &camino, vector<int> &pasos){
    visitados++;

    if(x == dim_n-1 && y == dim_m-1){
        hojas_visitadas++;
        if(camino.size() < mejorPeso){
            mejorPeso = camino.size();
            mejorCamino = camino;
            mejor_camino_pasos = pasos;
        }
        return;
    }

    int mayorCamino = camino.size() + cotaPesimista(x, y);
    if (mayorCamino >= mejorPeso) {
        descartados_no_prometedores++;
        return;
    }

    //PERPENDICULAR derecha-abajo 4
    int sig_x = x + 1;
    int sig_y = y +1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
            return;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,4);
        }
    }

    //ABAJO 5
    sig_x = x + 1;
    sig_y = y;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
            return;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,5);
        }
    }
    
    //DERECHO 3
    sig_x = x;
    sig_y = y + 1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
            return;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,3);
        }
    }

    //PERPENDICULAR derecha-arriba 2
    sig_x = x - 1;
    sig_y = y + 1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,2);
        }
    }
    
    //PERPENDICULAR izquierda-abajo 6
    sig_x = x + 1;
    sig_y = y - 1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,6);
        }
    }

    //PERPENDICULAR izquierda-arriba 8
    sig_x = x - 1;
    sig_y = y - 1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,8);
        }
    }
    
    //ARRIBA 1
    sig_x = x - 1;
    sig_y = y;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,1);
        }
    }

    //IZQUIERDA 7
    sig_x = x;
    sig_y = y - 1;
    if(sig_x>=0 && sig_y>=0 && sig_x<dim_n && sig_y<dim_m){
        if(visitado[sig_x][sig_y] || mapa[sig_x][sig_y]==0){
            descartados_no_factibles++;
        }
        else{
            reguistro(sig_x,sig_y,mapa,visitado,7);
        }
    }
}*/

double maze_bt(vector< vector<int>> &mapa){
    if(mapa[0][0]==0 || mapa[dim_n-1][dim_m-1]==0){
        mejorPeso = 0;
        return 0.00;
    }

    mejorPeso = INT_MAX;
    mejorCamino.clear();
    pasos.clear();
    mejor_camino_pasos.clear();

    vector<vector<bool>> visitado(dim_n, vector<bool>(dim_m, false));

    visitado[0][0]=true;
    camino.clear();
    camino.emplace_back(0, 0);
    visitados++;
    
    clock_t t_ini = clock();

    backtrack(0, 0, mapa, visitado, camino, pasos);

    clock_t t_fin = clock();

    return ((t_fin - t_ini) * 1000.00 / CLOCKS_PER_SEC);

}

void abrir_fichero(string file_name){

    ifstream file(file_name);

    if(file.is_open()){
        int num;

        file >> dim_n;
        file >> dim_m;

        vector< vector<int>> matriz(dim_n,vector<int>(dim_m));

        for(int i=0;i<dim_n;i++){
            for(int j=0;j<dim_m;j++){
                file>>matriz[i][j];
            }
        }

        int pos_init_x=0;
        int pos_init_y=0;

        vector< vector<int>> camino(dim_n,vector<int>(dim_m));
        vector< vector<int>> optimista(dim_n,vector<int>(dim_m));

        double tiempo = maze_bt(matriz);
        
        cout<<mejorPeso<<endl;

        cout<<visitados<<" "<<explorados<<" "<<hojas_visitadas<<" "<<descartados_no_factibles<<" "<<descartados_no_prometedores<<endl;

        cout<<tiempo<<endl;
        
        if(mejorPeso!=0){
            if(p2D){
                dibujar_p2D(matriz);
            }
        }
        
        if(p){
            if(mejorPeso==0){
                cout<<"<0>"<<endl;
            }
            else{
                mostrar_pasos();
            }
        }
    }
    else{
        cerr<<"ERROR: can't open file: "<<file_name<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
    }
}

int main(int argn,char* argv[]){

    string file;

    if(argn==1){
        cerr<<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
        return -1;
    }

    for(int i=1;i<argn;i++){
        string arg=argv[i];

        if(arg=="--p2D"){
            p2D=true;
        }
        else if(arg=="-p"){
            p=true;
        }
        else if(arg=="-f"){
            if(i!=argn-1){
                file=argv[++i];
                b_file=true;
            }
            else{
               // cerr<<"Error de argumento, falta [Nombre_archivo]: -f [Nombre_archivo]"<<endl;
               cerr<<"ERROR: missing filename."<<endl
                     <<"Usage:"<<endl
                     <<"maze [--p2D] [-p] -f file"<<endl;
                return -1;
            }
        }
        else{
            cerr<<"ERROR: unknow option "<<argv[i]<<"."<<endl
                <<"Usage:"<<endl
                <<"maze [--p2D] [-p] -f file"<<endl;
                return -1;
        }

    }
    if(!b_file){
        cerr<<"ERROR: missing filename."<<endl
            <<"Usage:"<<endl
            <<"maze [--p2D] [-p] -f file"<<endl;
        return -1;
    }

    abrir_fichero(file);

    return 0;
}