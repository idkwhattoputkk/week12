"""
Ejercicio 3: Calcular la suma de elementos en múltiples vectores
Implementación paralela usando concurrent.futures para sumar elementos de vectores independientes
"""

import time
import random
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from typing import List, Tuple
import numpy as np


def sum_vector_elements(vector: List[float]) -> Tuple[int, float, float]:
    """
    Suma los elementos de un vector
    
    Args:
        vector: Lista de números a sumar
    
    Returns:
        Tupla con (índice_del_vector, suma, tiempo_procesamiento)
    """
    start_time = time.time()
    
    # Simular algo de trabajo computacional adicional
    # para hacer más evidente la diferencia entre secuencial y paralelo
    result = sum(vector)
    
    # Operación adicional para aumentar el tiempo de procesamiento
    for _ in range(1000):
        result += 0.0001
    
    processing_time = time.time() - start_time
    return len(vector), result, processing_time


def sum_vectors_sequential(vectors: List[List[float]]) -> List[Tuple[int, float, float]]:
    """
    Versión secuencial de suma de vectores
    
    Args:
        vectors: Lista de vectores a procesar
    
    Returns:
        Lista de resultados
    """
    results = []
    
    for i, vector in enumerate(vectors):
        result = sum_vector_elements(vector)
        results.append((i, result[1], result[2]))
    
    return results


def sum_vectors_parallel(vectors: List[List[float]], max_workers: int = None) -> List[Tuple[int, float, float]]:
    """
    Versión paralela de suma de vectores usando ProcessPoolExecutor
    
    Args:
        vectors: Lista de vectores a procesar
        max_workers: Número máximo de workers
    
    Returns:
        Lista de resultados
    """
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        results = list(executor.map(sum_vector_elements, vectors))
    
    return results


def create_sample_vectors(num_vectors: int = 10, vector_size: int = 100000) -> List[List[float]]:
    """
    Crea vectores de muestra para testing
    
    Args:
        num_vectors: Número de vectores a crear
        vector_size: Tamaño de cada vector
    
    Returns:
        Lista de vectores con números aleatorios
    """
    vectors = []
    
    for i in range(num_vectors):
        # Crear vector con números aleatorios
        vector = [random.uniform(-100, 100) for _ in range(vector_size)]
        vectors.append(vector)
    
    return vectors


def benchmark_vector_summation():
    """
    Ejecuta benchmark comparando versión secuencial vs paralela
    """
    print("=== EJERCICIO 3: SUMA DE ELEMENTOS EN MÚLTIPLES VECTORES ===\n")
    
    # Crear vectores de muestra
    print("Creando vectores de muestra...")
    vectors = create_sample_vectors(12, 50000)
    print(f"Se crearon {len(vectors)} vectores de {len(vectors[0])} elementos cada uno\n")
    
    # Benchmark versión secuencial
    print("Ejecutando versión secuencial...")
    start_time = time.time()
    sequential_results = sum_vectors_sequential(vectors)
    sequential_time = time.time() - start_time
    
    print(f"Tiempo secuencial: {sequential_time:.4f} segundos")
    print(f"Tiempo promedio por vector: {sequential_time/len(vectors):.4f} segundos")
    
    # Calcular suma total secuencial
    total_sum_sequential = sum(result[1] for result in sequential_results)
    print(f"Suma total (secuencial): {total_sum_sequential:.2f}")
    
    # Benchmark versión paralela
    print("\nEjecutando versión paralela...")
    start_time = time.time()
    parallel_results = sum_vectors_parallel(vectors)
    parallel_time = time.time() - start_time
    
    print(f"Tiempo paralelo: {parallel_time:.4f} segundos")
    print(f"Tiempo promedio por vector: {parallel_time/len(vectors):.4f} segundos")
    
    # Calcular suma total paralela
    total_sum_parallel = sum(result[1] for result in parallel_results)
    print(f"Suma total (paralelo): {total_sum_parallel:.2f}")
    
    # Verificar que los resultados sean iguales
    print(f"\nVerificación de resultados:")
    print(f"Diferencia en suma total: {abs(total_sum_sequential - total_sum_parallel):.10f}")
    
    # Calcular speedup
    speedup = sequential_time / parallel_time if parallel_time > 0 else 0
    print(f"\nSpeedup: {speedup:.2f}x")
    
    # Mostrar resultados detallados
    print("\nResultados detallados:")
    for i, (vector_size, vector_sum, proc_time) in enumerate(parallel_results):
        print(f"  Vector {i+1}: {vector_size} elementos, suma = {vector_sum:.2f}, tiempo = {proc_time:.4f}s")
    
    print(f"\nVectores procesados exitosamente: {len(parallel_results)}\n")


if __name__ == "__main__":
    benchmark_vector_summation()
