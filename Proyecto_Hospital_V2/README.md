PROYECTO 2: SISTEMA DE GESTIÓN HOSPITALARIA CON PERSISTENCIA EN ARCHIVOS BINARIOS
📋 DESCRIPCIÓN DEL PROYECTO
Sistema completo de gestión hospitalaria que implementa persistencia de datos mediante archivos binarios y acceso aleatorio, demostrando dominio avanzado de operaciones de archivos binarios, gestión de memoria y algoritmos de búsqueda eficientes en C++.

🎯 OBJETIVOS DE APRENDIZAJE
Implementar persistencia de datos mediante archivos binarios

Dominar operaciones de lectura/escritura binaria con fstream

Aplicar acceso aleatorio usando seekg() y seekp()

Comprender el mapeo entre estructuras en memoria y archivos

Gestionar índices y posiciones en archivos binarios

🏗️ ARQUITECTURA DE ARCHIVOS
Diagrama de Archivos y Relaciones
text
┌─────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│  hospital.bin   │    │ pacientes.bin    │    │  doctores.bin    │
│                 │    │                  │    │                  │
│ - Datos general │◄───┤ - Lista pacientes│    │ - Lista doctores │
│ - Contadores IDs│    │ - IDs únicos     │    │ - IDs únicos     │
└─────────────────┘    └────────┬─────────┘    └────────┬─────────┘
                                │                      │
                                │                      │
                                ▼                      ▼
                        ┌─────────────────┐    ┌─────────────────┐
                        │   citas.bin     │    │ historiales.bin │
                        │                 │    │                 │
                        │ - Citas médicas │◄───┤ - Consultas     │
                        │ - Relaciones    │    │ - Diagnósticos  │
                        └─────────────────┘    └─────────────────┘
Descripción de Archivos
hospital.bin: Contiene información general del hospital y contadores de IDs

pacientes.bin: Almacena todos los registros de pacientes con sus datos médicos

doctores.bin: Contiene la información profesional de todos los doctores

citas.bin: Registra todas las citas médicas agendadas

historiales.bin: Almacena el historial médico completo de las consultas

📊 FORMATO DE ARCHIVOS
Estructura de Headers
Cada archivo binario comienza con un header que contiene metadata esencial:

cpp
struct ArchivoHeader {
    int cantidadRegistros;      // Número total de registros en el archivo
    int proximoID;              // Siguiente ID auto-incremental disponible
    int registrosActivos;       // Número de registros no eliminados
    int version;                // Versión del formato (1 para este proyecto)
};
Layout de Registros
text
Byte 0-15:     Header (ArchivoHeader)
Byte 16+:      [Registro 1]
Byte 16+size:  [Registro 2]
Byte 16+2*size:[Registro 3]
...
Byte 16+n*size:[Registro N]
Fórmulas de Cálculo de Posiciones
Para calcular la posición de cualquier registro:

cpp
long calcularPosicion(int indice, size_t tamañoRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamañoRegistro);
}
Ejemplos prácticos:

Paciente en índice 5: sizeof(ArchivoHeader) + (5 * sizeof(Paciente))

Doctor en índice 2: sizeof(ArchivoHeader) + (2 * sizeof(Doctor))

Cita en índice 10: sizeof(ArchivoHeader) + (10 * sizeof(Cita))

📝 MANUAL DE FUNCIONES PRINCIPALES
Gestión de Archivos
bool inicializarArchivo(const char* nombreArchivo)
Propósito: Crear un nuevo archivo binario con header inicial
Ejemplo:

cpp
if (inicializarArchivo("pacientes.bin")) {
    cout << "Archivo creado exitosamente" << endl;
}
ArchivoHeader leerHeader(const char* nombreArchivo)
Propósito: Leer la metadata de cualquier archivo binario
Ejemplo:

cpp
ArchivoHeader header = leerHeader("pacientes.bin");
cout << "Registros activos: " << header.registrosActivos << endl;
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header)
Propósito: Actualizar la metadata del archivo después de operaciones
Ejemplo:

cpp
ArchivoHeader header = leerHeader("pacientes.bin");
header.registrosActivos++;
actualizarHeader("pacientes.bin", header);
Operaciones de Acceso Aleatorio
Paciente leerPacientePorIndice(int indice)
Propósito: Leer un paciente específico usando acceso aleatorio
Ejemplo:

cpp
Paciente p = leerPacientePorIndice(5);  // Lee directamente el 6to paciente
if (p.id != 0) {
    cout << "Paciente: " << p.nombre << endl;
}
Patrón General de Operaciones con Archivos
cpp
bool operacionArchivo(...) {
    // 1. ABRIR archivo específico
    fstream archivo("nombre.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;
    
    // 2. POSICIONARSE en el registro exacto
    long posicion = calcularPosicion(indice, sizeof(Registro));
    archivo.seekg(posicion);
    
    // 3. LEER solo lo necesario
    Registro reg;
    archivo.read((char*)&reg, sizeof(Registro));
    
    // 4. PROCESAR en memoria
    reg.campo = nuevoValor;
    
    // 5. VOLVER a posicionarse para escribir
    archivo.seekp(posicion);
    
    // 6. ESCRIBIR de vuelta
    archivo.write((char*)&reg, sizeof(Registro));
    
    // 7. CERRAR archivo
    archivo.close();
    
    return true;
}
Gestión de Pacientes
bool agregarPaciente(Hospital* hospital, Paciente* paciente)
Propósito: Agregar nuevo paciente al archivo con validaciones
Validaciones incluidas:

Cédula única en el sistema

Edad entre 0-120 años

Email con formato válido

Tipo de sangre válido (O+, O-, A+, A-, B+, B-, AB+, AB-)

Teléfono con formato válido

Ejemplo:

cpp
Paciente nuevo;
strcpy(nuevo.nombre, "Juan");
strcpy(nuevo.apellido, "Pérez");
strcpy(nuevo.cedula, "V-12345678");
nuevo.edad = 30;
// ... otros campos
if (agregarPaciente(hospital, &nuevo)) {
    cout << "Paciente agregado con ID: " << nuevo.id << endl;
}
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula)
Propósito: Buscar paciente por cédula (case-insensitive)
Ejemplo:

cpp
Paciente* p = buscarPacientePorCedula(hospital, "V-12345678");
if (p != nullptr) {
    cout << "Encontrado: " << p->nombre << endl;
    delete p;  // Importante: liberar memoria
}
bool eliminarPaciente(Hospital* hospital, int id)
Propósito: Eliminar paciente (borrado lógico) y compactar archivo
Características:

Marca como eliminado en el archivo

Reordena IDs automáticamente

Actualiza contadores en header

Ejemplo:

cpp
if (eliminarPaciente(hospital, 5)) {
    cout << "Paciente eliminado y archivo compactado" << endl;
}
Gestión de Doctores
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad)
Propósito: Buscar todos los doctores de una especialidad específica
Ejemplo:

cpp
int cantidad;
Doctor** doctores = buscarDoctoresPorEspecialidad(hospital, "Cardiología", &cantidad);
if (doctores != nullptr) {
    for (int i = 0; i < cantidad; i++) {
        cout << doctores[i]->nombre << endl;
        delete doctores[i];
    }
    delete[] doctores;
}
bool asignarPacienteADoctor(Hospital* hospital, int idDoctor, int idPaciente)
Propósito: Establecer relación entre doctor y paciente
Validaciones:

Verifica existencia de doctor y paciente

Evita asignaciones duplicadas

Controla límite máximo de pacientes por doctor (50)

Gestión de Citas
bool agendarCita(Hospital* hospital, Cita* cita)
Propósito: Agendar nueva cita médica con validaciones completas
Validaciones incluidas:

Existencia de paciente y doctor

Formato de fecha (YYYY-MM-DD) válido

Formato de hora (HH:MM) válido

Disponibilidad del doctor en fecha/hora específica

bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora)
Propósito: Verificar si un doctor está disponible en fecha y hora específicas
Ejemplo:

cpp
if (verificarDisponibilidad(hospital, 3, "2024-01-15", "14:30")) {
    cout << "Doctor disponible" << endl;
} else {
    cout << "Doctor no disponible" << endl;
}
🔄 FILOSOFÍA DE PERSISTENCIA INMEDIATA
Ventajas del Enfoque
1. Escalabilidad:

cpp
// ✅ Puedes tener 10,000 pacientes en el archivo
// Solo usas memoria para 1 paciente a la vez cuando lo necesitas
2. Persistencia inmediata:

cpp
// ✅ Cada operación guarda inmediatamente:
modificarPaciente(15, nuevosDatos); // Ya guardado en disco
// Si el programa crashea aquí, el cambio persiste
3. Eficiencia en memoria:

Uso de memoria total: Aproximadamente 1-2 KB

Sin importar si tienes 10 o 10,000 registros

No se mantienen arrays dinámicos en memoria

4. Integridad de datos:

Los archivos siempre están actualizados

No hay riesgo de olvidar guardar

Cada operación es independiente y completa

🛠️ FUNCIONES DE VALIDACIÓN IMPLEMENTADAS
bool validarFecha(const char* fecha)
Valida formato YYYY-MM-DD y fechas válidas (considera años bisiestos)

bool validarHora(const char* hora)
Valida formato HH:MM y horarios válidos

bool validarEmail(const char* email)
Valida formato básico de email (@ y .)

bool validarEdad(int edad)
Valida rango de edad (0-120 años)

bool validarTipoSangre(const char* tipoSangre)
Valida tipos de sangre: O+, O-, A+, A-, B+, B-, AB+, AB-

bool validarTelefono(const char* telefono)
Valida formato de teléfono (dígitos, espacios, guiones, paréntesis)

📈 COMPACTACIÓN DE ARCHIVOS
bool compactarArchivoPacientes(Hospital* hospital)
Propósito: Reorganizar archivo después de eliminaciones
Proceso:

Crea archivo temporal

Copia solo registros no eliminados

Reasigna IDs secuencialmente

Reemplaza archivo original

Actualiza contadores del hospital

Beneficios:

Elimina espacio desperdiciado por registros borrados

Mantiene IDs secuenciales

Optimiza el acceso aleatorio

🎮 ESTRUCTURA DE MENÚS
Menú Principal
text
SISTEMA DE GESTIÓN HOSPITALARIA v2
(Persistencia con Archivos)
1. Gestión de Pacientes
2. Gestión de Doctores  
3. Gestión de Citas
4. Mantenimiento de Archivos
5. Guardar y Salir
Submenús Especializados
Pacientes: CRUD completo con validaciones

Doctores: Gestión profesional con especialidades

Citas: Agendamiento con verificación de disponibilidad

Mantenimiento: Verificación de integridad y estadísticas

💾 CONSIDERACIONES TÉCNICAS
Manejo de Memoria
Todas las búsquedas que retornan arrays dinámicos deben liberarse

Uso de delete y delete[] apropiado

Verificación de nullptr antes de operaciones

Manejo de Errores
Validación de apertura de archivos

Verificación de lecturas/escrituras exitosas

Mensajes de error descriptivos

Recuperación graceful de fallos

Persistencia
Datos sobreviven a cierres abruptos

Cada operación es atómica

Headers siempre consistentes

Backup automático durante compactación