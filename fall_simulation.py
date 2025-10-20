"""
Ejercicio 5: Simulación de caída libre para múltiples objetos
Implementación paralela usando concurrent.futures para calcular tiempos de caída independientes
"""

import time
import random
import math
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from typing import List, Tuple, NamedTuple
import numpy as np


class FallingObject(NamedTuple):
    """Representa un objeto en caída libre"""
    id: int
    height: float  # Altura inicial en metros
    mass: float    # Masa en kg
    drag_coefficient: float  # Coeficiente de resistencia del aire


class FallResult(NamedTuple):
    """Resultado del cálculo de caída libre"""
    object_id: int
    height: float
    fall_time: float
    final_velocity: float
    processing_time: float


def calculate_fall_time(object_data: FallingObject) -> FallResult:
    """
    Calcula el tiempo de caída libre para un objeto
    
    Args:
        object_data: Datos del objeto en caída libre
    
    Returns:
        Resultado del cálculo
    """
    start_time = time.time()
    
    # Constantes físicas
    g = 9.81  # Aceleración gravitacional (m/s²)
    air_density = 1.225  # Densidad del aire (kg/m³)
    
    height = object_data.height
    mass = object_data.mass
    drag_coeff = object_data.drag_coefficient
    
    # Cálculo simplificado del tiempo de caída considerando resistencia del aire
    # Para objetos esféricos, área proyectada aproximada
    radius = (mass / (4/3 * math.pi * 1000))**(1/3)  # Asumiendo densidad del agua
    area = math.pi * radius**2
    
    # Fuerza de resistencia del aire
    drag_force = 0.5 * air_density * drag_coeff * area
    
    # Ecuación simplificada para tiempo de caída con resistencia del aire
    # Usando aproximación numérica simple
    dt = 0.001  # Paso de tiempo pequeño
    t = 0
    v = 0
    y = height
    
    while y > 0:
        # Fuerza neta (gravedad - resistencia del aire)
        net_force = mass * g - drag_force * v**2
        acceleration = net_force / mass
        
        # Actualizar velocidad y posición
        v += acceleration * dt
        y -= v * dt
        t += dt
        
        # Evitar bucles infinitos
        if t > 100:  # Máximo 100 segundos
            break
    
    # Velocidad final aproximada
    final_velocity = v
    
    processing_time = time.time() - start_time
    
    return FallResult(
        object_id=object_data.id,
        height=height,
        fall_time=t,
        final_velocity=final_velocity,
        processing_time=processing_time
    )


def simulate_fall_sequential(objects: List[FallingObject]) -> List[FallResult]:
    """
    Versión secuencial de simulación de caída libre
    
    Args:
        objects: Lista de objetos a simular
    
    Returns:
        Lista de resultados
    """
    results = []
    
    for obj in objects:
        result = calculate_fall_time(obj)
        results.append(result)
    
    return results


def simulate_fall_parallel(objects: List[FallingObject], max_workers: int = None) -> List[FallResult]:
    """
    Versión paralela de simulación de caída libre usando ProcessPoolExecutor
    
    Args:
        objects: Lista de objetos a simular
        max_workers: Número máximo de workers
    
    Returns:
        Lista de resultados
    """
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        results = list(executor.map(calculate_fall_time, objects))
    
    return results


def create_sample_objects(num_objects: int = 10) -> List[FallingObject]:
    """
    Crea objetos de muestra para testing
    
    Args:
        num_objects: Número de objetos a crear
    
    Returns:
        Lista de objetos en caída libre
    """
    objects = []
    
    for i in range(num_objects):
        # Generar parámetros aleatorios realistas
        height = random.uniform(10, 200)  # Altura entre 10 y 200 metros
        mass = random.uniform(0.1, 50)    # Masa entre 0.1 y 50 kg
        drag_coeff = random.uniform(0.1, 1.5)  # Coeficiente de resistencia
        
        obj = FallingObject(
            id=i + 1,
            height=height,
            mass=mass,
            drag_coefficient=drag_coeff
        )
        objects.append(obj)
    
    return objects


def benchmark_fall_simulation():
    """
    Ejecuta benchmark comparando versión secuencial vs paralela
    """
    print("=== EJERCICIO 5: SIMULACIÓN DE CAÍDA LIBRE ===\n")
    
    # Crear objetos de muestra
    print("Creando objetos de muestra...")
    objects = create_sample_objects(12)
    print(f"Se crearon {len(objects)} objetos para simular\n")
    
    # Mostrar información de los objetos
    print("Objetos creados:")
    for obj in objects:
        print(f"  Objeto {obj.id}: altura={obj.height:.1f}m, masa={obj.mass:.2f}kg, drag={obj.drag_coefficient:.2f}")
    print()
    
    # Benchmark versión secuencial
    print("Ejecutando versión secuencial...")
    start_time = time.time()
    sequential_results = simulate_fall_sequential(objects)
    sequential_time = time.time() - start_time
    
    print(f"Tiempo secuencial: {sequential_time:.4f} segundos")
    print(f"Tiempo promedio por objeto: {sequential_time/len(objects):.4f} segundos")
    
    # Calcular estadísticas secuenciales
    avg_fall_time_seq = sum(result.fall_time for result in sequential_results) / len(sequential_results)
    avg_velocity_seq = sum(result.final_velocity for result in sequential_results) / len(sequential_results)
    
    print(f"Tiempo promedio de caída (secuencial): {avg_fall_time_seq:.2f}s")
    print(f"Velocidad promedio final (secuencial): {avg_velocity_seq:.2f}m/s")
    
    # Benchmark versión paralela
    print("\nEjecutando versión paralela...")
    start_time = time.time()
    parallel_results = simulate_fall_parallel(objects)
    parallel_time = time.time() - start_time
    
    print(f"Tiempo paralelo: {parallel_time:.4f} segundos")
    print(f"Tiempo promedio por objeto: {parallel_time/len(objects):.4f} segundos")
    
    # Calcular estadísticas paralelas
    avg_fall_time_par = sum(result.fall_time for result in parallel_results) / len(parallel_results)
    avg_velocity_par = sum(result.final_velocity for result in parallel_results) / len(parallel_results)
    
    print(f"Tiempo promedio de caída (paralelo): {avg_fall_time_par:.2f}s")
    print(f"Velocidad promedio final (paralelo): {avg_velocity_par:.2f}m/s")
    
    # Verificar que los resultados sean iguales
    print(f"\nVerificación de resultados:")
    time_diff = abs(avg_fall_time_seq - avg_fall_time_par)
    velocity_diff = abs(avg_velocity_seq - avg_velocity_par)
    print(f"Diferencia en tiempo promedio: {time_diff:.6f}s")
    print(f"Diferencia en velocidad promedio: {velocity_diff:.6f}m/s")
    
    # Calcular speedup
    speedup = sequential_time / parallel_time if parallel_time > 0 else 0
    print(f"\nSpeedup: {speedup:.2f}x")
    
    # Mostrar resultados detallados
    print("\nResultados detallados:")
    for result in parallel_results:
        print(f"  Objeto {result.object_id}: altura={result.height:.1f}m, "
              f"tiempo_caída={result.fall_time:.2f}s, "
              f"velocidad_final={result.final_velocity:.2f}m/s, "
              f"tiempo_procesamiento={result.processing_time:.4f}s")
    
    print(f"\nObjetos simulados exitosamente: {len(parallel_results)}")
    print("\nNota: Los cálculos incluyen resistencia del aire para mayor realismo\n")


if __name__ == "__main__":
    benchmark_fall_simulation()
