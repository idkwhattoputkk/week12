/*
 * Ejercicio 3: Calcular la suma de elementos en múltiples vectores
 * Implementación paralela usando OpenMP para sumar elementos de vectores independientes
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <iomanip>

using namespace std;

// Estructura para almacenar información de un vector
struct VectorData {
    int id;
    vector<double> data;
    double sum;
    double processing_time;
};

// Función para sumar elementos de un vector
void sumVectorElements(VectorData& vec_data) {
    auto start_time = chrono::high_resolution_clock::now();
    
    double sum = 0.0;
    
    // Suma los elementos del vector
    for (const auto& value : vec_data.data) {
        sum += value;
    }
    
    // Simular trabajo computacional adicional para hacer más evidente
    // la diferencia entre secuencial y paralelo
    for (int i = 0; i < 1000; ++i) {
        sum += 0.0001;
    }
    
    vec_data.sum = sum;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    vec_data.processing_time = duration.count() / 1000000.0; // Convertir a segundos
}

// Versión secuencial de suma de vectores
vector<VectorData> sumVectorsSequential(vector<VectorData>& vectors) {
    for (auto& vec : vectors) {
        sumVectorElements(vec);
    }
    return vectors;
}

// Versión paralela de suma de vectores usando OpenMP
vector<VectorData> sumVectorsParallel(vector<VectorData>& vectors) {
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(vectors.size()); ++i) {
        sumVectorElements(vectors[i]);
    }
    return vectors;
}

// Función para crear vectores de muestra
vector<VectorData> createSampleVectors(int num_vectors = 12, int vector_size = 50000) {
    vector<VectorData> vectors;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-100.0, 100.0);
    
    for (int i = 0; i < num_vectors; ++i) {
        VectorData vec_data;
        vec_data.id = i + 1;
        
        // Crear vector con números aleatorios
        vec_data.data.reserve(vector_size);
        for (int j = 0; j < vector_size; ++j) {
            vec_data.data.push_back(dis(gen));
        }
        
        vectors.push_back(vec_data);
    }
    
    return vectors;
}

// Función principal de benchmark
void benchmarkVectorSummation() {
    cout << "=== EJERCICIO 3: SUMA DE ELEMENTOS EN MÚLTIPLES VECTORES (C++ con OpenMP) ===" << endl << endl;
    
    // Crear vectores de muestra
    cout << "Creando vectores de muestra..." << endl;
    vector<VectorData> vectors = createSampleVectors(12, 50000);
    cout << "Se crearon " << vectors.size() << " vectores de " << vectors[0].data.size() << " elementos cada uno" << endl << endl;
    
    // Benchmark versión secuencial
    cout << "Ejecutando versión secuencial..." << endl;
    vector<VectorData> sequential_vectors = vectors;
    auto start_time = chrono::high_resolution_clock::now();
    vector<VectorData> sequential_results = sumVectorsSequential(sequential_vectors);
    auto end_time = chrono::high_resolution_clock::now();
    auto sequential_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double sequential_time = sequential_duration.count() / 1000000.0;
    
    cout << "Tiempo secuencial: " << fixed << setprecision(4) << sequential_time << " segundos" << endl;
    cout << "Tiempo promedio por vector: " << sequential_time / vectors.size() << " segundos" << endl;
    
    // Calcular suma total secuencial
    double total_sum_sequential = 0.0;
    for (const auto& result : sequential_results) {
        total_sum_sequential += result.sum;
    }
    cout << "Suma total (secuencial): " << fixed << setprecision(2) << total_sum_sequential << endl;
    
    // Benchmark versión paralela
    cout << endl << "Ejecutando versión paralela..." << endl;
    vector<VectorData> parallel_vectors = vectors;
    start_time = chrono::high_resolution_clock::now();
    vector<VectorData> parallel_results = sumVectorsParallel(parallel_vectors);
    end_time = chrono::high_resolution_clock::now();
    auto parallel_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double parallel_time = parallel_duration.count() / 1000000.0;
    
    cout << "Tiempo paralelo: " << parallel_time << " segundos" << endl;
    cout << "Tiempo promedio por vector: " << parallel_time / vectors.size() << " segundos" << endl;
    
    // Calcular suma total paralela
    double total_sum_parallel = 0.0;
    for (const auto& result : parallel_results) {
        total_sum_parallel += result.sum;
    }
    cout << "Suma total (paralelo): " << total_sum_parallel << endl;
    
    // Verificar que los resultados sean iguales
    cout << endl << "Verificación de resultados:" << endl;
    cout << "Diferencia en suma total: " << scientific << abs(total_sum_sequential - total_sum_parallel) << endl;
    
    // Calcular speedup
    double speedup = (parallel_time > 0) ? sequential_time / parallel_time : 0;
    cout << endl << "Speedup: " << fixed << setprecision(2) << speedup << "x" << endl;
    
    int num_threads = omp_get_max_threads();
    double efficiency = (speedup / num_threads) * 100;
    cout << "Eficiencia: " << efficiency << "%" << endl;
    cout << "Número de threads utilizados: " << num_threads << endl;
    
    // Mostrar resultados detallados
    cout << endl << "Resultados detallados:" << endl;
    for (const auto& result : parallel_results) {
        cout << "  Vector " << result.id << ": " << result.data.size() 
             << " elementos, suma = " << fixed << setprecision(2) << result.sum 
             << ", tiempo = " << result.processing_time << "s" << endl;
    }
    
    cout << endl << "Vectores procesados exitosamente: " << parallel_results.size() << endl << endl;
}

int main() {
    // Configurar OpenMP
    omp_set_num_threads(omp_get_max_threads());
    
    cout << "OpenMP disponible: " << omp_get_max_threads() << " threads" << endl;
    cout << "Compilador OpenMP: " << _OPENMP << endl << endl;
    
    benchmarkVectorSummation();
    
    return 0;
}
