/*
 * Ejercicio 5: Simulación de caída libre para múltiples objetos
 * Implementación paralela usando OpenMP para calcular tiempos de caída independientes
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <omp.h>
#include <iomanip>

using namespace std;

// Estructura para representar un objeto en caída libre
struct FallingObject {
    int id;
    double height;           // Altura inicial en metros
    double mass;             // Masa en kg
    double drag_coefficient; // Coeficiente de resistencia del aire
};

// Estructura para almacenar el resultado del cálculo de caída libre
struct FallResult {
    int object_id;
    double height;
    double fall_time;
    double final_velocity;
    double processing_time;
};

// Función para calcular el tiempo de caída libre para un objeto
FallResult calculateFallTime(const FallingObject& obj) {
    auto start_time = chrono::high_resolution_clock::now();
    
    // Constantes físicas
    const double g = 9.81;        // Aceleración gravitacional (m/s²)
    const double air_density = 1.225; // Densidad del aire (kg/m³)
    
    double height = obj.height;
    double mass = obj.mass;
    double drag_coeff = obj.drag_coefficient;
    
    // Cálculo simplificado del tiempo de caída considerando resistencia del aire
    // Para objetos esféricos, área proyectada aproximada
    double radius = pow(mass / (4.0/3.0 * M_PI * 1000), 1.0/3.0); // Asumiendo densidad del agua
    double area = M_PI * radius * radius;
    
    // Fuerza de resistencia del aire
    double drag_force = 0.5 * air_density * drag_coeff * area;
    
    // Ecuación simplificada para tiempo de caída con resistencia del aire
    // Usando aproximación numérica simple
    double dt = 0.001; // Paso de tiempo pequeño
    double t = 0;
    double v = 0;
    double y = height;
    
    while (y > 0) {
        // Fuerza neta (gravedad - resistencia del aire)
        double net_force = mass * g - drag_force * v * v;
        double acceleration = net_force / mass;
        
        // Actualizar velocidad y posición
        v += acceleration * dt;
        y -= v * dt;
        t += dt;
        
        // Evitar bucles infinitos
        if (t > 100) { // Máximo 100 segundos
            break;
        }
    }
    
    // Velocidad final aproximada
    double final_velocity = v;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double processing_time = duration.count() / 1000000.0; // Convertir a segundos
    
    return FallResult{
        obj.id,
        height,
        t,
        final_velocity,
        processing_time
    };
}

// Versión secuencial de simulación de caída libre
vector<FallResult> simulateFallSequential(const vector<FallingObject>& objects) {
    vector<FallResult> results;
    
    for (const auto& obj : objects) {
        results.push_back(calculateFallTime(obj));
    }
    
    return results;
}

// Versión paralela de simulación de caída libre usando OpenMP
vector<FallResult> simulateFallParallel(const vector<FallingObject>& objects) {
    vector<FallResult> results(objects.size());
    
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(objects.size()); ++i) {
        results[i] = calculateFallTime(objects[i]);
    }
    
    return results;
}

// Función para crear objetos de muestra
vector<FallingObject> createSampleObjects(int num_objects = 12) {
    vector<FallingObject> objects;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> height_dis(10.0, 200.0);
    uniform_real_distribution<double> mass_dis(0.1, 50.0);
    uniform_real_distribution<double> drag_dis(0.1, 1.5);
    
    for (int i = 0; i < num_objects; ++i) {
        FallingObject obj;
        obj.id = i + 1;
        obj.height = height_dis(gen);        // Altura entre 10 y 200 metros
        obj.mass = mass_dis(gen);            // Masa entre 0.1 y 50 kg
        obj.drag_coefficient = drag_dis(gen); // Coeficiente de resistencia
        
        objects.push_back(obj);
    }
    
    return objects;
}

// Función principal de benchmark
void benchmarkFallSimulation() {
    cout << "=== EJERCICIO 5: SIMULACIÓN DE CAÍDA LIBRE (C++ con OpenMP) ===" << endl << endl;
    
    // Crear objetos de muestra
    cout << "Creando objetos de muestra..." << endl;
    vector<FallingObject> objects = createSampleObjects(12);
    cout << "Se crearon " << objects.size() << " objetos para simular" << endl << endl;
    
    // Mostrar información de los objetos
    cout << "Objetos creados:" << endl;
    for (const auto& obj : objects) {
        cout << "  Objeto " << obj.id << ": altura=" << fixed << setprecision(1) << obj.height 
             << "m, masa=" << setprecision(2) << obj.mass 
             << "kg, drag=" << obj.drag_coefficient << endl;
    }
    cout << endl;
    
    // Benchmark versión secuencial
    cout << "Ejecutando versión secuencial..." << endl;
    auto start_time = chrono::high_resolution_clock::now();
    vector<FallResult> sequential_results = simulateFallSequential(objects);
    auto end_time = chrono::high_resolution_clock::now();
    auto sequential_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double sequential_time = sequential_duration.count() / 1000000.0;
    
    cout << "Tiempo secuencial: " << fixed << setprecision(4) << sequential_time << " segundos" << endl;
    cout << "Tiempo promedio por objeto: " << sequential_time / objects.size() << " segundos" << endl;
    
    // Calcular estadísticas secuenciales
    double avg_fall_time_seq = 0.0;
    double avg_velocity_seq = 0.0;
    for (const auto& result : sequential_results) {
        avg_fall_time_seq += result.fall_time;
        avg_velocity_seq += result.final_velocity;
    }
    avg_fall_time_seq /= sequential_results.size();
    avg_velocity_seq /= sequential_results.size();
    
    cout << "Tiempo promedio de caída (secuencial): " << fixed << setprecision(2) << avg_fall_time_seq << "s" << endl;
    cout << "Velocidad promedio final (secuencial): " << avg_velocity_seq << "m/s" << endl;
    
    // Benchmark versión paralela
    cout << endl << "Ejecutando versión paralela..." << endl;
    start_time = chrono::high_resolution_clock::now();
    vector<FallResult> parallel_results = simulateFallParallel(objects);
    end_time = chrono::high_resolution_clock::now();
    auto parallel_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double parallel_time = parallel_duration.count() / 1000000.0;
    
    cout << "Tiempo paralelo: " << parallel_time << " segundos" << endl;
    cout << "Tiempo promedio por objeto: " << parallel_time / objects.size() << " segundos" << endl;
    
    // Calcular estadísticas paralelas
    double avg_fall_time_par = 0.0;
    double avg_velocity_par = 0.0;
    for (const auto& result : parallel_results) {
        avg_fall_time_par += result.fall_time;
        avg_velocity_par += result.final_velocity;
    }
    avg_fall_time_par /= parallel_results.size();
    avg_velocity_par /= parallel_results.size();
    
    cout << "Tiempo promedio de caída (paralelo): " << avg_fall_time_par << "s" << endl;
    cout << "Velocidad promedio final (paralelo): " << avg_velocity_par << "m/s" << endl;
    
    // Verificar que los resultados sean iguales
    cout << endl << "Verificación de resultados:" << endl;
    double time_diff = abs(avg_fall_time_seq - avg_fall_time_par);
    double velocity_diff = abs(avg_velocity_seq - avg_velocity_par);
    cout << "Diferencia en tiempo promedio: " << scientific << time_diff << "s" << endl;
    cout << "Diferencia en velocidad promedio: " << velocity_diff << "m/s" << endl;
    
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
        cout << "  Objeto " << result.object_id << ": altura=" << fixed << setprecision(1) << result.height 
             << "m, tiempo_caída=" << setprecision(2) << result.fall_time 
             << "s, velocidad_final=" << result.final_velocity 
             << "m/s, tiempo_procesamiento=" << setprecision(4) << result.processing_time << "s" << endl;
    }
    
    cout << endl << "Objetos simulados exitosamente: " << parallel_results.size() << endl;
    cout << endl << "Nota: Los cálculos incluyen resistencia del aire para mayor realismo" << endl << endl;
}

int main() {
    // Configurar OpenMP
    omp_set_num_threads(omp_get_max_threads());
    
    cout << "OpenMP disponible: " << omp_get_max_threads() << " threads" << endl;
    cout << "Compilador OpenMP: " << _OPENMP << endl << endl;
    
    benchmarkFallSimulation();
    
    return 0;
}
