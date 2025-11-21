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

struct ArchivoHeader;
struct Hospital;
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
    
    // Contadores de IDs (auto-increment)
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadisticas generales
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
    
    // Navegacion enlazada
    int siguienteConsultaID;
    
    // Metadata
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
    
    // Indices para relaciones
    int cantidadConsultas;
    int primerConsultaID;
    
    int cantidadCitas;
    int citasIDs[20];
    
    // Metadata de registro
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
    
    // Relaciones con arrays fijos
    int cantidadPacientes;
    int pacientesIDs[50];
    
    int cantidadCitas;
    int citasIDs[30];
    
    // Metadata
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
    
    // Referencia al historial
    int consultaID;
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

// =============================================================================
// PROTOTIPOS DE FUNCIONES
// =============================================================================

// MODULO DE GESTION DE ARCHIVOS
bool inicializarArchivo(const char* nombreArchivo);
bool verificarArchivo(const char* nombreArchivo);
ArchivoHeader leerHeader(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
long calcularPosicion(int indice, size_t tamanioRegistro);

// MODULO DE HOSPITAL
Hospital* cargarDatosHospital();
bool guardarDatosHospital(Hospital* hospital);
bool actualizarDatosHospital(Hospital* hospital);

// MODULO DE PACIENTES
bool agregarPaciente(Hospital* hospital, Paciente* paciente);
Paciente* buscarPacientePorID(Hospital* hospital, int id);
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula);
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad);
bool actualizarPaciente(Paciente* paciente);
bool eliminarPaciente(Hospital* hospital, int id);
void listarTodosPacientes(Hospital* hospital);
Paciente leerPacientePorIndice(int indice);
bool compactarArchivoPacientes(Hospital* hospital);

// MODULO DE DOCTORES
bool agregarDoctor(Hospital* hospital, Doctor* doctor);
Doctor* buscarDoctorPorID(Hospital* hospital, int id);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad);
bool asignarPacienteADoctor(Hospital* hospital, int idDoctor, int idPaciente);
bool removerPacienteDeDoctor(Hospital* hospital, int idDoctor, int idPaciente);
void listarPacientesDeDoctor(Hospital* hospital, int idDoctor);
void listarTodosDoctores(Hospital* hospital);
bool eliminarDoctor(Hospital* hospital, int id);
bool compactarArchivoDoctores(Hospital* hospital);

// MODULO DE CITAS
bool agendarCita(Hospital* hospital, Cita* cita);
bool cancelarCita(Hospital* hospital, int idCita);
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos);
Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad);
Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad);
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);
bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora);

// MODULO DE HISTORIAL MEDICO
bool agregarConsultaAlHistorial(Hospital* hospital, HistorialMedico* consulta);
HistorialMedico* leerHistorialCompleto(int pacienteID, int* cantidad);
void mostrarHistorialMedico(Hospital* hospital, int pacienteID);
HistorialMedico* obtenerUltimaConsulta(int pacienteID);

// MODULO DE UTILIDADES
bool validarFecha(const char* fecha);
bool validarHora(const char* hora);
bool validarCedula(const char* cedula);
bool validarEmail(const char* email);
bool validarEdad(int edad);
bool validarTipoSangre(const char* tipoSangre);
bool validarTelefono(const char* telefono);
bool validarSoloLetras(const char* texto);
bool validarSoloNumeros(const char* texto);
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
// IMPLEMENTACION DE VALIDACIONES MEJORADAS
// =============================================================================

bool validarEdad(int edad) {
    return (edad >= 0 && edad <= 120);
}

bool validarTipoSangre(const char* tipoSangre) {
    const char* tiposValidos[] = {"O+", "O-", "A+", "A-", "B+", "B-", "AB+", "AB-", ""};
    for (int i = 0; i < 9; i++) {
        if (strcasecmp(tipoSangre, tiposValidos[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool validarTelefono(const char* telefono) {
    if (telefono == nullptr || strlen(telefono) == 0) return false;
    
    // Verificar que solo tenga digitos, espacios, guiones y parentesis
    for (int i = 0; telefono[i] != '\0'; i++) {
        if (!isdigit(telefono[i]) && telefono[i] != ' ' && telefono[i] != '-' && 
            telefono[i] != '(' && telefono[i] != ')') {
            return false;
        }
    }
    return strlen(telefono) <= 15;
}

bool validarSoloLetras(const char* texto) {
    if (texto == nullptr) return false;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isalpha(texto[i]) && texto[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool validarSoloNumeros(const char* texto) {
    if (texto == nullptr) return false;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isdigit(texto[i])) {
            return false;
        }
    }
    return true;
}

// =============================================================================
// IMPLEMENTACION DE GESTION DE ARCHIVOS
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

long calcularPosicion(int indice, size_t tamanioRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamanioRegistro);
}

// =============================================================================
// IMPLEMENTACION DE MODULO HOSPITAL
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
        strcpy(hospital->direccion, "Direccion no especificada");
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
// IMPLEMENTACION DE MODULO PACIENTES
// =============================================================================

bool agregarPaciente(Hospital* hospital, Paciente* paciente) {
    // Verificar cedula unica
    if (buscarPacientePorCedula(hospital, paciente->cedula) != nullptr) {
        cout << "Error: Ya existe un paciente con esta cedula." << endl;
        return false;
    }
    
    // Validaciones
    if (!validarEdad(paciente->edad)) {
        cout << "Error: Edad invalida. Debe estar entre 0 y 120 anios." << endl;
        return false;
    }
    
    if (!validarEmail(paciente->email)) {
        cout << "Error: Email invalido." << endl;
        return false;
    }
    
    if (!validarTipoSangre(paciente->tipoSangre)) {
        cout << "Error: Tipo de sangre invalido. Use: O+, O-, A+, A-, B+, B-, AB+, AB-" << endl;
        return false;
    }
    
    if (!validarTelefono(paciente->telefono)) {
        cout << "Error: Telefono invalido." << endl;
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
    
    // Actualizar estadisticas del hospital
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
            
            // Busqueda case-insensitive
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
    
    // Actualizar fecha de modificacion
    time(&paciente->fechaModificacion);
    
    // Sobrescribir paciente
    archivo.seekp(posicion);
    archivo.write((char*)paciente, sizeof(Paciente));
    archivo.close();
    
    return true;
}

bool eliminarPaciente(Hospital* hospital, int id) {
    Paciente* paciente = buscarPacientePorID(hospital, id);
    if (paciente == nullptr) {
        cout << "Paciente no encontrado." << endl;
        return false;
    }
    
    // Marcar como eliminado
    paciente->eliminado = true;
    paciente->activo = false;
    
    if (actualizarPaciente(paciente)) {
        // Actualizar header
        ArchivoHeader header = leerHeader("pacientes.bin");
        header.registrosActivos--;
        actualizarHeader("pacientes.bin", header);
        
        // Compactar archivo para reordenar IDs
        compactarArchivoPacientes(hospital);
        
        cout << "Paciente eliminado exitosamente." << endl;
        delete paciente;
        return true;
    }
    
    delete paciente;
    return false;
}

bool compactarArchivoPacientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivoEntrada("pacientes.bin", ios::binary);
    
    if (!archivoEntrada.is_open()) {
        return false;
    }
    
    // Crear archivo temporal
    ofstream archivoTemp("pacientes_temp.bin", ios::binary);
    if (!archivoTemp.is_open()) {
        archivoEntrada.close();
        return false;
    }
    
    // Escribir header temporal
    ArchivoHeader headerTemp;
    headerTemp.cantidadRegistros = 0;
    headerTemp.proximoID = 1;
    headerTemp.registrosActivos = 0;
    headerTemp.version = 1;
    archivoTemp.write((char*)&headerTemp, sizeof(ArchivoHeader));
    
    // Leer pacientes y copiar solo los no eliminados
    archivoEntrada.seekg(sizeof(ArchivoHeader));
    Paciente paciente;
    int nuevoID = 1;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivoEntrada.read((char*)&paciente, sizeof(Paciente));
        if (!paciente.eliminado) {
            paciente.id = nuevoID++;
            archivoTemp.write((char*)&paciente, sizeof(Paciente));
            headerTemp.registrosActivos++;
            headerTemp.cantidadRegistros++;
        }
    }
    
    archivoEntrada.close();
    archivoTemp.close();
    
    // Actualizar header del archivo temporal
    headerTemp.proximoID = nuevoID;
    fstream archivoTemp2("pacientes_temp.bin", ios::binary | ios::in | ios::out);
    archivoTemp2.seekp(0);
    archivoTemp2.write((char*)&headerTemp, sizeof(ArchivoHeader));
    archivoTemp2.close();
    
    // Reemplazar archivo original
    remove("pacientes.bin");
    rename("pacientes_temp.bin", "pacientes.bin");
    
    // Actualizar hospital
    hospital->siguienteIDPaciente = nuevoID;
    hospital->totalPacientesRegistrados = headerTemp.registrosActivos;
    actualizarDatosHospital(hospital);
    
    return true;
}

void listarTodosPacientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    ifstream archivo("pacientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de pacientes." << endl;
        return;
    }
    
    cout << "\n+----------------------------------------------------------------------------------------+" << endl;
    cout << "|                                LISTA DE PACIENTES                                     |" << endl;
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      |   CEDULA     | EDAD | CONSULTAS |       TELEFONO         |" << endl;
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    Paciente paciente;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&paciente, sizeof(Paciente));
        if (!paciente.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", paciente.nombre, paciente.apellido);
            
            cout << "| " << setw(4) << paciente.id << " | "
                 << setw(20) << left << nombreCompleto << " | "
                 << setw(12) << paciente.cedula << " | "
                 << setw(4) << paciente.edad << " | "
                 << setw(8) << paciente.cantidadConsultas << " | "
                 << setw(23) << paciente.telefono << " |" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "|                      NO HAY PACIENTES REGISTRADOS                                 |" << endl;
    }
    
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    cout << "Total de pacientes activos: " << contador << endl;
}

Paciente leerPacientePorIndice(int indice) {
    Paciente paciente;
    memset(&paciente, 0, sizeof(Paciente));
    
    ifstream archivo("pacientes.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de pacientes." << endl;
        return paciente;
    }
    
    // Verificar que el indice sea valido
    ArchivoHeader header = leerHeader("pacientes.bin");
    if (indice < 0 || indice >= header.cantidadRegistros) {
        cout << "Error: Indice fuera de rango." << endl;
        archivo.close();
        return paciente;
    }
    
    // Calcular posicion exacta (ACCESO ALEATORIO)
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

// =============================================================================
// IMPLEMENTACION DE MODULO DOCTORES
// =============================================================================

bool agregarDoctor(Hospital* hospital, Doctor* doctor) {
    // Validaciones
    if (doctor->aniosExperiencia < 0) {
        cout << "Error: Los anios de experiencia no pueden ser negativos." << endl;
        return false;
    }
    
    if (doctor->costoConsulta <= 0) {
        cout << "Error: El costo de consulta debe ser mayor a 0." << endl;
        return false;
    }
    
    if (!validarEmail(doctor->email)) {
        cout << "Error: Email invalido." << endl;
        return false;
    }
    
    if (!validarTelefono(doctor->telefono)) {
        cout << "Error: Telefono invalido." << endl;
        return false;
    }
    
    // Configurar datos del doctor
    doctor->id = hospital->siguienteIDDoctor++;
    doctor->disponible = true;
    doctor->eliminado = false;
    doctor->cantidadPacientes = 0;
    doctor->cantidadCitas = 0;
    time(&doctor->fechaCreacion);
    doctor->fechaModificacion = doctor->fechaCreacion;
    
    // Abrir archivo y agregar doctor
    ofstream archivo("doctores.bin", ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de doctores." << endl;
        return false;
    }
    
    archivo.write((char*)doctor, sizeof(Doctor));
    archivo.close();
    
    // Actualizar header
    ArchivoHeader header = leerHeader("doctores.bin");
    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID = hospital->siguienteIDDoctor;
    actualizarHeader("doctores.bin", header);
    
    // Actualizar estadisticas del hospital
    hospital->totalDoctoresRegistrados++;
    actualizarDatosHospital(hospital);
    
    cout << "Doctor creado exitosamente. ID: " << doctor->id << endl;
    return true;
}

Doctor* buscarDoctorPorID(Hospital* hospital, int id) {
    ArchivoHeader header = leerHeader("doctores.bin");
    ifstream archivo("doctores.bin", ios::binary);
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    
    Doctor* doctor = new Doctor;
    bool encontrado = false;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)doctor, sizeof(Doctor));
        if (doctor->id == id && !doctor->eliminado) {
            encontrado = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        delete doctor;
        return nullptr;
    }
    
    return doctor;
}

Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad) {
    ArchivoHeader header = leerHeader("doctores.bin");
    ifstream archivo("doctores.bin", ios::binary);
    
    *cantidad = 0;
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Primero contar coincidencias
    archivo.seekg(sizeof(ArchivoHeader));
    Doctor temp;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Doctor));
        if (!temp.eliminado && strcasecmp(temp.especialidad, especialidad) == 0) {
            (*cantidad)++;
        }
    }
    
    if (*cantidad == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Crear array de resultados
    Doctor** resultados = new Doctor*[*cantidad];
    int index = 0;
    
    // Volver al inicio y llenar resultados
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Doctor));
        if (!temp.eliminado && strcasecmp(temp.especialidad, especialidad) == 0) {
            resultados[index] = new Doctor;
            *resultados[index] = temp;
            index++;
        }
    }
    
    archivo.close();
    return resultados;
}

bool asignarPacienteADoctor(Hospital* hospital, int idDoctor, int idPaciente) {
    Doctor* doctor = buscarDoctorPorID(hospital, idDoctor);
    Paciente* paciente = buscarPacientePorID(hospital, idPaciente);
    
    if (doctor == nullptr || paciente == nullptr) {
        cout << "Error: Doctor o paciente no encontrado." << endl;
        return false;
    }
    
    // Verificar si ya esta asignado
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesIDs[i] == idPaciente) {
            cout << "El paciente ya esta asignado a este doctor." << endl;
            delete doctor;
            delete paciente;
            return false;
        }
    }
    
    // Verificar capacidad
    if (doctor->cantidadPacientes >= 50) {
        cout << "Error: El doctor ya tiene el maximo de pacientes asignados." << endl;
        delete doctor;
        delete paciente;
        return false;
    }
    
    // Asignar paciente
    doctor->pacientesIDs[doctor->cantidadPacientes] = idPaciente;
    doctor->cantidadPacientes++;
    
    // Actualizar en archivo
    ArchivoHeader header = leerHeader("doctores.bin");
    fstream archivo("doctores.bin", ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        delete doctor;
        delete paciente;
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Doctor temp;
    long posicion = -1;
    
    // Buscar doctor
    for (int i = 0; i < header.cantidadRegistros; i++) {
        posicion = archivo.tellg();
        archivo.read((char*)&temp, sizeof(Doctor));
        if (temp.id == idDoctor) {
            break;
        }
    }
    
    if (posicion != -1) {
        archivo.seekp(posicion);
        archivo.write((char*)doctor, sizeof(Doctor));
    }
    
    archivo.close();
    
    cout << "Paciente asignado exitosamente al doctor." << endl;
    delete doctor;
    delete paciente;
    return true;
}

void listarPacientesDeDoctor(Hospital* hospital, int idDoctor) {
    Doctor* doctor = buscarDoctorPorID(hospital, idDoctor);
    if (doctor == nullptr) {
        cout << "Doctor no encontrado." << endl;
        return;
    }
    
    if (doctor->cantidadPacientes == 0) {
        cout << "El doctor no tiene pacientes asignados." << endl;
        delete doctor;
        return;
    }
    
    cout << "\n+----------------------------------------------------------------------------------------+" << endl;
    cout << "|                    PACIENTES ASIGNADOS - Dr. " << doctor->nombre << " " << doctor->apellido << setw(30) << "|" << endl;
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      |   CEDULA     | EDAD | CONSULTAS |       TELEFONO         |" << endl;
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        Paciente* paciente = buscarPacientePorID(hospital, doctor->pacientesIDs[i]);
        if (paciente != nullptr && !paciente->eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", paciente->nombre, paciente->apellido);
            
            cout << "| " << setw(4) << paciente->id << " | "
                 << setw(20) << left << nombreCompleto << " | "
                 << setw(12) << paciente->cedula << " | "
                 << setw(4) << paciente->edad << " | "
                 << setw(8) << paciente->cantidadConsultas << " | "
                 << setw(23) << paciente->telefono << " |" << endl;
            
            delete paciente;
        }
    }
    
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    delete doctor;
}

void listarTodosDoctores(Hospital* hospital) {
    ArchivoHeader header = leerHeader("doctores.bin");
    ifstream archivo("doctores.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de doctores." << endl;
        return;
    }
    
    cout << "\n+--------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                      LISTA DE DOCTORES                                          |" << endl;
    cout << "+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      | ESPECIALIDAD          | EXP. |   COSTO   |  PAC.  |    HORARIO     |" << endl;
    cout << "+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    Doctor doctor;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&doctor, sizeof(Doctor));
        if (!doctor.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", doctor.nombre, doctor.apellido);
            
            cout << "| " << setw(4) << doctor.id << " | "
                 << setw(20) << left << nombreCompleto << " | "
                 << setw(21) << doctor.especialidad << " | "
                 << setw(4) << doctor.aniosExperiencia << " | $"
                 << setw(8) << fixed << setprecision(2) << doctor.costoConsulta << " | "
                 << setw(6) << doctor.cantidadPacientes << " | "
                 << setw(14) << doctor.horarioAtencion << " |" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "|                               NO HAY DOCTORES REGISTRADOS                                   |" << endl;
    }
    
    cout << "+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    cout << "Total de doctores activos: " << contador << endl;
}

bool eliminarDoctor(Hospital* hospital, int id) {
    Doctor* doctor = buscarDoctorPorID(hospital, id);
    if (doctor == nullptr) {
        cout << "Doctor no encontrado." << endl;
        return false;
    }
    
    // Marcar como eliminado
    doctor->eliminado = true;
    doctor->disponible = false;
    
    // Actualizar en archivo
    ArchivoHeader header = leerHeader("doctores.bin");
    fstream archivo("doctores.bin", ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        delete doctor;
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Doctor temp;
    long posicion = -1;
    
    // Buscar doctor
    for (int i = 0; i < header.cantidadRegistros; i++) {
        posicion = archivo.tellg();
        archivo.read((char*)&temp, sizeof(Doctor));
        if (temp.id == id) {
            break;
        }
    }
    
    if (posicion != -1) {
        archivo.seekp(posicion);
        archivo.write((char*)doctor, sizeof(Doctor));
        
        // Actualizar header
        header.registrosActivos--;
        actualizarHeader("doctores.bin", header);
        
        // Compactar archivo
        compactarArchivoDoctores(hospital);
        
        cout << "Doctor eliminado exitosamente." << endl;
    }
    
    archivo.close();
    delete doctor;
    return true;
}

bool compactarArchivoDoctores(Hospital* hospital) {
    ArchivoHeader header = leerHeader("doctores.bin");
    ifstream archivoEntrada("doctores.bin", ios::binary);
    
    if (!archivoEntrada.is_open()) {
        return false;
    }
    
    // Crear archivo temporal
    ofstream archivoTemp("doctores_temp.bin", ios::binary);
    if (!archivoTemp.is_open()) {
        archivoEntrada.close();
        return false;
    }
    
    // Escribir header temporal
    ArchivoHeader headerTemp;
    headerTemp.cantidadRegistros = 0;
    headerTemp.proximoID = 1;
    headerTemp.registrosActivos = 0;
    headerTemp.version = 1;
    archivoTemp.write((char*)&headerTemp, sizeof(ArchivoHeader));
    
    // Leer doctores y copiar solo los no eliminados
    archivoEntrada.seekg(sizeof(ArchivoHeader));
    Doctor doctor;
    int nuevoID = 1;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivoEntrada.read((char*)&doctor, sizeof(Doctor));
        if (!doctor.eliminado) {
            doctor.id = nuevoID++;
            archivoTemp.write((char*)&doctor, sizeof(Doctor));
            headerTemp.registrosActivos++;
            headerTemp.cantidadRegistros++;
        }
    }
    
    archivoEntrada.close();
    archivoTemp.close();
    
    // Actualizar header del archivo temporal
    headerTemp.proximoID = nuevoID;
    fstream archivoTemp2("doctores_temp.bin", ios::binary | ios::in | ios::out);
    archivoTemp2.seekp(0);
    archivoTemp2.write((char*)&headerTemp, sizeof(ArchivoHeader));
    archivoTemp2.close();
    
    // Reemplazar archivo original
    remove("doctores.bin");
    rename("doctores_temp.bin", "doctores.bin");
    
    // Actualizar hospital
    hospital->siguienteIDDoctor = nuevoID;
    hospital->totalDoctoresRegistrados = headerTemp.registrosActivos;
    actualizarDatosHospital(hospital);
    
    return true;
}

// =============================================================================
// IMPLEMENTACION DE MODULO CITAS
// =============================================================================

bool agendarCita(Hospital* hospital, Cita* cita) {
    // Verificar que paciente y doctor existen
    Paciente* paciente = buscarPacientePorID(hospital, cita->pacienteID);
    Doctor* doctor = buscarDoctorPorID(hospital, cita->doctorID);
    
    if (paciente == nullptr || doctor == nullptr) {
        cout << "Error: Paciente o doctor no encontrado." << endl;
        return false;
    }
    
    // Validar fecha y hora
    if (!validarFecha(cita->fecha)) {
        cout << "Error: Formato de fecha invalido. Use YYYY-MM-DD." << endl;
        delete paciente;
        delete doctor;
        return false;
    }
    
    if (!validarHora(cita->hora)) {
        cout << "Error: Formato de hora invalido. Use HH:MM." << endl;
        delete paciente;
        delete doctor;
        return false;
    }
    
    // Verificar disponibilidad
    if (!verificarDisponibilidad(hospital, cita->doctorID, cita->fecha, cita->hora)) {
        cout << "Error: El doctor no esta disponible en esa fecha y hora." << endl;
        delete paciente;
        delete doctor;
        return false;
    }
    
    // Configurar datos de la cita
    cita->id = hospital->siguienteIDCita++;
    strcpy(cita->estado, "Agendada");
    cita->atendida = false;
    cita->consultaID = -1;
    cita->eliminado = false;
    time(&cita->fechaCreacion);
    cita->fechaModificacion = cita->fechaCreacion;
    
    // Abrir archivo y agregar cita
    ofstream archivo("citas.bin", ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        delete paciente;
        delete doctor;
        return false;
    }
    
    archivo.write((char*)cita, sizeof(Cita));
    archivo.close();
    
    // Actualizar header
    ArchivoHeader header = leerHeader("citas.bin");
    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID = hospital->siguienteIDCita;
    actualizarHeader("citas.bin", header);
    
    // Actualizar estadisticas del hospital
    hospital->totalCitasAgendadas++;
    actualizarDatosHospital(hospital);
    
    cout << "Cita agendada exitosamente. ID: " << cita->id << endl;
    delete paciente;
    delete doctor;
    return true;
}

bool cancelarCita(Hospital* hospital, int idCita) {
    ArchivoHeader header = leerHeader("citas.bin");
    fstream archivo("citas.bin", ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Cita cita;
    long posicion = -1;
    bool encontrada = false;
    
    // Buscar cita
    for (int i = 0; i < header.cantidadRegistros; i++) {
        posicion = archivo.tellg();
        archivo.read((char*)&cita, sizeof(Cita));
        if (cita.id == idCita && !cita.eliminado) {
            encontrada = true;
            break;
        }
    }
    
    if (!encontrada) {
        cout << "Cita no encontrada." << endl;
        archivo.close();
        return false;
    }
    
    if (cita.atendida) {
        cout << "No se puede cancelar una cita ya atendida." << endl;
        archivo.close();
        return false;
    }
    
    // Actualizar estado
    strcpy(cita.estado, "Cancelada");
    time(&cita.fechaModificacion);
    
    // Sobrescribir cita
    archivo.seekp(posicion);
    archivo.write((char*)&cita, sizeof(Cita));
    archivo.close();
    
    cout << "Cita cancelada exitosamente." << endl;
    return true;
}

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos) {
    ArchivoHeader header = leerHeader("citas.bin");
    fstream archivo("citas.bin", ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Cita cita;
    long posicion = -1;
    bool encontrada = false;
    
    // Buscar cita
    for (int i = 0; i < header.cantidadRegistros; i++) {
        posicion = archivo.tellg();
        archivo.read((char*)&cita, sizeof(Cita));
        if (cita.id == idCita && !cita.eliminado) {
            encontrada = true;
            break;
        }
    }
    
    if (!encontrada) {
        cout << "Cita no encontrada." << endl;
        archivo.close();
        return false;
    }
    
    if (cita.atendida) {
        cout << "La cita ya fue atendida anteriormente." << endl;
        archivo.close();
        return false;
    }
    
    // Crear registro de historial medico
    HistorialMedico consulta;
    consulta.id = hospital->siguienteIDConsulta++;
    consulta.pacienteID = cita.pacienteID;
    strcpy(consulta.fecha, cita.fecha);
    strcpy(consulta.hora, cita.hora);
    strncpy(consulta.diagnostico, diagnostico, 199);
    strncpy(consulta.tratamiento, tratamiento, 199);
    strncpy(consulta.medicamentos, medicamentos, 149);
    consulta.doctorID = cita.doctorID;
    
    // Obtener costo del doctor
    Doctor* doctor = buscarDoctorPorID(hospital, cita.doctorID);
    if (doctor != nullptr) {
        consulta.costo = doctor->costoConsulta;
        delete doctor;
    } else {
        consulta.costo = 0.0f;
    }
    
    consulta.siguienteConsultaID = -1;
    consulta.eliminado = false;
    time(&consulta.fechaRegistro);
    
    // Agregar al historial
    if (!agregarConsultaAlHistorial(hospital, &consulta)) {
        cout << "Error: No se pudo registrar en el historial medico." << endl;
        archivo.close();
        return false;
    }
    
    // Actualizar cita
    cita.atendida = true;
    strcpy(cita.estado, "Atendida");
    cita.consultaID = consulta.id;
    strncpy(cita.observaciones, diagnostico, 199);
    time(&cita.fechaModificacion);
    
    // Sobrescribir cita
    archivo.seekp(posicion);
    archivo.write((char*)&cita, sizeof(Cita));
    archivo.close();
    
    cout << "Cita atendida y registrada en historial medico." << endl;
    return true;
}

Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad) {
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    *cantidad = 0;
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Primero contar
    archivo.seekg(sizeof(ArchivoHeader));
    Cita temp;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && temp.pacienteID == idPaciente) {
            (*cantidad)++;
        }
    }
    
    if (*cantidad == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Crear array de resultados
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    // Volver al inicio y llenar resultados
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && temp.pacienteID == idPaciente) {
            resultados[index] = new Cita;
            *resultados[index] = temp;
            index++;
        }
    }
    
    archivo.close();
    return resultados;
}

Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad) {
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    *cantidad = 0;
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Primero contar
    archivo.seekg(sizeof(ArchivoHeader));
    Cita temp;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && temp.doctorID == idDoctor) {
            (*cantidad)++;
        }
    }
    
    if (*cantidad == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Crear array de resultados
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    // Volver al inicio y llenar resultados
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && temp.doctorID == idDoctor) {
            resultados[index] = new Cita;
            *resultados[index] = temp;
            index++;
        }
    }
    
    archivo.close();
    return resultados;
}

Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad) {
    if (!validarFecha(fecha)) {
        cout << "Error: Fecha invalida." << endl;
        *cantidad = 0;
        return nullptr;
    }
    
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    *cantidad = 0;
    
    if (!archivo.is_open()) {
        return nullptr;
    }
    
    // Primero contar
    archivo.seekg(sizeof(ArchivoHeader));
    Cita temp;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && strcmp(temp.fecha, fecha) == 0) {
            (*cantidad)++;
        }
    }
    
    if (*cantidad == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Crear array de resultados
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    // Volver al inicio y llenar resultados
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&temp, sizeof(Cita));
        if (!temp.eliminado && strcmp(temp.fecha, fecha) == 0) {
            resultados[index] = new Cita;
            *resultados[index] = temp;
            index++;
        }
    }
    
    archivo.close();
    return resultados;
}

void listarCitasPendientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        return;
    }
    
    cout << "\n+--------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                       CITAS PENDIENTES                                          |" << endl;
    cout << "+------+-----------+-----------+------------+--------+--------------------------------------------+" << endl;
    cout << "|  ID  |  PACIENTE |   DOCTOR  |   FECHA    |  HORA  |                  MOTIVO                    |" << endl;
    cout << "+------+-----------+-----------+------------+--------+--------------------------------------------+" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    Cita cita;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&cita, sizeof(Cita));
        if (!cita.eliminado && !cita.atendida && strcmp(cita.estado, "Cancelada") != 0) {
            // Obtener nombres
            Paciente* p = buscarPacientePorID(hospital, cita.pacienteID);
            Doctor* d = buscarDoctorPorID(hospital, cita.doctorID);
            
            char nombrePaciente[30] = "N/A";
            char nombreDoctor[30] = "N/A";
            
            if (p != nullptr) {
                snprintf(nombrePaciente, 30, "%s. %s", p->nombre[0], p->apellido);
                delete p;
            }
            if (d != nullptr) {
                snprintf(nombreDoctor, 30, "Dr. %s", d->apellido);
                delete d;
            }
            
            cout << "| " << setw(4) << cita.id << " | "
                 << setw(9) << nombrePaciente << " | "
                 << setw(9) << nombreDoctor << " | "
                 << setw(10) << cita.fecha << " | "
                 << setw(6) << cita.hora << " | "
                 << setw(42) << left << cita.motivo << " |" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "|                               NO HAY CITAS PENDIENTES                                       |" << endl;
    }
    
    cout << "+------+-----------+-----------+------------+--------+--------------------------------------------+" << endl;
}

bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora) {
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.seekg(sizeof(ArchivoHeader));
    Cita cita;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&cita, sizeof(Cita));
        if (!cita.eliminado && cita.doctorID == idDoctor && 
            strcmp(cita.fecha, fecha) == 0 && strcmp(cita.hora, hora) == 0 &&
            !cita.atendida && strcmp(cita.estado, "Cancelada") != 0) {
            archivo.close();
            return false;
        }
    }
    
    archivo.close();
    return true;
}

// =============================================================================
// IMPLEMENTACION DE MODULO HISTORIAL MEDICO
// =============================================================================

bool agregarConsultaAlHistorial(Hospital* hospital, HistorialMedico* consulta) {
    // Configurar datos de la consulta
    consulta->id = hospital->siguienteIDConsulta++;
    consulta->eliminado = false;
    time(&consulta->fechaRegistro);
    
    // Abrir archivo y agregar consulta
    ofstream archivo("historiales.bin", ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de historiales." << endl;
        return false;
    }
    
    archivo.write((char*)consulta, sizeof(HistorialMedico));
    archivo.close();
    
    // Actualizar header
    ArchivoHeader header = leerHeader("historiales.bin");
    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID = hospital->siguienteIDConsulta;
    actualizarHeader("historiales.bin", header);
    
    // Actualizar estadisticas del hospital
    hospital->totalConsultasRealizadas++;
    actualizarDatosHospital(hospital);
    
    return true;
}

void mostrarHistorialMedico(Hospital* hospital, int pacienteID) {
    ArchivoHeader header = leerHeader("historiales.bin");
    ifstream archivo("historiales.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de historiales." << endl;
        return;
    }
    
    // Buscar paciente para mostrar su nombre
    Paciente* paciente = buscarPacientePorID(hospital, pacienteID);
    if (paciente == nullptr) {
        cout << "Paciente no encontrado." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "|                                   HISTORIAL MEDICO - " << paciente->nombre << " " << paciente->apellido << setw(40) << "|" << endl;
    cout << "+------------+-------+--------+--------------------------------------------------+------------------+---------------+" << endl;
    cout << "|   FECHA    | HORA  | DOCTOR |                   DIAGNOSTICO                    |    TRATAMIENTO   |    COSTO      |" << endl;
    cout << "+------------+-------+--------+--------------------------------------------------+------------------+---------------+" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    HistorialMedico consulta;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&consulta, sizeof(HistorialMedico));
        if (!consulta.eliminado && consulta.pacienteID == pacienteID) {
            cout << "| " << setw(10) << consulta.fecha << " | "
                 << setw(5) << consulta.hora << " | "
                 << setw(6) << consulta.doctorID << " | "
                 << setw(48) << left << (strlen(consulta.diagnostico) > 45 ? string(consulta.diagnostico).substr(0, 45) + "..." : consulta.diagnostico) << " | "
                 << setw(16) << left << (strlen(consulta.tratamiento) > 13 ? string(consulta.tratamiento).substr(0, 13) + "..." : consulta.tratamiento) << " | $"
                 << setw(11) << fixed << setprecision(2) << consulta.costo << " |" << endl;
            contador++;
        }
    }
    
    archivo.close();
    delete paciente;
    
    if (contador == 0) {
        cout << "|                                    NO HAY CONSULTAS REGISTRADAS                                          |" << endl;
    }
    
    cout << "+------------+-------+--------+--------------------------------------------------+------------------+---------------+" << endl;
}

// =============================================================================
// IMPLEMENTACION DE UTILIDADES
// =============================================================================

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    int anio, mes, dia;
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return false;
    
    if (anio < 1900 || anio > 2025) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    
    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto && dia > 29) return false;
        if (!bisiesto && dia > 28) return false;
    } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return false;
    }
    
    return true;
}

bool validarHora(const char* hora) {
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int hh, mm;
    if (sscanf(hora, "%d:%d", &hh, &mm) != 2) return false;
    
    return (hh >= 0 && hh <= 23) && (mm >= 0 && mm <= 59);
}

bool validarCedula(const char* cedula) {
    return cedula != nullptr && strlen(cedula) > 0 && strlen(cedula) <= 20;
}

bool validarEmail(const char* email) {
    if (email == nullptr) return false;
    
    bool tieneArroba = false;
    bool tienePunto = false;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') tieneArroba = true;
        if (email[i] == '.' && tieneArroba) tienePunto = true;
    }
    
    return tieneArroba && tienePunto;
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// =============================================================================
// IMPLEMENTACION DE INTERFAZ DE USUARIO
// =============================================================================

void mostrarMenuPrincipal() {
    cout << "\n==================================================" << endl;
    cout << "        SISTEMA DE GESTION HOSPITALARIA v2" << endl;
    cout << "           (Persistencia con Archivos)" << endl;
    cout << "==================================================" << endl;
    cout << "1. Gestion de Pacientes" << endl;
    cout << "2. Gestion de Doctores" << endl;
    cout << "3. Gestion de Citas" << endl;
    cout << "4. Mantenimiento de Archivos" << endl;
    cout << "5. Guardar y Salir" << endl;
    cout << "==================================================" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuPacientes() {
    cout << "\n==================================================" << endl;
    cout << "               GESTION DE PACIENTES" << endl;
    cout << "==================================================" << endl;
    cout << "1. Registrar nuevo paciente" << endl;
    cout << "2. Buscar paciente por cedula" << endl;
    cout << "3. Buscar paciente por nombre" << endl;
    cout << "4. Actualizar datos de paciente" << endl;
    cout << "5. Eliminar paciente" << endl;
    cout << "6. Listar todos los pacientes" << endl;
    cout << "7. Ver historial medico" << endl;
    cout << "8. Volver al menu principal" << endl;
    cout << "==================================================" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuDoctores() {
    cout << "\n==================================================" << endl;
    cout << "                GESTION DE DOCTORES" << endl;
    cout << "==================================================" << endl;
    cout << "1. Registrar nuevo doctor" << endl;
    cout << "2. Buscar doctor por ID" << endl;
    cout << "3. Buscar doctores por especialidad" << endl;
    cout << "4. Asignar paciente a doctor" << endl;
    cout << "5. Listar pacientes de doctor" << endl;
    cout << "6. Listar todos los doctores" << endl;
    cout << "7. Eliminar doctor" << endl;
    cout << "8. Volver al menu principal" << endl;
    cout << "==================================================" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuCitas() {
    cout << "\n==================================================" << endl;
    cout << "                 GESTION DE CITAS" << endl;
    cout << "==================================================" << endl;
    cout << "1. Agendar nueva cita" << endl;
    cout << "2. Cancelar cita" << endl;
    cout << "3. Atender cita" << endl;
    cout << "4. Buscar citas por paciente" << endl;
    cout << "5. Buscar citas por doctor" << endl;
    cout << "6. Buscar citas por fecha" << endl;
    cout << "7. Listar citas pendientes" << endl;
    cout << "8. Verificar disponibilidad" << endl;
    cout << "9. Volver al menu principal" << endl;
    cout << "==================================================" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuMantenimiento() {
    cout << "\n==================================================" << endl;
    cout << "             MANTENIMIENTO DE ARCHIVOS" << endl;
    cout << "==================================================" << endl;
    cout << "1. Verificar integridad de archivos" << endl;
    cout << "2. Compactar archivos (eliminar registros borrados)" << endl;
    cout << "3. Hacer respaldo de datos" << endl;
    cout << "4. Restaurar desde respaldo" << endl;
    cout << "5. Estadisticas de uso de archivos" << endl;
    cout << "6. Volver al menu principal" << endl;
    cout << "==================================================" << endl;
    cout << "Seleccione una opcion: ";
}

void ejecutarMenuPacientes(Hospital* hospital) {
    int opcion;
    char input[200];
    
    do {
        mostrarMenuPacientes();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                // Registrar nuevo paciente
                Paciente nuevoPaciente;
                
                cout << "Nombre: ";
                cin.getline(nuevoPaciente.nombre, 50);
                cout << "Apellido: ";
                cin.getline(nuevoPaciente.apellido, 50);
                cout << "Cedula: ";
                cin.getline(nuevoPaciente.cedula, 20);
                cout << "Edad: ";
                cin >> nuevoPaciente.edad;
                limpiarBuffer();
                cout << "Sexo (M/F): ";
                cin >> nuevoPaciente.sexo;
                limpiarBuffer();
                cout << "Tipo de Sangre (O+, O-, A+, A-, B+, B-, AB+, AB-): ";
                cin.getline(nuevoPaciente.tipoSangre, 5);
                cout << "Telefono: ";
                cin.getline(nuevoPaciente.telefono, 15);
                cout << "Direccion: ";
                cin.getline(nuevoPaciente.direccion, 100);
                cout << "Email: ";
                cin.getline(nuevoPaciente.email, 50);
                cout << "Alergias: ";
                cin.getline(nuevoPaciente.alergias, 500);
                cout << "Observaciones: ";
                cin.getline(nuevoPaciente.observaciones, 500);
                
                if (agregarPaciente(hospital, &nuevoPaciente)) {
                    cout << "Paciente registrado exitosamente." << endl;
                }
                break;
            }
            
            case 2: {
                // Buscar por cedula
                cout << "Cedula: ";
                cin.getline(input, 20);
                
                Paciente* paciente = buscarPacientePorCedula(hospital, input);
                if (paciente != nullptr) {
                    cout << "\n*** PACIENTE ENCONTRADO ***" << endl;
                    cout << "ID: " << paciente->id << endl;
                    cout << "Nombre: " << paciente->nombre << " " << paciente->apellido << endl;
                    cout << "Cedula: " << paciente->cedula << endl;
                    cout << "Edad: " << paciente->edad << endl;
                    cout << "Sexo: " << paciente->sexo << endl;
                    cout << "Tipo de Sangre: " << paciente->tipoSangre << endl;
                    cout << "Telefono: " << paciente->telefono << endl;
                    cout << "Direccion: " << paciente->direccion << endl;
                    cout << "Email: " << paciente->email << endl;
                    cout << "Alergias: " << paciente->alergias << endl;
                    cout << "Observaciones: " << paciente->observaciones << endl;
                    delete paciente;
                } else {
                    cout << "Paciente no encontrado." << endl;
                }
                break;
            }
            
            case 3: {
                // Buscar por nombre
                cout << "Nombre: ";
                cin.getline(input, 100);
                
                int cantidad;
                Paciente** resultados = buscarPacientesPorNombre(hospital, input, &cantidad);
                
                if (resultados != nullptr) {
                    cout << "\n*** " << cantidad << " PACIENTES ENCONTRADOS ***" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << "\n--- Paciente " << (i+1) << " ---" << endl;
                        cout << "ID: " << resultados[i]->id << endl;
                        cout << "Nombre: " << resultados[i]->nombre << " " << resultados[i]->apellido << endl;
                        cout << "Cedula: " << resultados[i]->cedula << endl;
                        cout << "Edad: " << resultados[i]->edad << endl;
                        cout << "Telefono: " << resultados[i]->telefono << endl;
                        cout << "Email: " << resultados[i]->email << endl;
                        delete resultados[i];
                    }
                    delete[] resultados;
                } else {
                    cout << "No se encontraron pacientes." << endl;
                }
                break;
            }
            
            case 4: {
                // Actualizar paciente
                int id;
                cout << "ID del paciente a actualizar: ";
                cin >> id;
                limpiarBuffer();
                
                Paciente* paciente = buscarPacientePorID(hospital, id);
                if (paciente != nullptr) {
                    cout << "\nActualizando datos del paciente: " << paciente->nombre << " " << paciente->apellido << endl;
                    
                    cout << "Nueva cedula (" << paciente->cedula << "): ";
                    cin.getline(input, 20);
                    if (strlen(input) > 0) {
                        strcpy(paciente->cedula, input);
                    }
                    
                    cout << "Nuevo telefono (" << paciente->telefono << "): ";
                    cin.getline(input, 15);
                    if (strlen(input) > 0) {
                        strcpy(paciente->telefono, input);
                    }
                    
                    cout << "Nueva direccion (" << paciente->direccion << "): ";
                    cin.getline(input, 100);
                    if (strlen(input) > 0) {
                        strcpy(paciente->direccion, input);
                    }
                    
                    cout << "Nuevo email (" << paciente->email << "): ";
                    cin.getline(input, 50);
                    if (strlen(input) > 0) {
                        if (validarEmail(input)) {
                            strcpy(paciente->email, input);
                        } else {
                            cout << "Email invalido. No se actualizo." << endl;
                        }
                    }
                    
                    cout << "Nuevo tipo de sangre (" << paciente->tipoSangre << "): ";
                    cin.getline(input, 5);
                    if (strlen(input) > 0) {
                        if (validarTipoSangre(input)) {
                            strcpy(paciente->tipoSangre, input);
                        } else {
                            cout << "Tipo de sangre invalido. No se actualizo." << endl;
                        }
                    }
                    
                    if (actualizarPaciente(paciente)) {
                        cout << "Paciente actualizado correctamente." << endl;
                    }
                    delete paciente;
                } else {
                    cout << "Paciente no encontrado." << endl;
                }
                break;
            }
            
            case 5: {
                // Eliminar paciente
                int id;
                cout << "ID del paciente a eliminar: ";
                cin >> id;
                limpiarBuffer();
                
                if (eliminarPaciente(hospital, id)) {
                    cout << "Paciente eliminado exitosamente." << endl;
                }
                break;
            }
            
            case 6: {
                // Listar pacientes
                listarTodosPacientes(hospital);
                break;
            }
            
            case 7: {
                // Ver historial medico
                int id;
                cout << "ID del paciente: ";
                cin >> id;
                limpiarBuffer();
                
                mostrarHistorialMedico(hospital, id);
                break;
            }
            
            case 8: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion invalida." << endl;
            }
        }
        
    } while (opcion != 8);
}

void ejecutarMenuDoctores(Hospital* hospital) {
    int opcion;
    char input[200];
    
    do {
        mostrarMenuDoctores();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                // Registrar nuevo doctor
                Doctor nuevoDoctor;
                
                cout << "Nombre: ";
                cin.getline(nuevoDoctor.nombre, 50);
                cout << "Apellido: ";
                cin.getline(nuevoDoctor.apellido, 50);
                cout << "Cedula profesional: ";
                cin.getline(nuevoDoctor.cedulaProfesional, 20);
                cout << "Especialidad: ";
                cin.getline(nuevoDoctor.especialidad, 50);
                cout << "Anios de experiencia: ";
                cin >> nuevoDoctor.aniosExperiencia;
                cout << "Costo de consulta: ";
                cin >> nuevoDoctor.costoConsulta;
                limpiarBuffer();
                cout << "Horario de atencion: ";
                cin.getline(nuevoDoctor.horarioAtencion, 50);
                cout << "Telefono: ";
                cin.getline(nuevoDoctor.telefono, 15);
                cout << "Email: ";
                cin.getline(nuevoDoctor.email, 50);
                
                if (agregarDoctor(hospital, &nuevoDoctor)) {
                    cout << "Doctor registrado exitosamente." << endl;
                }
                break;
            }
            
            case 2: {
                // Buscar doctor por ID
                int id;
                cout << "ID del doctor: ";
                cin >> id;
                limpiarBuffer();
                
                Doctor* doctor = buscarDoctorPorID(hospital, id);
                if (doctor != nullptr) {
                    cout << "\n*** DOCTOR ENCONTRADO ***" << endl;
                    cout << "ID: " << doctor->id << endl;
                    cout << "Nombre: " << doctor->nombre << " " << doctor->apellido << endl;
                    cout << "Cedula: " << doctor->cedulaProfesional << endl;
                    cout << "Especialidad: " << doctor->especialidad << endl;
                    cout << "Experiencia: " << doctor->aniosExperiencia << " anios" << endl;
                    cout << "Costo: $" << doctor->costoConsulta << endl;
                    cout << "Horario: " << doctor->horarioAtencion << endl;
                    cout << "Telefono: " << doctor->telefono << endl;
                    cout << "Email: " << doctor->email << endl;
                    cout << "Pacientes asignados: " << doctor->cantidadPacientes << endl;
                    delete doctor;
                } else {
                    cout << "Doctor no encontrado." << endl;
                }
                break;
            }
            
            case 3: {
                // Buscar por especialidad
                cout << "Especialidad: ";
                cin.getline(input, 50);
                
                int cantidad;
                Doctor** resultados = buscarDoctoresPorEspecialidad(hospital, input, &cantidad);
                
                if (resultados != nullptr) {
                    cout << "\n*** " << cantidad << " DOCTORES ENCONTRADOS ***" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << "\n--- Doctor " << (i+1) << " ---" << endl;
                        cout << "ID: " << resultados[i]->id << endl;
                        cout << "Nombre: " << resultados[i]->nombre << " " << resultados[i]->apellido << endl;
                        cout << "Especialidad: " << resultados[i]->especialidad << endl;
                        cout << "Experiencia: " << resultados[i]->aniosExperiencia << " anios" << endl;
                        cout << "Costo: $" << resultados[i]->costoConsulta << endl;
                        cout << "Horario: " << resultados[i]->horarioAtencion << endl;
                        cout << "Telefono: " << resultados[i]->telefono << endl;
                        delete resultados[i];
                    }
                    delete[] resultados;
                } else {
                    cout << "No se encontraron doctores con esa especialidad." << endl;
                }
                break;
            }
            
            case 4: {
                // Asignar paciente a doctor
                int idDoctor, idPaciente;
                cout << "ID del doctor: ";
                cin >> idDoctor;
                cout << "ID del paciente: ";
                cin >> idPaciente;
                limpiarBuffer();
                
                if (asignarPacienteADoctor(hospital, idDoctor, idPaciente)) {
                    cout << "Paciente asignado exitosamente." << endl;
                }
                break;
            }
            
            case 5: {
                // Listar pacientes de doctor
                int idDoctor;
                cout << "ID del doctor: ";
                cin >> idDoctor;
                limpiarBuffer();
                
                listarPacientesDeDoctor(hospital, idDoctor);
                break;
            }
            
            case 6: {
                // Listar doctores
                listarTodosDoctores(hospital);
                break;
            }
            
            case 7: {
                // Eliminar doctor
                int id;
                cout << "ID del doctor a eliminar: ";
                cin >> id;
                limpiarBuffer();
                
                if (eliminarDoctor(hospital, id)) {
                    cout << "Doctor eliminado exitosamente." << endl;
                }
                break;
            }
            
            case 8: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion invalida." << endl;
            }
        }
        
    } while (opcion != 8);
}

void ejecutarMenuCitas(Hospital* hospital) {
    int opcion;
    char input[200];
    
    do {
        mostrarMenuCitas();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                // Agendar nueva cita
                Cita nuevaCita;
                
                cout << "ID del paciente: ";
                cin >> nuevaCita.pacienteID;
                cout << "ID del doctor: ";
                cin >> nuevaCita.doctorID;
                limpiarBuffer();
                
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(nuevaCita.fecha, 11);
                cout << "Hora (HH:MM): ";
                cin.getline(nuevaCita.hora, 6);
                cout << "Motivo: ";
                cin.getline(nuevaCita.motivo, 150);
                
                if (agendarCita(hospital, &nuevaCita)) {
                    // Mensaje ya se imprime en agendarCita
                }
                break;
            }
            
            case 2: {
                // Cancelar cita
                int idCita;
                cout << "ID de la cita a cancelar: ";
                cin >> idCita;
                limpiarBuffer();
                
                if (cancelarCita(hospital, idCita)) {
                    // Mensaje ya se imprime en cancelarCita
                }
                break;
            }
            
            case 3: {
                // Atender cita
                int idCita;
                char diagnostico[200], tratamiento[200], medicamentos[150];
                
                cout << "ID de la cita a atender: ";
                cin >> idCita;
                limpiarBuffer();
                
                cout << "Diagnostico: ";
                cin.getline(diagnostico, 200);
                cout << "Tratamiento: ";
                cin.getline(tratamiento, 200);
                cout << "Medicamentos: ";
                cin.getline(medicamentos, 150);
                
                if (atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos)) {
                    // Mensaje ya se imprime en atenderCita
                }
                break;
            }
            
            case 4: {
                // Citas por paciente
                int idPaciente;
                cout << "ID del paciente: ";
                cin >> idPaciente;
                limpiarBuffer();
                
                int cantidad;
                Cita** citas = obtenerCitasDePaciente(hospital, idPaciente, &cantidad);
                
                if (citas != nullptr) {
                    cout << "\n*** " << cantidad << " CITAS ENCONTRADAS ***" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << "\n--- Cita " << (i+1) << " ---" << endl;
                        cout << "ID: " << citas[i]->id << endl;
                        cout << "Fecha: " << citas[i]->fecha << endl;
                        cout << "Hora: " << citas[i]->hora << endl;
                        cout << "Doctor ID: " << citas[i]->doctorID << endl;
                        cout << "Motivo: " << citas[i]->motivo << endl;
                        cout << "Estado: " << citas[i]->estado << endl;
                        cout << "Atendida: " << (citas[i]->atendida ? "Si" : "No") << endl;
                        delete citas[i];
                    }
                    delete[] citas;
                } else {
                    cout << "No se encontraron citas para este paciente." << endl;
                }
                break;
            }
            
            case 5: {
                // Citas por doctor
                int idDoctor;
                cout << "ID del doctor: ";
                cin >> idDoctor;
                limpiarBuffer();
                
                int cantidad;
                Cita** citas = obtenerCitasDeDoctor(hospital, idDoctor, &cantidad);
                
                if (citas != nullptr) {
                    cout << "\n*** " << cantidad << " CITAS ENCONTRADAS ***" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << "\n--- Cita " << (i+1) << " ---" << endl;
                        cout << "ID: " << citas[i]->id << endl;
                        cout << "Fecha: " << citas[i]->fecha << endl;
                        cout << "Hora: " << citas[i]->hora << endl;
                        cout << "Paciente ID: " << citas[i]->pacienteID << endl;
                        cout << "Motivo: " << citas[i]->motivo << endl;
                        cout << "Estado: " << citas[i]->estado << endl;
                        cout << "Atendida: " << (citas[i]->atendida ? "Si" : "No") << endl;
                        delete citas[i];
                    }
                    delete[] citas;
                } else {
                    cout << "No se encontraron citas para este doctor." << endl;
                }
                break;
            }
            
            case 6: {
                // Citas por fecha
                char fecha[11];
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                
                int cantidad;
                Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                
                if (citas != nullptr) {
                    cout << "\n*** " << cantidad << " CITAS ENCONTRADAS ***" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << "\n--- Cita " << (i+1) << " ---" << endl;
                        cout << "ID: " << citas[i]->id << endl;
                        cout << "Hora: " << citas[i]->hora << endl;
                        cout << "Paciente ID: " << citas[i]->pacienteID << endl;
                        cout << "Doctor ID: " << citas[i]->doctorID << endl;
                        cout << "Motivo: " << citas[i]->motivo << endl;
                        cout << "Estado: " << citas[i]->estado << endl;
                        delete citas[i];
                    }
                    delete[] citas;
                } else {
                    cout << "No se encontraron citas para esta fecha." << endl;
                }
                break;
            }
            
            case 7: {
                // Listar citas pendientes
                listarCitasPendientes(hospital);
                break;
            }
            
            case 8: {
                // Verificar disponibilidad
                int idDoctor;
                char fecha[11], hora[6];
                
                cout << "ID del doctor: ";
                cin >> idDoctor;
                limpiarBuffer();
                
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                cout << "Hora (HH:MM): ";
                cin.getline(hora, 6);
                
                if (verificarDisponibilidad(hospital, idDoctor, fecha, hora)) {
                    cout << "El doctor esta disponible en esa fecha y hora." << endl;
                } else {
                    cout << "El doctor no esta disponible en esa fecha y hora." << endl;
                }
                break;
            }
            
            case 9: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion invalida." << endl;
            }
        }
        
    } while (opcion != 9);
}

void ejecutarMenuMantenimiento(Hospital* hospital) {
    int opcion;
    
    do {
        mostrarMenuMantenimiento();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                // Verificar integridad de archivos
                const char* archivos[] = {
                    "hospital.bin", "pacientes.bin", "doctores.bin", 
                    "citas.bin", "historiales.bin"
                };
                
                cout << "\n=== VERIFICACION DE INTEGRIDAD DE ARCHIVOS ===" << endl;
                for (int i = 0; i < 5; i++) {
                    if (verificarArchivo(archivos[i])) {
                        ArchivoHeader header = leerHeader(archivos[i]);
                        cout << "✓ " << archivos[i] << " - " << header.registrosActivos 
                             << "/" << header.cantidadRegistros << " registros activos" << endl;
                    } else {
                        cout << "✗ " << archivos[i] << " - CORRUPTO O NO EXISTE" << endl;
                    }
                }
                break;
            }
            
            case 5: {
                // Estadisticas de uso
                cout << "\n=== ESTADISTICAS DEL SISTEMA ===" << endl;
                cout << "Total pacientes registrados: " << hospital->totalPacientesRegistrados << endl;
                cout << "Total doctores registrados: " << hospital->totalDoctoresRegistrados << endl;
                cout << "Total citas agendadas: " << hospital->totalCitasAgendadas << endl;
                cout << "Total consultas realizadas: " << hospital->totalConsultasRealizadas << endl;
                
                ArchivoHeader headerPac = leerHeader("pacientes.bin");
                ArchivoHeader headerDoc = leerHeader("doctores.bin");
                ArchivoHeader headerCitas = leerHeader("citas.bin");
                ArchivoHeader headerHist = leerHeader("historiales.bin");
                
                cout << "\n=== ESTADISTICAS DE ARCHIVOS ===" << endl;
                cout << "Pacientes: " << headerPac.registrosActivos << " activos de " << headerPac.cantidadRegistros << endl;
                cout << "Doctores: " << headerDoc.registrosActivos << " activos de " << headerDoc.cantidadRegistros << endl;
                cout << "Citas: " << headerCitas.registrosActivos << " activos de " << headerCitas.cantidadRegistros << endl;
                cout << "Historiales: " << headerHist.registrosActivos << " activos de " << headerHist.cantidadRegistros << endl;
                break;
            }
            
            case 6: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion no implementada en esta version." << endl;
            }
        }
        
    } while (opcion != 6);
}

// =============================================================================
// FUNCION PRINCIPAL
// =============================================================================

int main() {
    cout << "Inicializando Sistema de Gestion Hospitalaria v2..." << endl;
    cout << "(Sistema de persistencia con archivos binarios)" << endl;
    
    // Cargar datos del hospital
    Hospital* hospital = cargarDatosHospital();
    if (hospital == nullptr) {
        cout << "Error: No se pudo inicializar el sistema." << endl;
        return 1;
    }
    
    cout << "Hospital: " << hospital->nombre << endl;
    cout << "Sistema cargado correctamente." << endl;
    
    // Menu principal
    int opcion;
    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1:
                ejecutarMenuPacientes(hospital);
                break;
            case 2:
                ejecutarMenuDoctores(hospital);
                break;
            case 3:
                ejecutarMenuCitas(hospital);
                break;
            case 4:
                ejecutarMenuMantenimiento(hospital);
                break;
            case 5:
                cout << "Guardando datos y saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
        
    } while (opcion != 5);
    
    // Guardar datos y liberar memoria
    guardarDatosHospital(hospital);
    delete hospital;
    
    cout << "Sistema cerrado correctamente. ¡Hasta pronto!" << endl;
    return 0;
}
