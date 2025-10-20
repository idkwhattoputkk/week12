#!/bin/bash
# Script de demostración para algoritmos embarazosamente paralelos en C++ con OpenMP

echo "=========================================="
echo "DEMOSTRACIÓN C++ CON OPENMP"
echo "=========================================="
echo "Fecha: $(date)"
echo "Sistema: $(uname -s)"
echo "Arquitectura: $(uname -m)"
echo ""

# Verificar dependencias
echo "Verificando dependencias..."
echo "---------------------------"

# Verificar compilador
if command -v g++ &> /dev/null; then
    echo "✓ g++ encontrado: $(g++ --version | head -n1)"
else
    echo "✗ g++ no encontrado"
    exit 1
fi

# Verificar OpenMP
if echo '#include <omp.h>' | g++ -fopenmp -E -dM - 2>/dev/null | grep -q OPENMP; then
    echo "✓ OpenMP disponible"
else
    echo "✗ OpenMP no disponible"
    exit 1
fi

# Verificar OpenCV (opcional)
if pkg-config --exists opencv4 2>/dev/null; then
    echo "✓ OpenCV disponible"
    OPENCV_AVAILABLE=true
else
    echo "⚠ OpenCV no encontrado (ejercicio de imágenes no disponible)"
    OPENCV_AVAILABLE=false
fi

echo ""

# Compilar programas
echo "Compilando programas..."
echo "----------------------"

if [ "$OPENCV_AVAILABLE" = true ]; then
    make all
else
    echo "Compilando sin OpenCV..."
    make -j$(nproc) vector_summation vector_search fall_simulation main
fi

if [ $? -eq 0 ]; then
    echo "✓ Compilación exitosa"
else
    echo "✗ Error en compilación"
    exit 1
fi

echo ""

# Ejecutar demostración
echo "Ejecutando demostración..."
echo "========================="

# Mostrar información del sistema
echo "Información del sistema OpenMP:"
echo "Threads disponibles: $(OMP_NUM_THREADS=${OMP_NUM_THREADS:-$(nproc)} ./bin/main 2>&1 | grep "OpenMP disponible" | cut -d: -f2 | tr -d ' ')"
echo ""

# Ejecutar ejercicios
echo "Ejecutando todos los ejercicios..."
echo "---------------------------------"

if [ "$OPENCV_AVAILABLE" = true ]; then
    ./bin/main 5
else
    echo "Ejecutando ejercicios disponibles (sin filtro de imágenes)..."
    echo ""
    echo "=== EJERCICIO 3: SUMA DE VECTORES ==="
    ./bin/vector_summation
    echo ""
    echo "=== EJERCICIO 4: BÚSQUEDA EN VECTORES ==="
    ./bin/vector_search
    echo ""
    echo "=== EJERCICIO 5: SIMULACIÓN DE CAÍDA LIBRE ==="
    ./bin/fall_simulation
fi

echo ""
echo "=========================================="
echo "DEMOSTRACIÓN COMPLETADA"
echo "=========================================="

# Mostrar archivos generados
echo ""
echo "Archivos generados:"
echo "------------------"
if [ -d "sample_images" ]; then
    echo "Imágenes de muestra: $(ls sample_images/ | wc -l) archivos"
fi
if [ -d "output_images" ]; then
    echo "Imágenes procesadas: $(ls output_images/ | wc -l) archivos"
fi

echo ""
echo "Para ejecutar ejercicios individuales:"
echo "  ./bin/main 1    # Filtro de imágenes (requiere OpenCV)"
echo "  ./bin/main 2    # Suma de vectores"
echo "  ./bin/main 3    # Búsqueda en vectores"
echo "  ./bin/main 4    # Simulación de caída libre"
echo "  ./bin/main 5    # Todos los ejercicios"
echo ""
echo "Para limpiar archivos generados:"
echo "  make clean-data"
echo ""
echo "Para limpiar todo:"
echo "  make clean"
