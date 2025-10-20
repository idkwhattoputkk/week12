/*
 * Ejercicio 1: Aplicar un filtro a múltiples imágenes
 * Implementación paralela usando OpenMP para procesar múltiples imágenes
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <random>
#include <omp.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Estructura para almacenar información de una imagen
struct ImageData {
    string filename;
    Mat image;
    double processing_time;
};

// Función para aplicar filtro de escala de grises a una imagen
void applyGrayscaleFilter(ImageData& img_data) {
    auto start_time = chrono::high_resolution_clock::now();
    
    // Convertir a escala de grises
    Mat grayscale;
    cvtColor(img_data.image, grayscale, COLOR_BGR2GRAY);
    
    // Guardar imagen procesada
    string output_filename = "output_images/grayscale_" + img_data.filename;
    imwrite(output_filename, grayscale);
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    img_data.processing_time = duration.count() / 1000000.0; // Convertir a segundos
}

// Versión secuencial del filtro de imágenes
vector<ImageData> applyGrayscaleFilterSequential(const vector<ImageData>& images) {
    vector<ImageData> results = images;
    
    for (auto& img : results) {
        applyGrayscaleFilter(img);
    }
    
    return results;
}

// Versión paralela del filtro de imágenes usando OpenMP
vector<ImageData> applyGrayscaleFilterParallel(const vector<ImageData>& images) {
    vector<ImageData> results = images;
    
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(results.size()); ++i) {
        applyGrayscaleFilter(results[i]);
    }
    
    return results;
}

// Función para crear imágenes de muestra
vector<ImageData> createSampleImages(int num_images = 8, int width = 200, int height = 200) {
    vector<ImageData> images;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    // Crear directorio de imágenes de muestra
    system("mkdir -p sample_images");
    
    for (int i = 0; i < num_images; ++i) {
        // Crear imagen con colores aleatorios
        Mat image(height, width, CV_8UC3);
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image.at<Vec3b>(y, x) = Vec3b(dis(gen), dis(gen), dis(gen));
            }
        }
        
        string filename = "sample_image_" + to_string(i + 1) + ".png";
        string filepath = "sample_images/" + filename;
        imwrite(filepath, image);
        
        ImageData img_data;
        img_data.filename = filename;
        img_data.image = image.clone();
        images.push_back(img_data);
    }
    
    return images;
}

// Función principal de benchmark
void benchmarkImageFiltering() {
    cout << "=== EJERCICIO 1: FILTRO DE IMÁGENES (C++ con OpenMP) ===" << endl << endl;
    
    // Crear imágenes de muestra
    cout << "Creando imágenes de muestra..." << endl;
    vector<ImageData> images = createSampleImages(8);
    cout << "Se crearon " << images.size() << " imágenes de muestra" << endl << endl;
    
    // Crear directorio de salida
    system("mkdir -p output_images");
    
    // Benchmark versión secuencial
    cout << "Ejecutando versión secuencial..." << endl;
    auto start_time = chrono::high_resolution_clock::now();
    vector<ImageData> sequential_results = applyGrayscaleFilterSequential(images);
    auto end_time = chrono::high_resolution_clock::now();
    auto sequential_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double sequential_time = sequential_duration.count() / 1000000.0;
    
    cout << "Tiempo secuencial: " << fixed << setprecision(4) << sequential_time << " segundos" << endl;
    cout << "Tiempo promedio por imagen: " << sequential_time / images.size() << " segundos" << endl;
    
    // Limpiar archivos de salida para la prueba paralela
    system("rm -f output_images/*");
    
    // Benchmark versión paralela
    cout << endl << "Ejecutando versión paralela..." << endl;
    start_time = chrono::high_resolution_clock::now();
    vector<ImageData> parallel_results = applyGrayscaleFilterParallel(images);
    end_time = chrono::high_resolution_clock::now();
    auto parallel_duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    double parallel_time = parallel_duration.count() / 1000000.0;
    
    cout << "Tiempo paralelo: " << parallel_time << " segundos" << endl;
    cout << "Tiempo promedio por imagen: " << parallel_time / images.size() << " segundos" << endl;
    
    // Calcular speedup
    double speedup = (parallel_time > 0) ? sequential_time / parallel_time : 0;
    cout << endl << "Speedup: " << speedup << "x" << endl;
    
    int num_threads = omp_get_max_threads();
    double efficiency = (speedup / num_threads) * 100;
    cout << "Eficiencia: " << efficiency << "%" << endl;
    cout << "Número de threads utilizados: " << num_threads << endl;
    
    // Mostrar resultados detallados
    cout << endl << "Resultados detallados:" << endl;
    for (const auto& result : parallel_results) {
        cout << "  " << result.filename << ": " << result.processing_time << "s" << endl;
    }
    
    cout << endl << "Imágenes procesadas exitosamente: " << parallel_results.size() << endl;
    cout << "Archivos guardados en: output_images/" << endl << endl;
}

int main() {
    // Configurar OpenMP
    omp_set_num_threads(omp_get_max_threads());
    
    cout << "OpenMP disponible: " << omp_get_max_threads() << " threads" << endl;
    cout << "Compilador OpenMP: " << _OPENMP << endl << endl;
    
    benchmarkImageFiltering();
    
    return 0;
}
