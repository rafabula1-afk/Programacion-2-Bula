# 🏥 Sistema de Gestión Hospitalaria

## 📋 Descripción del Proyecto

Sistema completo de gestión hospitalaria desarrollado en C++ que permite administrar pacientes, doctores, citas médicas e historiales clínicos. El sistema demuestra dominio avanzado de **punteros, memoria dinámica y estructuras** en C++, implementando operaciones CRUD completas, manejo robusto de memoria y algoritmos de búsqueda eficientes.

## 🚀 Funcionalidades Principales

### 👥 Gestión de Pacientes
- ✅ Registrar nuevos pacientes
- ✅ Buscar pacientes por cédula, ID o nombre
- ✅ Actualizar datos de pacientes
- ✅ Eliminar pacientes (eliminación lógica)
- ✅ Listar todos los pacientes
- ✅ Gestión de historial médico completo

### 🩺 Gestión de Doctores
- ✅ Registrar nuevos doctores
- ✅ Buscar doctores por ID o especialidad
- ✅ Asignar pacientes a doctores
- ✅ Listar doctores y sus pacientes asignados
- ✅ Eliminar doctores

### 📅 Gestión de Citas Médicas
- ✅ Agendar nuevas citas con validaciones
- ✅ Cancelar citas
- ✅ Atender citas y generar historial automático
- ✅ Buscar citas por paciente, doctor o fecha
- ✅ Verificar disponibilidad de doctores
- ✅ Listar citas pendientes

## 🛠️ Tecnologías Utilizadas

- **Lenguaje:** C++ (estándar C++11)
- **Bibliotecas estándar:** `<iostream>`, `<cstring>`, `<ctime>`, `<iomanip>`, `<cctype>`
- **Compilador:** Compatible con GCC, Clang, MSVC y Dev-C++

## 📁 Estructuras de Datos

### Hospital
Estructura contenedora principal que gestiona todos los datos del sistema mediante arrays dinámicos. Contiene arrays de pacientes, doctores y citas, además de contadores automáticos de IDs.

### Paciente
Representa a un paciente con datos personales, médicos y arrays dinámicos para historial médico y citas agendadas. Cada paciente tiene cédula única en el sistema.

### Doctor
Representa a un doctor con información profesional y arrays dinámicos para pacientes asignados y citas agendadas. Incluye especialidad y costo de consulta.

### Cita
Conecta pacientes con doctores mediante IDs. Gestiona el estado de las citas ("Agendada", "Atendida", "Cancelada") y el proceso de atención.


### HistorialMedico
Registro individual de consulta médica dentro del historial de un paciente. Se almacena en arrays dinámicos dentro de cada paciente.

### Relacionea entre Estructuras

Hospital → Pacientes, Doctores, Citas (contención principal)

Paciente → HistorialMedico (cada paciente tiene su historial)

Paciente → Citas (cada paciente tiene citas agendadas)

Doctor → Citas (cada doctor tiene citas agendadas)

Doctor → Pacientes (asignación de pacientes a doctores)

Cita → Paciente + Doctor (conexión entre paciente y doctor)

## Funciones Principales

### Módulo de Gestión de Memoria
inicializarHospital() - Crea e inicializa la estructura principal con capacidades iniciales

destruirHospital() - Libera TODA la memoria recursivamente sin memory leaks

redimensionarArrayPacientes() - Duplica capacidad cuando el array se llena

redimensionarArrayDoctores() - Similar redimensionamiento para doctores

redimensionarArrayCitas() - Similar redimensionamiento para citas

copiarString() - Función auxiliar para deep copy de strings

copiarPaciente() - Realiza deep copy completa de un paciente

### Módulo de Pacientes (CRUD Completo)
crearPaciente() - Registra nuevo paciente con validación de cédula única

buscarPacientePorCedula() - Búsqueda case-insensitive por cédula

buscarPacientePorId() - Búsqueda rápida por ID único

buscarPacientesPorNombre() - Búsqueda parcial case-insensitive por nombre

actualizarPaciente() - Modificación de datos con validaciones integradas

eliminarPaciente() - Eliminación lógica liberando memoria de arrays internos

listarPacientes() - Visualización en formato tabular profesional

### Módulo de Historial Médico
agregarConsultaAlHistorial() - Añade consulta redimensionando array si es necesario

obtenerHistorialCompleto() - Retorna el array completo de historial

mostrarHistorialMedico() - Visualización tabular del historial

obtenerUltimaConsulta() - Acceso rápido a la última consulta

### Módulo de Doctores
crearDoctor() - Registro con validación de cédula profesional única

buscarDoctorPorId() - Búsqueda por ID único

buscarDoctoresPorEspecialidad() - Filtrado por especialidad médica

asignarPacienteADoctor() - Asigna paciente a doctor con verificación de duplicados

removerPacienteDeDoctor() - Elimina asignación paciente-doctor

listarPacientesDeDoctor() - Muestra pacientes asignados a un doctor

listarDoctores() - Visualización completa en formato tabular

eliminarDoctor() - Eliminación lógica con liberación de memoria

### Módulo de Citas
agendarCita() - Crea nueva cita con validaciones de fecha, hora y disponibilidad

cancelarCita() - Cambia estado a "Cancelada" y remueve de arrays asociados

atenderCita() - Cambia estado a "Atendida" y crea entrada en historial médico

obtenerCitasDePaciente() - Retorna todas las citas de un paciente

obtenerCitasDeDoctor() - Retorna todas las citas de un doctor

obtenerCitasPorFecha() - Filtra citas por fecha específica

listarCitasPendientes() - Muestra citas con estado "Agendada"

verificarDisponibilidad() - Valida si doctor está disponible en fecha/hora

Módulo de Validaciones
validarFecha() - Verifica formato YYYY-MM-DD y validez de fecha

validarHora() - Verifica formato HH:MM y validez de hora

validarCedula() - Valida que cédula no esté vacía y tenga longitud máxima

validarEmail() - Validación básica con presencia de '@' y '.'

### Módulo de Interfaz de Usuario
mostrarMenuPrincipal() - Menú principal del sistema

mostrarMenuPacientes() - Submenú de gestión de pacientes

mostrarMenuDoctores() - Submenú de gestión de doctores

mostrarMenuCitas() - Submenú de gestión de citas

ejecutarMenuPacientes() - Lógica del menú de pacientes

ejecutarMenuDoctores() - Lógica del menú de doctores

ejecutarMenuCitas() - Lógica del menú de citas

limpiarBuffer() - Función auxiliar para limpieza del buffer de entrada

## Decisiones de Diseño

Los arrays dinámicos se eligieron para demostrar dominio completo de:

Gestión manual de memoria con new y delete

Implementación de algoritmos de redimensionamiento

Control preciso sobre el uso de memoria

Mayor control sobre el rendimiento y comportamiento

### Estrategia de redimensionamiento
Algoritmo implementado:

Cuando cantidadActual >= capacidad

Calcular nuevaCapacidad = capacidadActual * 2

Crear nuevo array con new Tipo[nuevaCapacidad]

Copiar todos los elementos existentes al nuevo array

Liberar memoria del array antiguo con delete[]

Actualizar puntero y capacidad

### Manejo de memoria
Estrategias implementadas:

Liberación recursiva: destruirHospital() libera memoria de forma recursiva

Verificación de nullptr: Siempre se verifica antes de liberar memoria

Eliminación lógica: Uso de flags Activo/Disponible en lugar de eliminación física

Deep copy: Implementación completa en copiarPaciente()

Limpieza completa: Garantía de zero memory leaks al finalizar el programa



## 🔧 Instrucciones de Compilación y Ejecucion

```bash
g++ -std=c++11 sistema_gestion_hospitalaria.cpp -o hospital

./hospital
