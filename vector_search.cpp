/*
 * Ejercicio 4: Buscar un valor en múltiples vectores
 * Implementación paralela usando OpenMP para buscar un valor en vectores independientes
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <iomanip>

using namespace std;

// Estructura para almacenar información de búsqueda en un vector
struct SearchData {
    int vector_id;
    vector<int> data;
    int target_value;
    bool found;
    int position;
    double processing_time;
};

// Función para buscar un valor en un vector
void searchValueInVector(SearchData& search_data) {
    auto start_time = chrono::high_resolution_clock::now();
    
    // Búsqueda lineal
    search_data.found = false;
    search_data.position = -1;
    
    for (size_t i = 0; i < search_data.data.size(); ++i) {
        if (search_data.data[i] == search_data.target_value) {
            search_data.found = true;
            search_data.position = static_cast<int>(i);
            break;
        }
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    search_data.processing_time = duration.count() / 1000000.0; // Convertir a segundos
}

// Versión secuencial de búsqueda en vectores
vector<SearchData> searchValueSequential(vector<SearchData>& search_data) {
    for (auto& data : search_data) {
        searchValueInVector(data);
    }
    return search_data;
}

// Versión paralela de búsqueda en vectores usando OpenMP
vector<SearchData> searchValueParallel(vector<SearchData>& search_data) {
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(search_data.size()); ++i) {
        searchValueInVector(search_data[i]);
    }
    return search_data;
}

// Función para crear vectores de muestra
vector<SearchData> createSampleVectors(int num_vectors = 10, int vector_size = 75000) {
    vector<SearchData> search_data;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(1, 1000);
    
    for (int i = 0; i < num_vectors; ++i) {
        SearchData data;
        data.vector_id = i + 1;
        
        // Crear vector con números aleatorios
        data.data.reserve(vector_size);
        for (int j = 0; j < vector_size; ++j) {
            data.data.push_back(dis(gen));
        }
        
        // Asegurar que algunos vectores contengan valores específicos para testing
        if (i % 3 == 0) {  // Cada tercer vector tendrá el valor 42
            data.data[dis(gen) % vector_size] = 42;
        } else if (i % 3 == 1) {  // Cada segundo vector tendrá el valor 100
            data.data[dis(gen) % vector_size] = 100;
        }
        
        search_data.push_back(data);
    }
    
    return search_data;
}

// Función principal de benchmark
void benchmarkVectorSearch() {
    cout << "=== EJERCICIO 4: BÚSQUEDA EN MÚLTIPLES VECTORES (C++ con OpenMP) ===" << endl << endl;
    
    // Crear vectores de muestra
    cout << "Creando vectores de muestra..." << endl;
    vector<SearchData> search_data = createSampleVectors(10, 75000);
    cout << "Se crearon " << search_data.size() << " vectores de " << search_data[0].data.size() << " elementos cada uno" << endl << endl;
    
    // Valores a buscar
    vector<int> search_values = {42, 100, 999, 1};
    
    for (int target_value : search_values) {
        cout << "Buscando valor: " << target_value << endl;
        cout << string(40, '-') << endl;
        
        // Configurar datos de búsqueda
        vector<SearchData> current_search_data = search_data;
        for (auto& data : current_search_data) {
            data.target_value = target_value;
            data.found = false;
            data.position = -1;
        }
        
        // Benchmark versión secuencial
        cout << "Ejecutando versión secuencial..." << endl;
        vector<SearchData> sequential_data = current_search_data;
        auto start_time = chrono::high_resolution_clock::now();
        vector<SearchData> sequential_results = searchValueSequential(sequential_data);
        auto end_time = chrono::high_resolution_clock::now();
        auto sequential_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        double sequential_time = sequential_duration.count() / 1000000.0;
        
        cout << "Tiempo secuencial: " << fixed << setprecision(4) << sequential_time << " segundos" << endl;
        cout << "Tiempo promedio por vector: " << sequential_time / search_data.size() << " segundos" << endl;
        
        // Contar resultados encontrados
        int found_count_sequential = 0;
        for (const auto& result : sequential_results) {
            if (result.found) found_count_sequential++;
        }
        cout << "Valores encontrados (secuencial): " << found_count_sequential << "/" << search_data.size() << endl;
        
        // Benchmark versión paralela
        cout << endl << "Ejecutando versión paralela..." << endl;
        vector<SearchData> parallel_data = current_search_data;
        start_time = chrono::high_resolution_clock::now();
        vector<SearchData> parallel_results = searchValueParallel(parallel_data);
        end_time = chrono::high_resolution_clock::now();
        auto parallel_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        double parallel_time = parallel_duration.count() / 1000000.0;
        
        cout << "Tiempo paralelo: " << parallel_time << " segundos" << endl;
        cout << "Tiempo promedio por vector: " << parallel_time / search_data.size() << " segundos" << endl;
        
        // Contar resultados encontrados
        int found_count_parallel = 0;
        for (const auto& result : parallel_results) {
            if (result.found) found_count_parallel++;
        }
        cout << "Valores encontrados (paralelo): " << found_count_parallel << "/" << search_data.size() << endl;
        
        // Verificar que los resultados sean iguales
        cout << endl << "Verificación de resultados:" << endl;
        cout << "Diferencia en cantidad encontrada: " << abs(found_count_sequential - found_count_parallel) << endl;
        
        // Calcular speedup
        double speedup = (parallel_time > 0) ? sequential_time / parallel_time : 0;
        cout << "Speedup: " << fixed << setprecision(2) << speedup << "x" << endl;
        
        int num_threads = omp_get_max_threads();
        double efficiency = (speedup / num_threads) * 100;
        cout << "Eficiencia: " << efficiency << "%" << endl;
        cout << "Número de threads utilizados: " << num_threads << endl;
        
        // Mostrar resultados detallados
        cout << endl << "Resultados detallados:" << endl;
        for (const auto& result : parallel_results) {
            string status = result.found ? 
                "encontrado en posición " + to_string(result.position) : 
                "no encontrado";
            cout << "  Vector " << result.vector_id << ": " << status 
                 << ", tiempo = " << result.processing_time << "s" << endl;
        }
        
        cout << endl << "Vectores procesados exitosamente: " << parallel_results.size() << endl;
        cout << endl << string(60, '=') << endl << endl;
    }
}

int main() {
    // Configurar OpenMP
    omp_set_num_threads(omp_get_max_threads());
    
    cout << "OpenMP disponible: " << omp_get_max_threads() << " threads" << endl;
    cout << "Compilador OpenMP: " << _OPENMP << endl << endl;
    
    benchmarkVectorSearch();
    
    return 0;
}
