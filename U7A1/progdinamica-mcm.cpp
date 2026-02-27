#include <iostream>
#include <vector>
#include <climits>
#include <string>
using namespace std;

// Dimensiones de las matrices
// p = [5, 10, 3, 12, 5, 50, 6]
// Matriz A1: 5x10, A2: 10x3, A3: 3x12, A4: 12x5, A5: 5x50, A6: 50x6

int n; // número de matrices
vector<vector<int>> m; // tabla de costos mínimos
vector<vector<int>> s; // tabla de particiones óptimas

// ──────────────────────────────────────────────
// Función que construye la parentización óptima
// ──────────────────────────────────────────────
string parentizacion(int i, int j) {
    if (i == j)
        return "A" + to_string(i);
    return "(" + parentizacion(i, s[i][j]) + " x " + parentizacion(s[i][j] + 1, j) + ")";
}

// ──────────────────────────────────────────────
// Imprime la tabla completa (m o s)
// ──────────────────────────────────────────────
void imprimirTabla(const vector<vector<int>>& tabla, const string& nombre) {
    cout << "\nTabla " << nombre << ":\n";
    cout << string(60, '-') << endl;
    cout << "  i\\j ";
    for (int j = 1; j <= n; j++)
        printf("%8d", j);
    cout << endl;
    cout << string(60, '-') << endl;

    for (int i = 1; i <= n; i++) {
        printf("  %d   ", i);
        for (int j = 1; j <= n; j++) {
            if (j < i)
                printf("%8s", "-");
            else
                printf("%8d", tabla[i][j]);
        }
        cout << endl;
    }
    cout << string(60, '-') << endl;
}

int main() {
    vector<int> p = {5, 10, 3, 12, 5, 50, 6};
    n = p.size() - 1; // 6 matrices

    // Inicializar tablas
    m.assign(n + 1, vector<int>(n + 1, 0));
    s.assign(n + 1, vector<int>(n + 1, 0));

    cout << "============================================================" << endl;
    cout << "   MULTIPLICACIÓN ENCADENADA DE MATRICES" << endl;
    cout << "   Programación Dinámica" << endl;
    cout << "============================================================" << endl;

    cout << "\nDimensiones p = [5, 10, 3, 12, 5, 50, 6]" << endl;
    cout << "\nMatrices:" << endl;
    for (int i = 1; i <= n; i++)
        cout << "  A" << i << ": " << p[i-1] << " x " << p[i] << endl;

    // ──────────────────────────────────────────────
    // Algoritmo de Programación Dinámica
    // ──────────────────────────────────────────────
    // l = longitud de la subcadena (número de matrices)
    // Caso base: l=1 → m[i][i] = 0 (ya está inicializado)

    cout << "\n============================================================" << endl;
    cout << "   PROCESO DE LLENADO (por longitud de cadena)" << endl;
    cout << "============================================================" << endl;

    for (int l = 2; l <= n; l++) { // longitud de la cadena
        cout << "\n--- Longitud l = " << l << " ---" << endl;

        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            m[i][j] = INT_MAX;

            cout << "\n  Calculando m[" << i << "][" << j << "]:" << endl;

            for (int k = i; k < j; k++) {
                int q = m[i][k] + m[k+1][j] + p[i-1] * p[k] * p[j];

                cout << "    k=" << k << ": m[" << i << "][" << k << "] + m["
                     << k+1 << "][" << j << "] + " << p[i-1] << "*" << p[k]
                     << "*" << p[j] << " = " << m[i][k] << " + " << m[k+1][j]
                     << " + " << p[i-1] * p[k] * p[j] << " = " << q;

                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                    cout << "  ← NUEVO MÍNIMO (k=" << k << ")";
                }
                cout << endl;
            }

            cout << "  → m[" << i << "][" << j << "] = " << m[i][j]
                 << ", s[" << i << "][" << j << "] = " << s[i][j] << endl;
        }
    }

    // ──────────────────────────────────────────────
    // Resultados
    // ──────────────────────────────────────────────
    cout << "\n============================================================" << endl;
    cout << "   TABLAS FINALES" << endl;
    cout << "============================================================" << endl;

    imprimirTabla(m, "m (costos mínimos)");
    imprimirTabla(s, "s (particiones óptimas)");

    cout << "\n============================================================" << endl;
    cout << "   RESULTADO FINAL" << endl;
    cout << "============================================================" << endl;

    cout << "\n  Mínimo de multiplicaciones escalares: " << m[1][n] << endl;
    cout << "\n  Parentización óptima: " << parentizacion(1, n) << endl;

    cout << "\n============================================================" << endl;

    return 0;
}