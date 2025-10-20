"""
Programa principal para ejecutar todos los ejercicios de algoritmos embarazosamente paralelos
"""

import sys
import os
import time
from typing import List

# Importar todos los módulos de ejercicios
from image_filter import benchmark_image_filtering
from vector_summation import benchmark_vector_summation
from vector_search import benchmark_vector_search
from fall_simulation import benchmark_fall_simulation


def print_header():
    """Imprime el encabezado del programa"""
    print("=" * 80)
    print("ALGORITMOS EMBARAZOSAMENTE PARALELOS - DEMOSTRACIÓN COMPLETA")
    print("=" * 80)
    print("Este programa demuestra la implementación de algoritmos embarazosamente")
    print("paralelos usando Python y concurrent.futures para optimización de rendimiento.")
    print("=" * 80)
    print()


def print_exercise_menu():
    """Imprime el menú de ejercicios disponibles"""
    print("EJERCICIOS DISPONIBLES:")
    print("1. Filtro de imágenes en paralelo")
    print("2. Suma de elementos en múltiples vectores")
    print("3. Búsqueda en múltiples vectores")
    print("4. Simulación de caída libre para múltiples objetos")
    print("5. Ejecutar todos los ejercicios")
    print("0. Salir")
    print()


def run_exercise(exercise_number: int):
    """
    Ejecuta el ejercicio especificado
    
    Args:
        exercise_number: Número del ejercicio a ejecutar
    """
    start_time = time.time()
    
    try:
        if exercise_number == 1:
            benchmark_image_filtering()
        elif exercise_number == 2:
            benchmark_vector_summation()
        elif exercise_number == 3:
            benchmark_vector_search()
        elif exercise_number == 4:
            benchmark_fall_simulation()
        elif exercise_number == 5:
            run_all_exercises()
        else:
            print("Número de ejercicio inválido.")
            return
        
        total_time = time.time() - start_time
        print(f"Tiempo total de ejecución: {total_time:.4f} segundos")
        
    except Exception as e:
        print(f"Error ejecutando ejercicio {exercise_number}: {e}")
        import traceback
        traceback.print_exc()


def run_all_exercises():
    """Ejecuta todos los ejercicios en secuencia"""
    print("EJECUTANDO TODOS LOS EJERCICIOS")
    print("=" * 50)
    
    exercises = [
        (1, "Filtro de imágenes"),
        (2, "Suma de vectores"),
        (3, "Búsqueda en vectores"),
        (4, "Simulación de caída libre")
    ]
    
    total_start_time = time.time()
    
    for exercise_num, exercise_name in exercises:
        print(f"\nIniciando ejercicio {exercise_num}: {exercise_name}")
        print("-" * 50)
        
        exercise_start_time = time.time()
        run_exercise(exercise_num)
        exercise_time = time.time() - exercise_start_time
        
        print(f"Ejercicio {exercise_num} completado en {exercise_time:.4f} segundos")
        print("=" * 50)
    
    total_time = time.time() - total_start_time
    print(f"\nTODOS LOS EJERCICIOS COMPLETADOS")
    print(f"Tiempo total: {total_time:.4f} segundos")


def interactive_mode():
    """Modo interactivo para seleccionar ejercicios"""
    while True:
        print_exercise_menu()
        
        try:
            choice = input("Seleccione un ejercicio (0-5): ").strip()
            
            if choice == '0':
                print("¡Hasta luego!")
                break
            elif choice in ['1', '2', '3', '4', '5']:
                exercise_num = int(choice)
                print(f"\nEjecutando ejercicio {exercise_num}...")
                run_exercise(exercise_num)
                
                input("\nPresione Enter para continuar...")
                print("\n" + "=" * 80 + "\n")
            else:
                print("Opción inválida. Por favor, seleccione un número del 0 al 5.")
                
        except KeyboardInterrupt:
            print("\n\nPrograma interrumpido por el usuario.")
            break
        except Exception as e:
            print(f"Error: {e}")


def main():
    """Función principal del programa"""
    print_header()
    
    # Verificar argumentos de línea de comandos
    if len(sys.argv) > 1:
        try:
            exercise_num = int(sys.argv[1])
            if exercise_num in range(1, 6):
                print(f"Ejecutando ejercicio {exercise_num} desde línea de comandos...")
                run_exercise(exercise_num)
            else:
                print("Número de ejercicio inválido. Use números del 1 al 5.")
        except ValueError:
            print("Argumento inválido. Use un número del 1 al 5.")
    else:
        # Modo interactivo
        interactive_mode()


if __name__ == "__main__":
    main()
