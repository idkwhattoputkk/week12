# Makefile para algoritmos embarazosamente paralelos en C++ con OpenMP

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -O3 -fopenmp -Wall -Wextra
LDFLAGS = -fopenmp

# Flags específicos para OpenCV (solo para image_filter.cpp)
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

# Directorio de objetos
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SOURCES = main.cpp image_filter.cpp vector_summation.cpp vector_search.cpp fall_simulation.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
TARGETS = $(BIN_DIR)/main $(BIN_DIR)/image_filter $(BIN_DIR)/vector_summation $(BIN_DIR)/vector_search $(BIN_DIR)/fall_simulation

# Regla por defecto
all: $(TARGETS)

# Crear directorios necesarios
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilar programa principal
$(BIN_DIR)/main: $(OBJ_DIR)/main.o $(OBJ_DIR)/vector_summation.o $(OBJ_DIR)/vector_search.o $(OBJ_DIR)/fall_simulation.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilar ejercicios individuales
$(BIN_DIR)/image_filter: $(OBJ_DIR)/image_filter.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(OPENCV_FLAGS)

$(BIN_DIR)/vector_summation: $(OBJ_DIR)/vector_summation.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(BIN_DIR)/vector_search: $(OBJ_DIR)/vector_search.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(BIN_DIR)/fall_simulation: $(OBJ_DIR)/fall_simulation.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Compilar objetos
$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/image_filter.o: image_filter.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OPENCV_FLAGS) -c -o $@ $<

$(OBJ_DIR)/vector_summation.o: vector_summation.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/vector_search.o: vector_search.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/fall_simulation.o: fall_simulation.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Reglas de limpieza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -rf sample_images output_images

clean-data:
	rm -rf sample_images output_images

# Reglas de instalación de dependencias
install-deps:
	@echo "Instalando dependencias..."
	@echo "Para Ubuntu/Debian:"
	@echo "sudo apt-get update"
	@echo "sudo apt-get install build-essential libomp-dev libopencv-dev pkg-config"
	@echo ""
	@echo "Para macOS con Homebrew:"
	@echo "brew install opencv pkg-config"
	@echo ""
	@echo "Para CentOS/RHEL:"
	@echo "sudo yum groupinstall 'Development Tools'"
	@echo "sudo yum install libomp-devel opencv-devel pkgconfig"

# Reglas de ejecución
run-main: $(BIN_DIR)/main
	$(BIN_DIR)/main

run-image: $(BIN_DIR)/image_filter
	$(BIN_DIR)/image_filter

run-vector-sum: $(BIN_DIR)/vector_summation
	$(BIN_DIR)/vector_summation

run-vector-search: $(BIN_DIR)/vector_search
	$(BIN_DIR)/vector_search

run-fall: $(BIN_DIR)/fall_simulation
	$(BIN_DIR)/fall_simulation

# Regla para ejecutar todos los ejercicios
run-all: $(BIN_DIR)/main
	$(BIN_DIR)/main 5

# Regla para mostrar información del sistema
info:
	@echo "Información del sistema:"
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "OpenMP disponible:"
	@echo '#include <omp.h>' | $(CXX) -fopenmp -E -dM - | grep -i openmp || echo "OpenMP no disponible"
	@echo "OpenCV disponible:"
	@pkg-config --exists opencv4 && echo "Sí" || echo "No"

# Regla de ayuda
help:
	@echo "Makefile para algoritmos embarazosamente paralelos en C++ con OpenMP"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make all          - Compilar todos los programas"
	@echo "  make clean        - Limpiar archivos compilados y datos"
	@echo "  make clean-data   - Limpiar solo los datos generados"
	@echo "  make install-deps - Mostrar instrucciones para instalar dependencias"
	@echo "  make info         - Mostrar información del sistema"
	@echo "  make help         - Mostrar esta ayuda"
	@echo ""
	@echo "Ejecución:"
	@echo "  make run-main     - Ejecutar programa principal"
	@echo "  make run-image    - Ejecutar ejercicio 1 (filtro de imágenes)"
	@echo "  make run-vector-sum - Ejecutar ejercicio 3 (suma de vectores)"
	@echo "  make run-vector-search - Ejecutar ejercicio 4 (búsqueda en vectores)"
	@echo "  make run-fall     - Ejecutar ejercicio 5 (simulación de caída libre)"
	@echo "  make run-all      - Ejecutar todos los ejercicios"
	@echo ""
	@echo "Ejemplos:"
	@echo "  make run-main 1   - Ejecutar ejercicio 1 desde línea de comandos"
	@echo "  make run-main 5   - Ejecutar todos los ejercicios"

# Marcar reglas que no crean archivos
.PHONY: all clean clean-data install-deps info help run-main run-image run-vector-sum run-vector-search run-fall run-all
