#Compilador y flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I.
LDFLAGS = 

#Directorios
SRC_DIR = .
HOSPITAL_DIR = hospital
PACIENTES_DIR = pacientes
DOCTORES_DIR = doctores
CITAS_DIR = citas
HISTORIAL_DIR = historial
PERSISTENCIA_DIR = persistencia
UTILIDADES_DIR = utilidades

#Archivos fuente
SRCS = main.cpp \
       $(HOSPITAL_DIR)/Hospital.cpp \
       $(PACIENTES_DIR)/Paciente.cpp \
       $(PACIENTES_DIR)/operacionesPacientes.cpp \
       $(DOCTORES_DIR)/Doctor.cpp \
       $(DOCTORES_DIR)/operacionesDoctores.cpp \
       $(CITAS_DIR)/Cita.cpp \
       $(CITAS_DIR)/operacionesCitas.cpp \
       $(HISTORIAL_DIR)/HistorialMedico.cpp \
       $(HISTORIAL_DIR)/operacionesHistorial.cpp \
       $(UTILIDADES_DIR)/Validaciones.cpp \
       $(UTILIDADES_DIR)/Formatos.cpp

#Archivos objeto
OBJS = $(SRCS:.cpp=.o)

#Nombre del ejecutable
TARGET = hospital_v3

#Regla principal
all: $(TARGET)

#Enlazar objeto
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

#Compilar archivos .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Crear directorio de datos
datos:
	mkdir -p datos

#Limpiar archivos compilados
clean:
	rm -f $(OBJS) $(TARGET)

#Limpiar todo incluyendo archivos de datos
clean-all: clean
	rm -rf datos/*.bin datos/*.tmp

#Ejecutar programa
run: all datos
	./$(TARGET)

#Depurar
debug: CXXFLAGS += -g
debug: clean all

#Release
release: CXXFLAGS += -O3
release: clean all

#Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make all     - Compilar todo (por defecto)"
	@echo "  make clean   - Eliminar archivos objeto y ejecutable"
	@echo "  make clean-all - Eliminar todo incluyendo archivos de datos"
	@echo "  make run     - Compilar y ejecutar"
	@echo "  make debug   - Compilar con información de depuración"
	@echo "  make release - Compilar con optimizaciones"
	@echo "  make help    - Mostrar esta ayuda"

.PHONY: all clean clean-all run debug release help datos