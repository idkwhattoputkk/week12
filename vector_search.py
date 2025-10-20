"""
Ejercicio 4: Buscar un valor en múltiples vectores
Implementación paralela usando concurrent.futures para buscar un valor en vectores independientes
"""

import time
import random
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from typing import List, Tuple, Optional
import numpy as np


def search_value_in_vector(vector: Tuple[List[int], int]) -> Tuple[int, bool, int, float]:
    """
    Busca un valor en un vector
    
    Args:
        vector: Tupla con (lista_de_números, valor_a_buscar)
    
    Returns:
        Tupla con (índice_del_vector, encontrado, posición_encontrada, tiempo_procesamiento)
    """
    numbers, target_value = vector
    start_time = time.time()
    
    # Búsqueda lineal
    found = False
    position = -1
    
    for i, num in enumerate(numbers):
        if num == target_value:
            found = True
            position = i
            break
    
    processing_time = time.time() - start_time
    return len(numbers), found, position, processing_time


def search_value_sequential(vectors: List[List[int]], target_value: int) -> List[Tuple[int, bool, int, float]]:
    """
    Versión secuencial de búsqueda en vectores
    
    Args:
        vectors: Lista de vectores donde buscar
        target_value: Valor a buscar
    
    Returns:
        Lista de resultados
    """
    results = []
    
    for i, vector in enumerate(vectors):
        result = search_value_in_vector((vector, target_value))
        results.append((i, result[1], result[2], result[3]))
    
    return results


def search_value_parallel(vectors: List[List[int]], target_value: int, max_workers: int = None) -> List[Tuple[int, bool, int, float]]:
    """
    Versión paralela de búsqueda en vectores usando ProcessPoolExecutor
    
    Args:
        vectors: Lista de vectores donde buscar
        target_value: Valor a buscar
        max_workers: Número máximo de workers
    
    Returns:
        Lista de resultados
    """
    # Preparar datos para procesamiento paralelo
    vector_data = [(vector, target_value) for vector in vectors]
    
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        results = list(executor.map(search_value_in_vector, vector_data))
    
    return results


def create_sample_vectors(num_vectors: int = 8, vector_size: int = 100000) -> List[List[int]]:
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
        vector = [random.randint(1, 1000) for _ in range(vector_size)]
        
        # Asegurar que algunos vectores contengan valores específicos para testing
        if i % 3 == 0:  # Cada tercer vector tendrá el valor 42
            vector[random.randint(0, vector_size-1)] = 42
        elif i % 3 == 1:  # Cada segundo vector tendrá el valor 100
            vector[random.randint(0, vector_size-1)] = 100
        
        vectors.append(vector)
    
    return vectors


def benchmark_vector_search():
    """
    Ejecuta benchmark comparando versión secuencial vs paralela
    """
    print("=== EJERCICIO 4: BÚSQUEDA EN MÚLTIPLES VECTORES ===\n")
    
    # Crear vectores de muestra
    print("Creando vectores de muestra...")
    vectors = create_sample_vectors(10, 75000)
    print(f"Se crearon {len(vectors)} vectores de {len(vectors[0])} elementos cada uno\n")
    
    # Valores a buscar
    search_values = [42, 100, 999, 1]
    
    for target_value in search_values:
        print(f"Buscando valor: {target_value}")
        print("-" * 40)
        
        # Benchmark versión secuencial
        print("Ejecutando versión secuencial...")
        start_time = time.time()
        sequential_results = search_value_sequential(vectors, target_value)
        sequential_time = time.time() - start_time
        
        print(f"Tiempo secuencial: {sequential_time:.4f} segundos")
        print(f"Tiempo promedio por vector: {sequential_time/len(vectors):.4f} segundos")
        
        # Contar resultados encontrados
        found_count_sequential = sum(1 for result in sequential_results if result[1])
        print(f"Valores encontrados (secuencial): {found_count_sequential}/{len(vectors)}")
        
        # Benchmark versión paralela
        print("\nEjecutando versión paralela...")
        start_time = time.time()
        parallel_results = search_value_parallel(vectors, target_value)
        parallel_time = time.time() - start_time
        
        print(f"Tiempo paralelo: {parallel_time:.4f} segundos")
        print(f"Tiempo promedio por vector: {parallel_time/len(vectors):.4f} segundos")
        
        # Contar resultados encontrados
        found_count_parallel = sum(1 for result in parallel_results if result[1])
        print(f"Valores encontrados (paralelo): {found_count_parallel}/{len(vectors)}")
        
        # Verificar que los resultados sean iguales
        print(f"\nVerificación de resultados:")
        print(f"Diferencia en cantidad encontrada: {abs(found_count_sequential - found_count_parallel)}")
        
        # Calcular speedup
        speedup = sequential_time / parallel_time if parallel_time > 0 else 0
        print(f"Speedup: {speedup:.2f}x")
        
        # Mostrar resultados detallados
        print("\nResultados detallados:")
        for i, (vector_idx, found, position, proc_time) in enumerate(parallel_results):
            status = f"encontrado en posición {position}" if found else "no encontrado"
            print(f"  Vector {i+1}: {status}, tiempo = {proc_time:.4f}s")
        
        print(f"\nVectores procesados exitosamente: {len(parallel_results)}")
        print("\n" + "="*60 + "\n")


if __name__ == "__main__":
    benchmark_vector_search()
