#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void rodCutting(int precio[], int n) {
    // dp[i] almacena el precio máximo para una cuerda de largo i
    vector<int> dp(n + 1, 0);
    // corte[i] almacena el primer corte óptimo para largo i
    vector<int> corte(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        int maxVal = -1;
        for (int j = 1; j <= i; j++) {
            if (precio[j] + dp[i - j] > maxVal) {
                maxVal = precio[j] + dp[i - j];
                corte[i] = j;
            }
        }
        dp[i] = maxVal;
    }

    // Mostrar tabla dp
    cout << "Tabla de programacion dinamica:" << endl;
    cout << "Largo\tPrecio Maximo" << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "\t" << dp[i] << endl;
    }

    // Precio máximo para largo n
    cout << "\nPrecio maximo para cuerda de largo " << n << ": " << dp[n] << endl;

    // Reconstruir la descomposición óptima
    cout << "Descomposicion optima: ";
    int largo = n;
    bool primero = true;
    while (largo > 0) {
        if (!primero) cout << " + ";
        cout << corte[largo];
        largo -= corte[largo];
        primero = false;
    }
    cout << endl;
}

int main() {
    // Índice 0 no se usa; precio[i] = precio de cuerda de largo i
    int precio[] = {0, 1, 4, 10, 12, 15, 20, 21, 32, 31, 41, 51};
    int n = 11; // largo de la cuerda

    rodCutting(precio, n);

    return 0;
}