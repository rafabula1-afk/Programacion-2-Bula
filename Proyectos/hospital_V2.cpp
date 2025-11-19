#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <fstream>
using namespace std;

// =============================================================================
// DECLARACIONES ANTICIPADAS DE ESTRUCTURAS
// =============================================================================

struct Paciente;
struct Doctor;
struct Cita;
struct HistorialMedico;

// =============================================================================
// ESTRUCTURAS PARA ARCHIVOS BINARIOS
// =============================================================================

struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;
};

struct HistorialMedico {
    int id;
    int pacienteID;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    
    int siguienteConsultaID;
    
    bool eliminado;
    time_t fechaRegistro;
};

struct Paciente {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    bool activo;
    
    int cantidadConsultas;
    int primerConsultaID;
    
    int cantidadCitas;
    int citasIDs[20];
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    
    int cantidadPacientes;
    int pacientesIDs[50];
    
    int cantidadCitas;
    int citasIDs[30];
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Cita {
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
    
    int consultaID;
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

// =============================================================================
// PROTOTIPOS DE FUNCIONES
// =============================================================================

// MÓDULO DE GESTIÓN DE ARCHIVOS
bool inicializarArchivo(const char* nombreArchivo);
bool verificarArchivo(const char* nombreArchivo);
ArchivoHeader leerHeader(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
long calcularPosicion(int indice, size_t tamañoRegistro);

// MÓDULO DE HOSPITAL
Hospital* cargarDatosHospital();
bool guardarDatosHospital(Hospital* hospital);
bool actualizarDatosHospital(Hospital* hospital);

// MÓDULO DE PACIENTES
bool agregarPaciente(Hospital* hospital, Paciente* paciente);
Paciente* buscarPacientePorID(Hospital* hospital, int id);
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula);
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad);
bool actualizarPaciente(Paciente* paciente);
bool eliminarPaciente(int id);
void listarTodosPacientes(Hospital* hospital);
Paciente leerPacientePorIndice(int indice);

// MÓDULO DE DOCTORES
bool agregarDoctor(Hospital* hospital, Doctor* doctor);
Doctor* buscarDoctorPorID(Hospital* hospital, int id);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad);
bool actualizarDoctor(Doctor* doctor);
bool eliminarDoctor(int id);
void listarTodosDoctores(Hospital* hospital);

// MÓDULO DE CITAS
bool agendarCita(Hospital* hospital, Cita* cita);
bool cancelarCita(int id);
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos);
Cita** obtenerCitasDePaciente(int idPaciente, int* cantidad);
Cita** obtenerCitasDeDoctor(int idDoctor, int* cantidad);
Cita** obtenerCitasPorFecha(const char* fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);
bool verificarDisponibilidad(int idDoctor, const char* fecha, const char* hora);

// MÓDULO DE HISTORIAL MÉDICO
bool agregarConsultaAlHistorial(Hospital* hospital, HistorialMedico* consulta);
HistorialMedico* leerHistorialCompleto(int pacienteID, int* cantidad);
void mostrarHistorialMedico(Hospital* hospital, int pacienteID);
HistorialMedico* obtenerUltimaConsulta(int pacienteID);

// MÓDULO DE UTILIDADES
bool validarFecha(const char* fecha);
bool validarHora(const char* hora);
bool validarCedula(const char* cedula);
bool validarEmail(const char* email);
void limpiarBuffer();
void mostrarMenuPrincipal();
void mostrarMenuPacientes();
void mostrarMenuDoctores();
void mostrarMenuCitas();
void mostrarMenuMantenimiento();
void ejecutarMenuPacientes(Hospital* hospital);
void ejecutarMenuDoctores(Hospital* hospital);
void ejecutarMenuCitas(Hospital* hospital);
void ejecutarMenuMantenimiento(Hospital* hospital);

// =============================================================================
// IMPLEMENTACIÓN DE GESTIÓN DE ARCHIVOS
// =============================================================================

bool inicializarArchivo(const char* nombreArchivo) {
    fstream archivo(nombreArchivo, ios::binary | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    ArchivoHeader header;
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;
    
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    return true;
}

bool verificarArchivo(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }
    
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    return header.version == 1;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    ifstream archivo(nombreArchivo, ios::binary);
    
    if (archivo.is_open()) {
        archivo.read((char*)&header, sizeof(ArchivoHeader));
        archivo.close();
    } else {
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 0;
    }
    
    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    
    return true;
}

long calcularPosicion(int indice, size_t tamañoRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamañoRegistro);
}

// =============================================================================
// IMPLEMENTACIÓN DE MÓDULO HOSPITAL
// =============================================================================

Hospital* cargarDatosHospital() {
    // Verificar y crear archivos si no existen
    const char* archivos[] = {
        "hospital.bin", "pacientes.bin", "doctores.bin", 
        "citas.bin", "historiales.bin"
    };
    
    for (int i = 0; i < 5; i++) {
        if (!verificarArchivo(archivos[i])) {
            if (!inicializarArchivo(archivos[i])) {
                cout << "Error: No se pudo crear " << archivos[i] << endl;
                return nullptr;
            }
        }
    }
    
    // Cargar datos del hospital
    Hospital* hospital = new Hospital;
    ifstream archivo("hospital.bin", ios::binary);
    
    if (archivo.is_open()) {
        archivo.read((char*)hospital, sizeof(Hospital));
        archivo.close();
    } else {
        // Valores por defecto
        strcpy(hospital->nombre, "Hospital Central");
        strcpy(hospital->direccion, "Dirección no especificada");
        strcpy(hospital->telefono, "000-000-0000");
        hospital->siguienteIDPaciente = 1;
        hospital->siguienteIDDoctor = 1;
        hospital->siguienteIDCita = 1;
        hospital->siguienteIDConsulta = 1;
        hospital->totalPacientesRegistrados = 0;
        hospital->totalDoctoresRegistrados = 0;
        hospital->totalCitasAgendadas = 0;
        hospital->totalConsultasRealizadas = 0;
    }
    
    cout << "Sistema hospitalario cargado correctamente." << endl;
    return hospital;
}

bool guardarDatosHospital(Hospital* hospital) {
    ofstream archivo("hospital.bin", ios::binary);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.write((char*)hospital, sizeof(Hospital));
    archivo.close();
    
    // Actualizar headers de otros archivos
    ArchivoHeader header;
    
    // Actualizar header de pacientes
    header = leerHeader("pacientes.bin");
    header.proximoID = hospital->siguienteIDPaciente;
    actualizarHeader("pacientes.bin", header);
    
    // Actualizar header de doctores
    header = leerHeader("doctores.bin");
    header.proximoID = hospital->siguienteIDDoctor;
    actualizarHeader("doctores.bin", header);
    
    // Actualizar header de citas
    header = leerHeader("citas.bin");
    header.proximoID = hospital->siguienteIDCita;
    actualizarHeader("citas.bin", header);
    
    // Actualizar header de historiales
    header = leerHeader("historiales.bin");
    header.proximoID = hospital->siguienteIDConsulta;
    actualizarHeader("historiales.bin", header);
    
    cout << "Datos del hospital guardados correctamente." << endl;
    return true;
}

bool actualizarDatosHospital(Hospital* hospital) {
    fstream archivo("hospital.bin", ios::binary | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.write((char*)hospital, sizeof(Hospital));
    archivo.close();
    return true;
}

// =============================================================================
// IMPLEMENTACIÓN DE MÓDULO PACIENTES
// =============================================================================

bool agregarPaciente(Hospital* hospital, Paciente* paciente) {
    // Verificar cédula única
    if (buscarPacientePorCedula(hospital, paciente->cedula) != nullptr) {
        cout << "Error: Ya existe un paciente con esta cédula." << endl;
        return false;
    }
    
    // Configurar datos del paciente
    paciente->id = hospital->siguienteIDPaciente++;
    paciente->activo = true;
    paciente->eliminado = false;
    paciente->cantidadConsultas = 0;
    paciente->primerConsultaID = -1;
    paciente->cantidadCitas = 0;
    time(&paciente->fechaCreacion);
    paciente->fechaModificacion = paciente->fechaCreacion;
    
    // Abrir archivo y agregar paciente
    ofstream archivo("pacientes.bin", ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de pacientes." << endl;
        return false;
    }
    
    archivo.write((char*)paciente, sizeof(Paciente));
    archivo.close();
    
    // Actualizar header
    ArchivoHeader header = leerHeader("pacientes.bin");
    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID = hospital->siguienteIDPaciente;
    actualizarHeader("pacientes.bin", header);
    
    // Actualizar estadísticas del hospital
    hospital->totalPacientesRegistrados++;
    actualizarDatosHospital(hospital);
    
    cout << "Paciente creado exitosamente. ID: " << paciente->id << endl;
    return true;
}

Paciente* buscarPacientePorID(Hospital* hospital, int id) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivo("pacientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Saltar header
    archivo.seekg(sizeof(ArchivoHeader));
    
    Paciente* paciente = new Paciente;
    bool encontrado = false;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)paciente, sizeof(Paciente));
        if (paciente->id == id && !paciente->eliminado) {
            encontrado = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        delete paciente;
        return nullptr;
    }
    
    return paciente;
}

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivo("pacientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    
    Paciente* paciente = new Paciente;
    bool encontrado = false;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)paciente, sizeof(Paciente));
        if (strcasecmp(paciente->cedula, cedula) == 0 && !paciente->eliminado) {
            encontrado = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        delete paciente;
        return nullptr;
    }
    
    return paciente;
}

Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivo("pacientes.bin", ios::binary);
    
    *cantidad = 0;
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Primero contar coincidencias
    archivo.seekg(sizeof(ArchivoHeader));
    Paciente temp;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Paciente));
        if (!temp.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", temp.nombre, temp.apellido);
            
            // Búsqueda case-insensitive
            char nombreLower[100], busquedaLower[100];
            strcpy(nombreLower, nombreCompleto);
            strcpy(busquedaLower, nombre);
            
            for (int j = 0; nombreLower[j]; j++) nombreLower[j] = tolower(nombreLower[j]);
            for (int j = 0; busquedaLower[j]; j++) busquedaLower[j] = tolower(busquedaLower[j]);
            
            if (strstr(nombreLower, busquedaLower) != nullptr) {
                (*cantidad)++;
            }
        }
    }
    
    if (*cantidad == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Crear array de resultados
    Paciente** resultados = new Paciente*[*cantidad];
    int index = 0;
    
    // Volver al inicio y llenar resultados
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Paciente));
        if (!temp.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", temp.nombre, temp.apellido);
            
            char nombreLower[100], busquedaLower[100];
            strcpy(nombreLower, nombreCompleto);
            strcpy(busquedaLower, nombre);
            
            for (int j = 0; nombreLower[j]; j++) nombreLower[j] = tolower(nombreLower[j]);
            for (int j = 0; busquedaLower[j]; j++) busquedaLower[j] = tolower(busquedaLower[j]);
            
            if (strstr(nombreLower, busquedaLower) != nullptr) {
                resultados[index] = new Paciente;
                *resultados[index] = temp;
                index++;
            }
        }
    }
    
    archivo.close();
    return resultados;
}

bool actualizarPaciente(Paciente* paciente) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    fstream archivo("pacientes.bin", ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Paciente temp;
    long posicion = -1;
    
    // Buscar paciente
    for (int i = 0; i < header.cantidadRegistros; i++) {
        posicion = archivo.tellg();
        archivo.read((char*)&temp, sizeof(Paciente));
        if (temp.id == paciente->id) {
            break;
        }
    }
    
    if (posicion == -1) {
        archivo.close();
        return false;
    }
    
    // Actualizar fecha de modificación
    time(&paciente->fechaModificacion);
    
    // Sobrescribir paciente
    archivo.seekp(posicion);
    archivo.write((char*)paciente, sizeof(Paciente));
    archivo.close();
    
    cout << "Paciente actualizado correctamente." << endl;
    return true;
}

bool eliminarPaciente(int id) {
    Paciente* paciente = nullptr;
    Hospital hospitalTemp;
    // Necesitaríamos cargar el hospital para buscar, pero por simplicidad
    // asumimos que tenemos acceso al hospital
    
    // En una implementación completa, buscaríamos el paciente y marcaríamos como eliminado
    cout << "Función eliminarPaciente requiere implementación completa." << endl;
    return false;
}

void listarTodosPacientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivo("pacientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de pacientes." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    LISTA DE PACIENTES                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CÉDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
    cout << "¦-----+---------------------+--------------+------+----------¦" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    Paciente paciente;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&paciente, sizeof(Paciente));
        if (!paciente.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", paciente.nombre, paciente.apellido);
            
            cout << "¦ " << setw(3) << paciente.id << " ¦ "
                 << setw(19) << left << nombreCompleto << " ¦ "
                 << setw(12) << paciente.cedula << " ¦ "
                 << setw(4) << paciente.edad << " ¦ "
                 << setw(8) << paciente.cantidadConsultas << "¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "¦               NO HAY PACIENTES REGISTRADOS               ¦" << endl;
    }
    
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Total de pacientes activos: " << contador << endl;
}

Paciente leerPacientePorIndice(int indice) {
    Paciente paciente;
    memset(&paciente, 0, sizeof(Paciente)); // Inicializar con ceros
    
    ifstream archivo("pacientes.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de pacientes." << endl;
        return paciente;
    }
    
    // Verificar que el índice sea válido
    ArchivoHeader header = leerHeader("pacientes.bin");
    if (indice < 0 || indice >= header.cantidadRegistros) {
        cout << "Error: Índice fuera de rango." << endl;
        archivo.close();
        return paciente;
    }
    
    // Calcular posición exacta (ACCESO ALEATORIO)
    long posicion = calcularPosicion(indice, sizeof(Paciente));
    
    // Posicionarse y leer
    archivo.seekg(posicion);
    archivo.read((char*)&paciente, sizeof(Paciente));
    
    if (archivo.fail()) {
        cout << "Error: No se pudo leer el paciente." << endl;
        memset(&paciente, 0, sizeof(Paciente));
    }
    
    archivo.close();
    return paciente;
}