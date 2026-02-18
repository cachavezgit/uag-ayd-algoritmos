#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

// Estructura para representar un objeto
struct Objeto {
    int id;
    int valor;
    int peso;
    double ratio; // valor/peso para ordenar
};

// Estructura para representar un nodo en el árbol de búsqueda
struct Nodo {
    int nivel;           // Nivel en el árbol (índice del objeto)
    int valorActual;     // Valor acumulado
    int pesoActual;      // Peso acumulado
    double limiteSuperior; // Límite superior (bound)
    vector<bool> solucion; // Objetos incluidos
    
    // Operador para la cola de prioridad (max-heap por límite superior)
    bool operator<(const Nodo& otro) const {
        return limiteSuperior < otro.limiteSuperior;
    }
};

// Calcula el límite superior usando relajación fraccionaria
double calcularLimiteSuperior(const vector<Objeto>& objetos, int nivel, 
                            int valorActual, int pesoActual, int capacidad) {
    double limite = valorActual;
    int pesoRestante = capacidad - pesoActual;
    int n = objetos.size();
    
    // Agregar objetos completos mientras quepan
    for (int i = nivel; i < n && pesoRestante > 0; i++) {
        if (objetos[i].peso <= pesoRestante) {
            pesoRestante -= objetos[i].peso;
            limite += objetos[i].valor;
        } else {
            // Agregar fracción del objeto (relajación)
            limite += (double)objetos[i].valor * pesoRestante / objetos[i].peso;
            break;
        }
    }
    return limite;
}

// Algoritmo de Ramificación y Poda
void branchAndBound(vector<Objeto> objetos, int capacidad) {
    int n = objetos.size();
    
    // Ordenar objetos por ratio valor/peso (descendente)
    sort(objetos.begin(), objetos.end(), [](const Objeto& a, const Objeto& b) {
        return a.ratio > b.ratio;
    });
    
    cout << "\n=== OBJETOS ORDENADOS POR RATIO (valor/peso) ===" << endl;
    cout << "Orden\tID Orig\tValor\tPeso\tRatio" << endl;
    cout << "------------------------------------------------" << endl;
    for (int i = 0; i < n; i++) {
        cout << i+1 << "\t" << objetos[i].id << "\t" << objetos[i].valor 
             << "\t" << objetos[i].peso << "\t" << fixed << setprecision(2) 
             << objetos[i].ratio << endl;
    }
    
    // Cola de prioridad (max-heap por límite superior)
    priority_queue<Nodo> cola;
    
    // Nodo raíz
    Nodo raiz;
    raiz.nivel = 0;
    raiz.valorActual = 0;
    raiz.pesoActual = 0;
    raiz.solucion.resize(n, false);
    raiz.limiteSuperior = calcularLimiteSuperior(objetos, 0, 0, 0, capacidad);
    
    cola.push(raiz);
    
    int mejorValor = 0;
    vector<bool> mejorSolucion(n, false);
    int nodosProcesados = 0;
    int nodosPodados = 0;
    
    cout << "\n=== PROCESO DE RAMIFICACIÓN Y PODA ===" << endl;
    cout << "Límite superior inicial: " << raiz.limiteSuperior << endl;
    cout << "\nExplorando árbol de decisiones..." << endl;
    
    while (!cola.empty()) {
        Nodo actual = cola.top();
        cola.pop();
        nodosProcesados++;
        
        // Podar si el límite superior es menor o igual que el mejor valor conocido
        if (actual.limiteSuperior <= mejorValor) {
            nodosPodados++;
            continue;
        }
        
        // Si llegamos al final del árbol
        if (actual.nivel == n) {
            if (actual.valorActual > mejorValor) {
                mejorValor = actual.valorActual;
                mejorSolucion = actual.solucion;
            }
            continue;
        }
        
        int i = actual.nivel;
        
        // Rama IZQUIERDA: Incluir el objeto i
        if (actual.pesoActual + objetos[i].peso <= capacidad) {
            Nodo incluir;
            incluir.nivel = actual.nivel + 1;
            incluir.valorActual = actual.valorActual + objetos[i].valor;
            incluir.pesoActual = actual.pesoActual + objetos[i].peso;
            incluir.solucion = actual.solucion;
            incluir.solucion[i] = true;
            incluir.limiteSuperior = calcularLimiteSuperior(objetos, incluir.nivel, 
                                   incluir.valorActual, incluir.pesoActual, capacidad);
            
            // Actualizar mejor solución si es factible y mejor
            if (incluir.valorActual > mejorValor) {
                mejorValor = incluir.valorActual;
                mejorSolucion = incluir.solucion;
            }
            
            // Solo agregar si el límite superior es prometedora
            if (incluir.limiteSuperior > mejorValor) {
                cola.push(incluir);
            } else {
                nodosPodados++;
            }
        }
        
        // Rama DERECHA: No incluir el objeto i
        Nodo noIncluir;
        noIncluir.nivel = actual.nivel + 1;
        noIncluir.valorActual = actual.valorActual;
        noIncluir.pesoActual = actual.pesoActual;
        noIncluir.solucion = actual.solucion;
        noIncluir.solucion[i] = false;
        noIncluir.limiteSuperior = calcularLimiteSuperior(objetos, noIncluir.nivel, 
                                 noIncluir.valorActual, noIncluir.pesoActual, capacidad);
        
        // Solo agregar si el límite superior es prometedora
        if (noIncluir.limiteSuperior > mejorValor) {
            cola.push(noIncluir);
        } else {
            nodosPodados++;
        }
    }
    
    // Mostrar resultados
    cout << "\n=== RESULTADOS ===" << endl;
    cout << "Nodos procesados: " << nodosProcesados << endl;
    cout << "Nodos podados: " << nodosPodados << endl;
    
    cout << "\n--- SOLUCIÓN ÓPTIMA ---" << endl;
    cout << "Valor máximo: " << mejorValor << endl;
    
    int pesoTotal = 0;
    cout << "\nObjetos seleccionados:" << endl;
    cout << "ID\tValor\tPeso" << endl;
    cout << "------------------------" << endl;
    
    for (int i = 0; i < n; i++) {
        if (mejorSolucion[i]) {
            cout << objetos[i].id << "\t" << objetos[i].valor << "\t" 
                 << objetos[i].peso << endl;
            pesoTotal += objetos[i].peso;
        }
    }
    
    cout << "------------------------" << endl;
    cout << "TOTAL:\t" << mejorValor << "\t" << pesoTotal << endl;
    cout << "\nCapacidad de la mochila: " << capacidad << endl;
    cout << "Peso utilizado: " << pesoTotal << " (" 
         << fixed << setprecision(1) << (100.0 * pesoTotal / capacidad) << "%)" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "  PROBLEMA DE LA MOCHILA 0/1" << endl;
    cout << "  Método: Ramificación y Poda" << endl;
    cout << "  (Branch and Bound)" << endl;
    cout << "========================================" << endl;
    
    // Datos del problema
    int capacidad = 101;
    
    // Crear vector de objetos con los datos proporcionados
    vector<Objeto> objetos = {
        {1, 79, 85, 79.0/85},
        {2, 32, 26, 32.0/26},
        {3, 47, 48, 47.0/48},
        {4, 18, 21, 18.0/21},
        {5, 26, 22, 26.0/22},
        {6, 85, 95, 85.0/95},
        {7, 33, 43, 33.0/43},
        {8, 40, 45, 40.0/45},
        {9, 45, 55, 45.0/55},
        {10, 59, 52, 59.0/52}
    };
    
    cout << "\n=== DATOS DEL PROBLEMA ===" << endl;
    cout << "Capacidad de la mochila: " << capacidad << endl;
    cout << "\nObjetos disponibles:" << endl;
    cout << "ID\tValor\tPeso\tRatio (v/p)" << endl;
    cout << "----------------------------------------" << endl;
    
    for (const auto& obj : objetos) {
        cout << obj.id << "\t" << obj.valor << "\t" << obj.peso << "\t" 
             << fixed << setprecision(2) << obj.ratio << endl;
    }
    
    // Ejecutar el algoritmo
    branchAndBound(objetos, capacidad);
    
    return 0;
}