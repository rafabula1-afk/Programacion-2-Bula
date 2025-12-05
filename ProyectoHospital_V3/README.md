# 🏥 Sistema de Gestión Hospitalaria v3.0

## 📋 Descripción del Proyecto

Sistema modular de gestión hospitalaria desarrollado en **C++ con Programación Orientada a Objetos**, que implementa un sistema completo para administrar pacientes, doctores, citas médicas e historiales clínicos. El proyecto demuestra dominio avanzado de **clases, templates, persistencia de archivos y manejo robusto de memoria**.

## ✨ Características Principales

### 🏗️ Arquitectura Modular
- ✅ **Sistema completamente modular** con separación clara de responsabilidades
- ✅ **11 módulos independientes** organizados por funcionalidad
- ✅ **Separación .hpp/.cpp** en todos los componentes
- ✅ **Makefile profesional** para compilación automatizada

### 🛡️ Robustez y Seguridad
- ✅ **Persistencia completa** en archivos binarios con sistema de headers
- ✅ **Validaciones exhaustivas** en todos los datos de entrada
- ✅ **Manejo de errores** completo con mensajes descriptivos
- ✅ **Eliminación lógica** para preservar integridad de datos históricos
- ✅ **Compactación automática** de archivos para optimizar espacio

### 📊 Funcionalidades Completas

#### 👥 **Gestión de Pacientes** (`pacientes/`)
- Registro con validación de cédula única
- Búsqueda por ID, cédula o nombre (parcial)
- Modificación de datos con preservación de historial
- Eliminación lógica con confirmación
- Visualización en formatos básico y completo

#### 🩺 **Gestión de Doctores** (`doctores/`)
- Registro con cédula profesional única
- Especialidades médicas predefinidas
- Asignación de pacientes a doctores
- Control de disponibilidad y horarios
- Gestión de costos de consulta

#### 📅 **Gestión de Citas** (`citas/`)
- Agendamiento con validación de disponibilidad
- Estados: Agendada, Atendida, Cancelada, Reprogramada
- Búsqueda por paciente, doctor o fecha
- Proceso completo de atención con generación automática de historial
- Verificación de conflictos de horario

#### 📋 **Historial Médico** (`historial/`)
- Registro completo de consultas
- Enlace automático con citas atendidas
- Navegación entre consultas relacionadas
- Visualización tabular y detallada

## 🏗️ Estructura del Proyecto
ProyectoHospital_v3/
├── datos/ # Archivos binarios de persistencia
│ ├── hospital.bin
│ ├── pacientes.bin
│ ├── doctores.bin
│ ├── citas.bin
│ └── historiales.bin
├── hospital/ # Entidad principal del sistema
│ ├── Hospital.hpp
│ └── Hospital.cpp
├── pacientes/ # Módulo de gestión de pacientes
│ ├── Paciente.hpp
│ ├── Paciente.cpp
│ ├── operacionesPacientes.hpp
│ └── operacionesPacientes.cpp
├── doctores/ # Módulo de gestión de doctores
│ ├── Doctor.hpp
│ ├── Doctor.cpp
│ ├── operacionesDoctores.hpp
│ └── operacionesDoctores.cpp
├── citas/ # Módulo de gestión de citas
│ ├── Cita.hpp
│ ├── Cita.cpp
│ ├── operacionesCitas.hpp
│ └── operacionesCitas.cpp
├── historial/ # Módulo de historial médico
│ ├── HistorialMedico.hpp
│ ├── HistorialMedico.cpp
│ ├── operacionesHistorial.hpp
│ └── operacionesHistorial.cpp
├── persistencia/ # Sistema de archivos y persistencia
│ ├── GestorArchivos.hpp
│ ├── GestorArchivos.cpp
│ └── Constantes.hpp
├── utilidades/ # Funciones auxiliares
│ ├── Validaciones.hpp
│ ├── Validaciones.cpp
│ ├── Formatos.hpp
│ └── Formatos.cpp
├── main.cpp # Punto de entrada principal
└── Makefile # Sistema de compilación

text

## 🎯 Tecnologías y Conceptos Implementados

### 🔧 **Tecnologías Utilizadas**
- **Lenguaje:** C++11 (estándar ISO/IEC 14882:2011)
- **Paradigma:** Programación Orientada a Objetos (POO)
- **Persistencia:** Archivos binarios con headers personalizados
- **Compilación:** Makefile con soporte para múltiples configuraciones

### 🧠 **Conceptos Avanzados de C++**
- **Clases y Encapsulamiento** completo con getters/setters
- **Templates** en GestorArchivos para operaciones genéricas
- **reinterpret_cast** para conversiones seguras de tipos
- **Rule of Three** (destructor, constructor de copia, operador de asignación)
- **Sobrecarga de operadores** para operaciones de asignación
- **Métodos estáticos** y **namespaces** para organización

### 💾 **Sistema de Persistencia**
- **Archivos binarios** con estructura de headers
- **Sistema de versionado** para compatibilidad futura
- **Compactación automática** para eliminar registros eliminados
- **Verificación de integridad** al inicializar el sistema
- **Manejo de directorios** automático

## 🚀 Instalación y Compilación

### Prerrequisitos
- Compilador C++11 (g++, clang++, o MSVC)
- Sistema make (GNU Make)
- 50MB de espacio en disco

### Compilación
```bash
# Clonar o descargar el proyecto
git clone <repositorio>
cd ProyectoHospital_v3

# Compilar el proyecto
make all

# O simplemente
make