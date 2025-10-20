/*
 * Programa principal para ejecutar todos los ejercicios de algoritmos embarazosamente paralelos
 * Implementación en C++ con OpenMP
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <omp.h>

// Declaraciones de funciones de los ejercicios
void benchmarkImageFiltering();
void benchmarkVectorSummation();
void benchmarkVectorSearch();
void benchmarkFallSimulation();

// Función para imprimir el encabezado del programa
void printHeader() {
    cout << string(80, '=') << endl;
    cout << "ALGORITMOS EMBARAZOSAMENTE PARALELOS - DEMOSTRACIÓN COMPLETA (C++ con OpenMP)" << endl;
    cout << string(80, '=') << endl;
    cout << "Este programa demuestra la implementación de algoritmos embarazosamente" << endl;
    cout << "paralelos usando C++ y OpenMP para optimización de rendimiento." << endl;
    cout << string(80, '=') << endl;
    cout << endl;
}

// Función para imprimir el menú de ejercicios disponibles
void printExerciseMenu() {
    cout << "EJERCICIOS DISPONIBLES:" << endl;
    cout << "1. Filtro de imágenes en paralelo" << endl;
    cout << "2. Suma de elementos en múltiples vectores" << endl;
    cout << "3. Búsqueda en múltiples vectores" << endl;
    cout << "4. Simulación de caída libre para múltiples objetos" << endl;
    cout << "5. Ejecutar todos los ejercicios" << endl;
    cout << "0. Salir" << endl;
    cout << endl;
}

// Función para ejecutar el ejercicio especificado
void runExercise(int exercise_number) {
    auto start_time = chrono::high_resolution_clock::now();
    
    try {
        switch (exercise_number) {
            case 1:
                benchmarkImageFiltering();
                break;
            case 2:
                benchmarkVectorSummation();
                break;
            case 3:
                benchmarkVectorSearch();
                break;
            case 4:
                benchmarkFallSimulation();
                break;
            case 5:
                runAllExercises();
                break;
            default:
                cout << "Número de ejercicio inválido." << endl;
                return;
        }
        
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        double total_time = duration.count() / 1000000.0;
        
        cout << "Tiempo total de ejecución: " << fixed << setprecision(4) << total_time << " segundos" << endl;
        
    } catch (const exception& e) {
        cout << "Error ejecutando ejercicio " << exercise_number << ": " << e.what() << endl;
    }
}

// Función para ejecutar todos los ejercicios en secuencia
void runAllExercises() {
    cout << "EJECUTANDO TODOS LOS EJERCICIOS" << endl;
    cout << string(50, '=') << endl;
    
    vector<pair<int, string>> exercises = {
        {1, "Filtro de imágenes"},
        {2, "Suma de vectores"},
        {3, "Búsqueda en vectores"},
        {4, "Simulación de caída libre"}
    };
    
    auto total_start_time = chrono::high_resolution_clock::now();
    
    for (const auto& exercise : exercises) {
        int exercise_num = exercise.first;
        const string& exercise_name = exercise.second;
        
        cout << endl << "Iniciando ejercicio " << exercise_num << ": " << exercise_name << endl;
        cout << string(50, '-') << endl;
        
        auto exercise_start_time = chrono::high_resolution_clock::now();
        runExercise(exercise_num);
        auto exercise_end_time = chrono::high_resolution_clock::now();
        auto exercise_duration = chrono::duration_cast<chrono::microseconds>(exercise_end_time - exercise_start_time);
        double exercise_time = exercise_duration.count() / 1000000.0;
        
        cout << "Ejercicio " << exercise_num << " completado en " << fixed << setprecision(4) << exercise_time << " segundos" << endl;
        cout << string(50, '=') << endl;
    }
    
    auto total_end_time = chrono::high_resolution_clock::now();
    auto total_duration = chrono::duration_cast<chrono::microseconds>(total_end_time - total_start_time);
    double total_time = total_duration.count() / 1000000.0;
    
    cout << endl << "TODOS LOS EJERCICIOS COMPLETADOS" << endl;
    cout << "Tiempo total: " << fixed << setprecision(4) << total_time << " segundos" << endl;
}

// Función para mostrar información del sistema
void showSystemInfo() {
    cout << "INFORMACIÓN DEL SISTEMA:" << endl;
    cout << "OpenMP disponible: " << omp_get_max_threads() << " threads" << endl;
    cout << "Versión OpenMP: " << _OPENMP << endl;
    cout << "Threads configurados: " << omp_get_num_threads() << endl;
    cout << endl;
}

// Modo interactivo para seleccionar ejercicios
void interactiveMode() {
    int choice;
    
    while (true) {
        printExerciseMenu();
        
        cout << "Seleccione un ejercicio (0-5): ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada inválida. Por favor, ingrese un número del 0 al 5." << endl;
            continue;
        }
        
        if (choice == 0) {
            cout << "¡Hasta luego!" << endl;
            break;
        } else if (choice >= 1 && choice <= 5) {
            cout << endl << "Ejecutando ejercicio " << choice << "..." << endl;
            runExercise(choice);
            
            cout << endl << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
            cout << endl << string(80, '=') << endl << endl;
        } else {
            cout << "Opción inválida. Por favor, seleccione un número del 0 al 5." << endl;
        }
    }
}

// Función principal del programa
int main(int argc, char* argv[]) {
    printHeader();
    showSystemInfo();
    
    // Configurar OpenMP
    omp_set_num_threads(omp_get_max_threads());
    
    // Verificar argumentos de línea de comandos
    if (argc > 1) {
        try {
            int exercise_num = stoi(argv[1]);
            if (exercise_num >= 1 && exercise_num <= 5) {
                cout << "Ejecutando ejercicio " << exercise_num << " desde línea de comandos..." << endl;
                runExercise(exercise_num);
            } else {
                cout << "Número de ejercicio inválido. Use números del 1 al 5." << endl;
                return 1;
            }
        } catch (const invalid_argument& e) {
            cout << "Argumento inválido. Use un número del 1 al 5." << endl;
            return 1;
        }
    } else {
        // Modo interactivo
        interactiveMode();
    }
    
    return 0;
}
