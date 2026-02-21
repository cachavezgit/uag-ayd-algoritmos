#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
    int n = 10;
    int W = 140; // Capacidad máxima de la mochila

    // Objetos: índice 1 a 10
    int valor[] = {0, 79, 32, 47, 18, 26, 85, 33, 40, 45, 59};
    int peso[]  = {0, 85, 26, 48, 21, 22, 95, 43, 45, 55, 52};

    // Tabla DP: dp[i][w] = máximo valor usando objetos 1..i con capacidad w
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Llenado de la tabla
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            // No tomar el objeto i
            dp[i][w] = dp[i - 1][w];
            // Tomar el objeto i (si cabe)
            if (peso[i] <= w) {
                dp[i][w] = max(dp[i][w], dp[i - 1][w - peso[i]] + valor[i]);
            }
        }
    }

    // El valor óptimo
    cout << "=============================================" << endl;
    cout << " PROBLEMA DE LA MOCHILA - Prog. Dinamica" << endl;
    cout << "=============================================" << endl;
    cout << "Capacidad maxima: " << W << " unidades" << endl;
    cout << endl;
    cout << "Valor optimo: " << dp[n][W] << endl;

    // Rastreo de objetos seleccionados (backtracking)
    vector<int> seleccionados;
    int w = W;
    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            seleccionados.push_back(i);
            w -= peso[i];
        }
    }

    cout << endl;
    cout << "Objetos seleccionados:" << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(10) << "Objeto" << setw(10) << "Valor" << setw(10) << "Peso" << endl;
    cout << "----------------------------------------------" << endl;

    int totalValor = 0, totalPeso = 0;
    for (int i = seleccionados.size() - 1; i >= 0; i--) {
        int obj = seleccionados[i];
        cout << left << setw(10) << obj << setw(10) << valor[obj] << setw(10) << peso[obj] << endl;
        totalValor += valor[obj];
        totalPeso += peso[obj];
    }

    cout << "----------------------------------------------" << endl;
    cout << left << setw(10) << "TOTAL" << setw(10) << totalValor << setw(10) << totalPeso << endl;
    cout << "=============================================" << endl;

    // Imprimir tabla DP (opcional, parte relevante)
    cout << endl << "Tabla DP (ultimas filas, capacidades seleccionadas):" << endl;
    cout << "dp[" << n << "][" << W << "] = " << dp[n][W] << endl;

    return 0;
}