# Dependencias para algoritmos embarazosamente paralelos en C++ con OpenMP

## Requisitos del Sistema

### Compilador
- **g++** 7.0 o superior (con soporte para C++17)
- **Clang++** 6.0 o superior (alternativa)
- **OpenMP** habilitado en el compilador

### Librerías Requeridas

#### OpenMP
- **Ubuntu/Debian**: `sudo apt-get install libomp-dev`
- **CentOS/RHEL**: `sudo yum install libomp-devel`
- **macOS**: `brew install libomp` (o viene incluido con Xcode)
- **Windows**: Incluido en Visual Studio 2019+

#### OpenCV (solo para ejercicio de filtro de imágenes)
- **Ubuntu/Debian**: `sudo apt-get install libopencv-dev`
- **CentOS/RHEL**: `sudo yum install opencv-devel`
- **macOS**: `brew install opencv`
- **Windows**: Descargar desde opencv.org

#### Herramientas de Desarrollo
- **pkg-config**: Para configuración automática de OpenCV
- **make**: Para usar el Makefile incluido

## Instalación por Sistema Operativo

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential libomp-dev libopencv-dev pkg-config make
```

### CentOS/RHEL/Fedora
```bash
sudo yum groupinstall 'Development Tools'
sudo yum install libomp-devel opencv-devel pkgconfig make
```

### macOS (con Homebrew)
```bash
brew install opencv pkg-config make
# OpenMP viene incluido con Xcode Command Line Tools
```

### Windows (con vcpkg)
```bash
vcpkg install opencv openmp
```

## Verificación de la Instalación

### Verificar OpenMP
```bash
echo '#include <omp.h>' | g++ -fopenmp -E -dM - | grep -i openmp
```

### Verificar OpenCV
```bash
pkg-config --exists opencv4 && echo "OpenCV disponible" || echo "OpenCV no encontrado"
```

### Compilar un programa de prueba
```bash
echo '#include <omp.h>
#include <iostream>
int main() {
    std::cout << "OpenMP threads: " << omp_get_max_threads() << std::endl;
    return 0;
}' > test_openmp.cpp

g++ -fopenmp test_openmp.cpp -o test_openmp
./test_openmp
rm test_openmp.cpp test_openmp
```

## Configuración del Compilador

### Flags Recomendados
- **C++17**: `-std=c++17`
- **Optimización**: `-O3`
- **OpenMP**: `-fopenmp`
- **Warnings**: `-Wall -Wextra`

### Variables de Entorno OpenMP
```bash
export OMP_NUM_THREADS=4  # Usar 4 threads
export OMP_SCHEDULE=dynamic  # Scheduling dinámico
export OMP_PROC_BIND=true  # Bind threads a cores
```

## Problemas Comunes y Soluciones

### Error: "omp.h: No such file or directory"
- **Solución**: Instalar libomp-dev (Ubuntu) o libomp-devel (CentOS)

### Error: "undefined reference to omp_get_thread_num"
- **Solución**: Agregar `-fopenmp` a los flags de linking

### Error: "opencv2/opencv.hpp: No such file or directory"
- **Solución**: Instalar libopencv-dev y verificar PKG_CONFIG_PATH

### Performance pobre
- **Solución**: Verificar que OMP_NUM_THREADS esté configurado correctamente
- **Solución**: Usar `-O3` para optimización máxima

## Compilación Manual (sin Makefile)

### Programa principal
```bash
g++ -std=c++17 -O3 -fopenmp -Wall -Wextra main.cpp vector_summation.cpp vector_search.cpp fall_simulation.cpp -o main
```

### Ejercicio de imágenes (requiere OpenCV)
```bash
g++ -std=c++17 -O3 -fopenmp -Wall -Wextra image_filter.cpp `pkg-config --cflags --libs opencv4` -o image_filter
```

### Otros ejercicios
```bash
g++ -std=c++17 -O3 -fopenmp -Wall -Wextra vector_summation.cpp -o vector_summation
g++ -std=c++17 -O3 -fopenmp -Wall -Wextra vector_search.cpp -o vector_search
g++ -std=c++17 -O3 -fopenmp -Wall -Wextra fall_simulation.cpp -o fall_simulation
```
