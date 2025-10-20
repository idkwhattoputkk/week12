#!/usr/bin/env python3
"""
Script de demostración automatizada para algoritmos embarazosamente paralelos
Ejecuta todos los ejercicios y genera un reporte de rendimiento
"""

import time
import sys
import os
from datetime import datetime

# Importar todos los módulos de ejercicios
from image_filter import benchmark_image_filtering
from vector_summation import benchmark_vector_summation
from vector_search import benchmark_vector_search
from fall_simulation import benchmark_fall_simulation


def generate_performance_report():
    """Genera un reporte completo de rendimiento de todos los ejercicios"""
    
    print("=" * 80)
    print("REPORTE DE RENDIMIENTO - ALGORITMOS EMBARAZOSAMENTE PARALELOS")
    print("=" * 80)
    print(f"Fecha y hora: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"Sistema: {os.name}")
    print(f"Python: {sys.version}")
    print("=" * 80)
    
    exercises = [
        ("Filtro de Imágenes", benchmark_image_filtering),
        ("Suma de Vectores", benchmark_vector_summation),
        ("Búsqueda en Vectores", benchmark_vector_search),
        ("Simulación de Caída Libre", benchmark_fall_simulation)
    ]
    
    total_start_time = time.time()
    results = []
    
    for i, (name, benchmark_func) in enumerate(exercises, 1):
        print(f"\n{'='*20} EJERCICIO {i}: {name.upper()} {'='*20}")
        
        exercise_start_time = time.time()
        
        try:
            # Capturar la salida del benchmark
            benchmark_func()
            exercise_time = time.time() - exercise_start_time
            
            results.append({
                'exercise': name,
                'status': 'SUCCESS',
                'time': exercise_time,
                'error': None
            })
            
            print(f"\n✓ Ejercicio {i} completado exitosamente en {exercise_time:.4f} segundos")
            
        except Exception as e:
            exercise_time = time.time() - exercise_start_time
            results.append({
                'exercise': name,
                'status': 'ERROR',
                'time': exercise_time,
                'error': str(e)
            })
            
            print(f"\n✗ Ejercicio {i} falló después de {exercise_time:.4f} segundos")
            print(f"Error: {e}")
    
    total_time = time.time() - total_start_time
    
    # Resumen final
    print("\n" + "=" * 80)
    print("RESUMEN FINAL")
    print("=" * 80)
    
    successful_exercises = [r for r in results if r['status'] == 'SUCCESS']
    failed_exercises = [r for r in results if r['status'] == 'ERROR']
    
    print(f"Ejercicios ejecutados: {len(results)}")
    print(f"Ejercicios exitosos: {len(successful_exercises)}")
    print(f"Ejercicios fallidos: {len(failed_exercises)}")
    print(f"Tiempo total de ejecución: {total_time:.4f} segundos")
    
    if successful_exercises:
        avg_time = sum(r['time'] for r in successful_exercises) / len(successful_exercises)
        print(f"Tiempo promedio por ejercicio: {avg_time:.4f} segundos")
    
    print("\nDetalles por ejercicio:")
    for i, result in enumerate(results, 1):
        status_symbol = "✓" if result['status'] == 'SUCCESS' else "✗"
        print(f"  {status_symbol} Ejercicio {i}: {result['exercise']} - {result['time']:.4f}s")
        if result['error']:
            print(f"    Error: {result['error']}")
    
    print("\n" + "=" * 80)
    print("DEMOSTRACIÓN COMPLETADA")
    print("=" * 80)
    
    return results


def main():
    """Función principal del script de demostración"""
    
    if len(sys.argv) > 1 and sys.argv[1] == '--help':
        print("Script de demostración de algoritmos embarazosamente paralelos")
        print("\nUso:")
        print("  python demo.py           # Ejecutar demostración completa")
        print("  python demo.py --help    # Mostrar esta ayuda")
        print("\nEste script ejecuta todos los ejercicios y genera un reporte de rendimiento.")
        return
    
    try:
        results = generate_performance_report()
        
        # Determinar código de salida
        failed_count = len([r for r in results if r['status'] == 'ERROR'])
        sys.exit(failed_count)
        
    except KeyboardInterrupt:
        print("\n\nDemostración interrumpida por el usuario.")
        sys.exit(1)
    except Exception as e:
        print(f"\nError inesperado en la demostración: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
