#include <iostream>
#include <vector>

using namespace std;

// Tamaño del tablero
const int N = 9;

// Función para imprimir el tablero
void imprimirTablero(const vector<vector<int>>& tablero) {
    for (int r = 0; r < N; r++) {
        for (int d = 0; d < N; d++) {
            cout << tablero[r][d] << " ";
        }
        cout << endl;
    }
}

// Verifica si es seguro colocar el número en la posición dada
bool esSeguro(vector<vector<int>>& tablero, int fila, int col, int num) {
    // Verificar fila
    for (int x = 0; x < N; x++) {
        if (tablero[fila][x] == num) return false;
    }

    // Verificar columna
    for (int x = 0; x < N; x++) {
        if (tablero[x][col] == num) return false;
    }

    // Verificar sub-cuadrícula 3x3
    int inicioFila = fila - fila % 3;
    int inicioCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tablero[i + inicioFila][j + inicioCol] == num) return false;
        }
    }

    return true;
}

// Algoritmo de Backtracking
bool resolverSudoku(vector<vector<int>>& tablero) {
    int fila = -1;
    int col = -1;
    bool vacio = true;

    // Buscar la siguiente celda vacía
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tablero[i][j] == 0) {
                fila = i;
                col = j;
                vacio = false;
                break;
            }
        }
        if (!vacio) break;
    }

    // Si no hay celdas vacías, el Sudoku está resuelto
    if (vacio) return true;

    // Intentar números del 1 al 9
    for (int num = 1; num <= 9; num++) {
        if (esSeguro(tablero, fila, col, num)) {
            tablero[fila][col] = num;

            if (resolverSudoku(tablero)) return true;

            // Si no lleva a solución, resetear (Backtrack)
            tablero[fila][col] = 0;
        }
    }
    return false;
}

int main() {
    // 0 representa las celdas vacías de la imagen proporcionada
    vector<vector<int>> tablero = {
        {6, 9, 0, 0, 0, 0, 7, 0, 0},
        {0, 0, 0, 0, 9, 6, 0, 0, 0},
        {0, 8, 0, 7, 5, 3, 0, 9, 0},
        {0, 2, 0, 3, 7, 4, 5, 6, 1},
        {3, 6, 0, 0, 0, 5, 0, 2, 0},
        {0, 0, 0, 9, 6, 0, 3, 7, 8},
        {0, 0, 6, 0, 3, 1, 0, 8, 4},
        {0, 4, 5, 8, 0, 7, 6, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 5, 7}
    };

    if (resolverSudoku(tablero)) {
        cout << "Sudoku resuelto exitosamente:" << endl;
        imprimirTablero(tablero);
    } else {
        cout << "No se encontró una solución para este tablero." << endl;
    }

    return 0;
}