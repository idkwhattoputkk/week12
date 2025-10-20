"""
Ejercicio 1: Aplicar un filtro a múltiples imágenes
Implementación paralela usando concurrent.futures para procesar múltiples imágenes
"""

import os
import time
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from PIL import Image
import numpy as np
from typing import List, Tuple


def apply_grayscale_filter(image_path: str, output_path: str) -> Tuple[str, float]:
    """
    Aplica un filtro de escala de grises a una imagen
    
    Args:
        image_path: Ruta de la imagen de entrada
        output_path: Ruta donde guardar la imagen procesada
    
    Returns:
        Tupla con el nombre del archivo y tiempo de procesamiento
    """
    start_time = time.time()
    
    try:
        # Cargar imagen
        image = Image.open(image_path)
        
        # Convertir a escala de grises
        grayscale_image = image.convert('L')
        
        # Guardar imagen procesada
        grayscale_image.save(output_path)
        
        processing_time = time.time() - start_time
        return os.path.basename(image_path), processing_time
        
    except Exception as e:
        print(f"Error procesando {image_path}: {e}")
        return os.path.basename(image_path), 0.0


def apply_grayscale_filter_sequential(image_paths: List[str], output_dir: str) -> List[Tuple[str, float]]:
    """
    Versión secuencial del filtro de imágenes
    
    Args:
        image_paths: Lista de rutas de imágenes
        output_dir: Directorio de salida
    
    Returns:
        Lista de tuplas con resultados
    """
    results = []
    
    for image_path in image_paths:
        filename = os.path.basename(image_path)
        output_path = os.path.join(output_dir, f"grayscale_{filename}")
        result = apply_grayscale_filter(image_path, output_path)
        results.append(result)
    
    return results


def apply_grayscale_filter_parallel(image_paths: List[str], output_dir: str, max_workers: int = None) -> List[Tuple[str, float]]:
    """
    Versión paralela del filtro de imágenes usando ProcessPoolExecutor
    
    Args:
        image_paths: Lista de rutas de imágenes
        output_dir: Directorio de salida
        max_workers: Número máximo de workers (None = automático)
    
    Returns:
        Lista de tuplas con resultados
    """
    # Preparar rutas de salida
    output_paths = []
    for image_path in image_paths:
        filename = os.path.basename(image_path)
        output_path = os.path.join(output_dir, f"grayscale_{filename}")
        output_paths.append(output_path)
    
    # Procesar en paralelo
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        results = list(executor.map(apply_grayscale_filter, image_paths, output_paths))
    
    return results


def create_sample_images(num_images: int = 5, size: Tuple[int, int] = (200, 200)) -> List[str]:
    """
    Crea imágenes de muestra para testing
    
    Args:
        num_images: Número de imágenes a crear
        size: Tamaño de las imágenes
    
    Returns:
        Lista de rutas de las imágenes creadas
    """
    os.makedirs("sample_images", exist_ok=True)
    image_paths = []
    
    for i in range(num_images):
        # Crear imagen con colores aleatorios
        image_array = np.random.randint(0, 256, (size[1], size[0], 3), dtype=np.uint8)
        image = Image.fromarray(image_array)
        
        filename = f"sample_image_{i+1}.png"
        image_path = os.path.join("sample_images", filename)
        image.save(image_path)
        image_paths.append(image_path)
    
    return image_paths


def benchmark_image_filtering():
    """
    Ejecuta benchmark comparando versión secuencial vs paralela
    """
    print("=== EJERCICIO 1: FILTRO DE IMÁGENES ===\n")
    
    # Crear imágenes de muestra
    print("Creando imágenes de muestra...")
    image_paths = create_sample_images(8)
    print(f"Se crearon {len(image_paths)} imágenes de muestra\n")
    
    # Crear directorio de salida
    os.makedirs("output_images", exist_ok=True)
    
    # Benchmark versión secuencial
    print("Ejecutando versión secuencial...")
    start_time = time.time()
    sequential_results = apply_grayscale_filter_sequential(image_paths, "output_images")
    sequential_time = time.time() - start_time
    
    print(f"Tiempo secuencial: {sequential_time:.4f} segundos")
    print(f"Tiempo promedio por imagen: {sequential_time/len(image_paths):.4f} segundos")
    
    # Limpiar archivos de salida para la prueba paralela
    for _, _ in sequential_results:
        pass  # Los archivos ya están creados
    
    # Benchmark versión paralela
    print("\nEjecutando versión paralela...")
    start_time = time.time()
    parallel_results = apply_grayscale_filter_parallel(image_paths, "output_images")
    parallel_time = time.time() - start_time
    
    print(f"Tiempo paralelo: {parallel_time:.4f} segundos")
    print(f"Tiempo promedio por imagen: {parallel_time/len(image_paths):.4f} segundos")
    
    # Calcular speedup
    speedup = sequential_time / parallel_time if parallel_time > 0 else 0
    print(f"\nSpeedup: {speedup:.2f}x")
    print(f"Eficiencia: {(speedup / os.cpu_count()) * 100:.1f}%")
    
    # Mostrar resultados detallados
    print("\nResultados detallados:")
    for filename, proc_time in parallel_results:
        print(f"  {filename}: {proc_time:.4f}s")
    
    print(f"\nImágenes procesadas exitosamente: {len(parallel_results)}")
    print("Archivos guardados en: output_images/\n")


if __name__ == "__main__":
    benchmark_image_filtering()
