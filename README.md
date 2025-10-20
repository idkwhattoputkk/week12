# Algoritmos Embarazosamente Paralelos

Este proyecto implementa cuatro ejercicios de algoritmos embarazosamente paralelos en **dos versiones**:

1. **Python** con `concurrent.futures` - Fácil de entender y ejecutar
2. **C++** con **OpenMP** - Máximo rendimiento y control

Cada ejercicio demuestra cómo aprovechar el paralelismo para mejorar el rendimiento en tareas que pueden ejecutarse independientemente.

## ¿Qué son los Algoritmos Embarazosamente Paralelos?

Los algoritmos embarazosamente paralelos (embarrassingly parallel) son aquellos donde las tareas pueden dividirse completamente en subtareas independientes que no necesitan comunicarse entre sí durante la ejecución. Estas tareas pueden ejecutarse en paralelo sin sincronización compleja.

### Características principales:
- **Independencia total**: Cada tarea puede ejecutarse sin depender de otras
- **Sin comunicación**: Las tareas no necesitan intercambiar datos durante la ejecución
- **Escalabilidad lineal**: El speedup teórico es proporcional al número de procesadores
- **Fácil paralelización**: No requieren técnicas complejas de sincronización

## Ejercicios Implementados

### 1. Filtro de Imágenes (`image_filter.py`)

**Objetivo**: Aplicar un filtro de escala de grises a múltiples imágenes de forma paralela.

**Descomposición funcional**:
```
Leer imagen 1 → Aplicar filtro → Guardar
Leer imagen 2 → Aplicar filtro → Guardar
...
Leer imagen n → Aplicar filtro → Guardar
```

**Paralelización**: Cada imagen se procesa independientemente usando `ProcessPoolExecutor`.

**Tecnologías**: PIL (Pillow) para procesamiento de imágenes, `concurrent.futures` para paralelismo.

### 2. Suma de Elementos en Múltiples Vectores (`vector_summation.py`)

**Objetivo**: Calcular la suma de elementos en múltiples vectores de forma paralela.

**Descomposición funcional**:
```
Vector 1 → Sumar elementos → Resultado 1
Vector 2 → Sumar elementos → Resultado 2
...
Vector n → Sumar elementos → Resultado n
```

**Paralelización**: Cada vector se suma independientemente usando `ProcessPoolExecutor`.

**Optimización**: Se incluye trabajo computacional adicional para hacer más evidente la diferencia entre secuencial y paralelo.

### 3. Búsqueda en Múltiples Vectores (`vector_search.py`)

**Objetivo**: Buscar un valor específico en múltiples vectores de forma paralela.

**Descomposición funcional**:
```
Vector 1 → Buscar valor → Resultado 1
Vector 2 → Buscar valor → Resultado 2
...
Vector n → Buscar valor → Resultado n
```

**Paralelización**: Cada búsqueda se ejecuta independientemente usando `ProcessPoolExecutor`.

**Algoritmo**: Búsqueda lineal con verificación de resultados.

### 4. Simulación de Caída Libre (`fall_simulation.py`)

**Objetivo**: Calcular tiempos de caída libre para múltiples objetos con diferentes parámetros físicos.

**Descomposición funcional**:
```
Objeto 1: Altura → Calcular tiempo → Resultado 1
Objeto 2: Altura → Calcular tiempo → Resultado 2
...
Objeto n: Altura → Calcular tiempo → Resultado n
```

**Paralelización**: Cada simulación física se ejecuta independientemente usando `ProcessPoolExecutor`.

**Física**: Incluye resistencia del aire para mayor realismo en los cálculos.

## Estructura del Proyecto

```
week12/
├── # Versión Python
├── main.py                 # Programa principal con menú interactivo
├── image_filter.py         # Ejercicio 1: Filtro de imágenes
├── vector_summation.py    # Ejercicio 3: Suma de vectores
├── vector_search.py       # Ejercicio 4: Búsqueda en vectores
├── fall_simulation.py     # Ejercicio 5: Simulación de caída libre
├── demo.py               # Script de demostración automatizada
├── requirements.txt       # Dependencias de Python
│
├── # Versión C++ con OpenMP
├── main.cpp               # Programa principal en C++
├── image_filter.cpp       # Ejercicio 1: Filtro de imágenes
├── vector_summation.cpp  # Ejercicio 3: Suma de vectores
├── vector_search.cpp     # Ejercicio 4: Búsqueda en vectores
├── fall_simulation.cpp   # Ejercicio 5: Simulación de caída libre
├── Makefile              # Sistema de compilación
├── CPP_DEPENDENCIES.md   # Guía de dependencias C++
│
└── README.md             # Esta documentación
```

## Instalación y Uso

### Requisitos del Sistema
- **Múltiples núcleos de CPU** (recomendado para ver speedup)
- **Python 3.7+** (para versión Python)
- **C++17 compatible compiler** (para versión C++)
- **OpenMP** (para paralelismo en C++)

### Versión Python

#### Instalación de Dependencias
```bash
pip install -r requirements.txt
```

#### Ejecución
```bash
# Modo interactivo
python main.py

# Ejercicio específico
python main.py 1  # Filtro de imágenes
python main.py 2  # Suma de vectores
python main.py 3  # Búsqueda en vectores
python main.py 4  # Simulación de caída libre
python main.py 5  # Todos los ejercicios

# Demostración automatizada
python demo.py
```

### Versión C++ con OpenMP

#### Instalación de Dependencias
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libomp-dev libopencv-dev pkg-config

# macOS (con Homebrew)
brew install opencv pkg-config

# Ver guía completa en CPP_DEPENDENCIES.md
```

#### Compilación
```bash
# Compilar todos los programas
make all

# O compilar individualmente
make run-image      # Ejercicio 1
make run-vector-sum  # Ejercicio 3
make run-vector-search # Ejercicio 4
make run-fall        # Ejercicio 5
```

#### Ejecución
```bash
# Programa principal
./bin/main

# Ejercicios individuales
./bin/image_filter
./bin/vector_summation
./bin/vector_search
./bin/fall_simulation

# Ejecutar todos los ejercicios
./bin/main 5
```

## Resultados y Benchmarks

Cada ejercicio incluye benchmarks automáticos que comparan:
- **Tiempo secuencial**: Ejecución tradicional sin paralelismo
- **Tiempo paralelo**: Ejecución usando `ProcessPoolExecutor`
- **Speedup**: Factor de mejora de rendimiento
- **Eficiencia**: Porcentaje de utilización de los núcleos disponibles

### Ejemplo de Salida Típica

```
=== EJERCICIO 1: FILTRO DE IMÁGENES ===

Creando imágenes de muestra...
Se crearon 8 imágenes de muestra

Ejecutando versión secuencial...
Tiempo secuencial: 2.3456 segundos
Tiempo promedio por imagen: 0.2932 segundos

Ejecutando versión paralela...
Tiempo paralelo: 0.6789 segundos
Tiempo promedio por imagen: 0.0849 segundos

Speedup: 3.45x
Eficiencia: 86.3%
```

## Tecnologías y Librerías Utilizadas

### Versión Python
#### Paralelismo
- **`concurrent.futures.ProcessPoolExecutor`**: Para paralelismo basado en procesos
- **`concurrent.futures.ThreadPoolExecutor`**: Alternativa para paralelismo basado en hilos
- **`multiprocessing`**: Base del ProcessPoolExecutor

#### Procesamiento de Datos
- **`PIL (Pillow)`**: Procesamiento de imágenes
- **`numpy`**: Operaciones numéricas eficientes
- **`random`**: Generación de datos de prueba

#### Utilidades
- **`time`**: Medición de rendimiento
- **`os`**: Operaciones del sistema de archivos
- **`math`**: Cálculos matemáticos para física

### Versión C++ con OpenMP
#### Paralelismo
- **OpenMP**: Paralelismo compartido de memoria
- **`#pragma omp parallel for`**: Directiva principal para paralelización
- **`omp_get_max_threads()`**: Control de threads disponibles

#### Procesamiento de Datos
- **OpenCV**: Procesamiento eficiente de imágenes
- **STL containers**: `std::vector` para manejo de datos
- **`<random>`**: Generación de datos de prueba

#### Utilidades
- **`<chrono>`**: Medición precisa de rendimiento
- **`<cmath>`**: Cálculos matemáticos optimizados
- **`<iomanip>`**: Formateo de salida

## Comparación Python vs C++

| Aspecto | Python + concurrent.futures | C++ + OpenMP |
|---------|----------------------------|--------------|
| **Facilidad de uso** | ⭐⭐⭐⭐⭐ Muy fácil | ⭐⭐⭐ Moderado |
| **Rendimiento** | ⭐⭐⭐ Bueno | ⭐⭐⭐⭐⭐ Excelente |
| **Control de memoria** | ⭐⭐ Limitado | ⭐⭐⭐⭐⭐ Completo |
| **Portabilidad** | ⭐⭐⭐⭐⭐ Excelente | ⭐⭐⭐⭐ Buena |
| **Debugging** | ⭐⭐⭐⭐ Fácil | ⭐⭐⭐ Moderado |
| **Tiempo de desarrollo** | ⭐⭐⭐⭐⭐ Rápido | ⭐⭐⭐ Moderado |

### Cuándo usar cada versión:

**Usar Python cuando:**
- Prototipado rápido
- Desarrollo educativo
- Sistemas con pocos núcleos
- Facilidad de mantenimiento es prioritaria

**Usar C++ cuando:**
- Rendimiento máximo requerido
- Sistemas con muchos núcleos
- Control preciso de memoria
- Aplicaciones de producción

## Ventajas del Paralelismo Implementado

### 1. Escalabilidad
- El rendimiento mejora proporcionalmente con el número de núcleos
- Fácil adaptación a sistemas con más procesadores

### 2. Eficiencia de Recursos
- Aprovecha completamente los núcleos disponibles
- Reduce el tiempo de inactividad de la CPU

### 3. Simplicidad de Implementación
- No requiere sincronización compleja
- Código fácil de entender y mantener

### 4. Robustez
- Fallos en una tarea no afectan a las demás
- Recuperación automática de errores

## Limitaciones y Consideraciones

### 1. Overhead de Comunicación
- Crear procesos tiene un costo inicial
- Para tareas muy pequeñas, el overhead puede superar los beneficios

### 2. Memoria
- Cada proceso tiene su propio espacio de memoria
- Puede aumentar significativamente el uso de RAM

### 3. Dependencias
- Requiere que las tareas sean verdaderamente independientes
- No aplicable a algoritmos con dependencias complejas

## Casos de Uso Reales

Los algoritmos embarazosamente paralelos son comunes en:

- **Procesamiento de imágenes**: Filtros, redimensionamiento, conversión de formatos
- **Análisis de datos**: Cálculos estadísticos independientes
- **Simulaciones**: Modelos físicos independientes
- **Búsquedas**: Consultas en bases de datos separadas
- **Renderizado**: Generación de frames independientes

## Conclusiones

Este proyecto demuestra que los algoritmos embarazosamente paralelos ofrecen:

1. **Mejoras significativas de rendimiento** con implementación relativamente simple
2. **Escalabilidad lineal** en sistemas multi-núcleo
3. **Aplicabilidad amplia** en problemas del mundo real
4. **Facilidad de mantenimiento** y debugging

La clave está en identificar correctamente las tareas independientes y aplicar las herramientas de paralelismo apropiadas para cada caso específico.
