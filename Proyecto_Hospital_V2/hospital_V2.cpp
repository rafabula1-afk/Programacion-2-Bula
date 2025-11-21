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
long calcularPosicion(int indice, size_t tamañoRegistro);

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
bool eliminarPaciente(int id);
void listarTodosPacientes(Hospital* hospital);
Paciente leerPacientePorIndice(int indice);

// MODULO DE DOCTORES
bool agregarDoctor(Hospital* hospital, Doctor* doctor);
Doctor* buscarDoctorPorID(Hospital* hospital, int id);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad);
bool actualizarDoctor(Doctor* doctor);
bool eliminarDoctor(int id);
void listarTodosDoctores(Hospital* hospital);

// MODULO DE CITAS
bool agendarCita(Hospital* hospital, Cita* cita);
bool cancelarCita(int id);
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos);
Cita** obtenerCitasDePaciente(int idPaciente, int* cantidad);
Cita** obtenerCitasDeDoctor(int idDoctor, int* cantidad);
Cita** obtenerCitasPorFecha(const char* fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);
bool verificarDisponibilidad(int idDoctor, const char* fecha, const char* hora);

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

long calcularPosicion(int indice, size_t tamañoRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamañoRegistro);
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
// IMPLEMENTACION DE MODULO PACIENTES
// =============================================================================

bool agregarPaciente(Hospital* hospital, Paciente* paciente) {
    // Verificar cedula unica
    if (buscarPacientePorCedula(hospital, paciente->cedula) != nullptr) {
        cout << "Error: Ya existe un paciente con esta cedula." << endl;
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
    
    cout << "Paciente actualizado correctamente." << endl;
    return true;
}

bool eliminarPaciente(int id) {
    Paciente* paciente = nullptr;
    Hospital hospitalTemp;
    // Necesitariamos cargar el hospital para buscar, pero por simplicidad
    // asumimos que tenemos acceso al hospital
    
    // En una implementacion completa, buscariamos el paciente y marcariamos como eliminado
    cout << "Funcion eliminarPaciente requiere implementacion completa." << endl;
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
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CEDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
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

void listarTodosDoctores(Hospital* hospital) {
    ArchivoHeader header = leerHeader("doctores.bin");
    ifstream archivo("doctores.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de doctores." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                            LISTA DE DOCTORES                                ¦" << endl;
    cout << "¦-----------------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ ESPECIALIDAD         ¦ EXP. ¦ COSTO    ¦ PAC.   ¦" << endl;
    cout << "¦-----+---------------------+----------------------+------+----------+--------¦" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    Doctor doctor;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&doctor, sizeof(Doctor));
        if (!doctor.eliminado) {
            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", doctor.nombre, doctor.apellido);
            
            cout << "¦ " << setw(3) << doctor.id << " ¦ "
                 << setw(19) << left << nombreCompleto << " ¦ "
                 << setw(20) << doctor.especialidad << " ¦ "
                 << setw(4) << doctor.aniosExperiencia << " ¦ $"
                 << setw(7) << fixed << setprecision(2) << doctor.costoConsulta << " ¦ "
                 << setw(6) << doctor.cantidadPacientes << "¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "¦                NO HAY DOCTORES REGISTRADOS                 ¦" << endl;
    }
    
    cout << "+-----------------------------------------------------------------------------+" << endl;
    cout << "Total de doctores activos: " << contador << endl;
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
    return true;
}

void listarCitasPendientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("citas.bin");
    ifstream archivo("citas.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        return;
    }
    
    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                            CITAS PENDIENTES                                 ¦" << endl;
    cout << "¦-----------------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ PAC.  ¦ DOC.  ¦ FECHA    ¦ HORA   ¦ MOTIVO                           ¦" << endl;
    cout << "¦-----+-------+-------+----------+--------+----------------------------------¦" << endl;
    
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
                snprintf(nombrePaciente, 30, "%s", p->nombre);
                delete p;
            }
            if (d != nullptr) {
                snprintf(nombreDoctor, 30, "%s", d->nombre);
                delete d;
            }
            
            cout << "¦ " << setw(3) << cita.id << " ¦ "
                 << setw(5) << nombrePaciente << " ¦ "
                 << setw(5) << nombreDoctor << " ¦ "
                 << setw(8) << cita.fecha << " ¦ "
                 << setw(6) << cita.hora << " ¦ "
                 << setw(32) << left << cita.motivo << "¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "¦                      NO HAY CITAS PENDIENTES                           ¦" << endl;
    }
    
    cout << "+-----------------------------------------------------------------------------+" << endl;
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
    
    cout << "\n+----------------------------------------------------------------------------------+" << endl;
    cout << "¦                         HISTORIAL MEDICO - " << paciente->nombre << " " << paciente->apellido << setw(25) << "¦" << endl;
    cout << "¦---------------------------------------------------------------------------------¦" << endl;
    cout << "¦ FECHA     ¦ HORA  ¦ DOCTOR ¦ DIAGNOSTICO                                      ¦" << endl;
    cout << "¦-----------+-------+--------+--------------------------------------------------¦" << endl;
    
    archivo.seekg(sizeof(ArchivoHeader));
    HistorialMedico consulta;
    int contador = 0;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read((char*)&consulta, sizeof(HistorialMedico));
        if (!consulta.eliminado && consulta.pacienteID == pacienteID) {
            cout << "¦ " << setw(9) << consulta.fecha << " ¦ "
                 << setw(5) << consulta.hora << " ¦ "
                 << setw(6) << consulta.doctorID << " ¦ "
                 << setw(48) << left << consulta.diagnostico << "¦" << endl;
            contador++;
        }
    }
    
    archivo.close();
    delete paciente;
    
    if (contador == 0) {
        cout << "¦               NO HAY CONSULTAS REGISTRADAS               ¦" << endl;
    }
    
    cout << "+---------------------------------------------------------------------------------+" << endl;
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
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   SISTEMA DE GESTION HOSPITALARIA v2   ║" << endl;
    cout << "║         (Persistencia con Archivos)    ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "1. Gestion de Pacientes" << endl;
    cout << "2. Gestion de Doctores" << endl;
    cout << "3. Gestion de Citas" << endl;
    cout << "4. Consultas y Reportes" << endl;
    cout << "5. Mantenimiento de Archivos" << endl;
    cout << "6. Guardar y Salir" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuPacientes() {
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                   GESTION DE PACIENTES                    ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Registrar nuevo paciente                               ¦" << endl;
    cout << "¦ 2. Buscar paciente por cedula                             ¦" << endl;
    cout << "¦ 3. Buscar paciente por nombre                             ¦" << endl;
    cout << "¦ 4. Actualizar datos de paciente                           ¦" << endl;
    cout << "¦ 5. Eliminar paciente                                      ¦" << endl;
    cout << "¦ 6. Listar todos los pacientes                             ¦" << endl;
    cout << "¦ 7. Ver historial medico                                   ¦" << endl;
    cout << "¦ 8. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuDoctores() {
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    GESTION DE DOCTORES                    ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Registrar nuevo doctor                                 ¦" << endl;
    cout << "¦ 2. Buscar doctor por ID                                   ¦" << endl;
    cout << "¦ 3. Buscar doctores por especialidad                       ¦" << endl;
    cout << "¦ 4. Asignar paciente a doctor                              ¦" << endl;
    cout << "¦ 5. Listar pacientes de doctor                             ¦" << endl;
    cout << "¦ 6. Listar todos los doctores                              ¦" << endl;
    cout << "¦ 7. Eliminar doctor                                        ¦" << endl;
    cout << "¦ 8. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuCitas() {
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                     GESTION DE CITAS                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Agendar nueva cita                                     ¦" << endl;
    cout << "¦ 2. Cancelar cita                                          ¦" << endl;
    cout << "¦ 3. Atender cita                                           ¦" << endl;
    cout << "¦ 4. Buscar citas por paciente                              ¦" << endl;
    cout << "¦ 5. Buscar citas por doctor                                ¦" << endl;
    cout << "¦ 6. Buscar citas por fecha                                 ¦" << endl;
    cout << "¦ 7. Listar citas pendientes                                ¦" << endl;
    cout << "¦ 8. Verificar disponibilidad                               ¦" << endl;
    cout << "¦ 9. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";
}

void mostrarMenuMantenimiento() {
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                 MANTENIMIENTO DE ARCHIVOS                 ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Verificar integridad de archivos                       ¦" << endl;
    cout << "¦ 2. Compactar archivos (eliminar registros borrados)       ¦" << endl;
    cout << "¦ 3. Hacer respaldo de datos                                ¦" << endl;
    cout << "¦ 4. Restaurar desde respaldo                               ¦" << endl;
    cout << "¦ 5. Estadisticas de uso de archivos                        ¦" << endl;
    cout << "¦ 6. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
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
                cout << "Sexo (M/F): ";
                cin >> nuevoPaciente.sexo;
                limpiarBuffer();
                cout << "Telefono: ";
                cin.getline(nuevoPaciente.telefono, 15);
                cout << "Direccion: ";
                cin.getline(nuevoPaciente.direccion, 100);
                cout << "Email: ";
                cin.getline(nuevoPaciente.email, 50);
                cout << "Tipo de Sangre: ";
                cin.getline(nuevoPaciente.tipoSangre, 5);
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
                    cout << "\nPaciente encontrado:" << endl;
                    cout << "ID: " << paciente->id << endl;
                    cout << "Nombre: " << paciente->nombre << " " << paciente->apellido << endl;
                    cout << "Edad: " << paciente->edad << endl;
                    cout << "Telefono: " << paciente->telefono << endl;
                    cout << "Email: " << paciente->email << endl;
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
                    cout << "Se encontraron " << cantidad << " pacientes:" << endl;
                    for (int i = 0; i < cantidad; i++) {
                        cout << i+1 << ". " << resultados[i]->nombre << " " << resultados[i]->apellido 
                             << " (Cedula: " << resultados[i]->cedula << ")" << endl;
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
                    if (strlen(input) > 0 && validarEmail(input)) {
                        strcpy(paciente->email, input);
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
                // Eliminar paciente (borrado logico)
                int id;
                cout << "ID del paciente a eliminar: ";
                cin >> id;
                limpiarBuffer();
                
                Paciente* paciente = buscarPacientePorID(hospital, id);
                if (paciente != nullptr) {
                    paciente->eliminado = true;
                    paciente->activo = false;
                    if (actualizarPaciente(paciente)) {
                        cout << "Paciente eliminado correctamente." << endl;
                        
                        // Actualizar header
                        ArchivoHeader header = leerHeader("pacientes.bin");
                        header.registrosActivos--;
                        actualizarHeader("pacientes.bin", header);
                    }
                    delete paciente;
                } else {
                    cout << "Paciente no encontrado." << endl;
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
                    cout << "\nDoctor encontrado:" << endl;
                    cout << "Nombre: " << doctor->nombre << " " << doctor->apellido << endl;
                    cout << "Especialidad: " << doctor->especialidad << endl;
                    cout << "Experiencia: " << doctor->aniosExperiencia << " anios" << endl;
                    cout << "Costo: $" << doctor->costoConsulta << endl;
                    cout << "Telefono: " << doctor->telefono << endl;
                    delete doctor;
                } else {
                    cout << "Doctor no encontrado." << endl;
                }
                break;
            }
            
            case 6: {
                // Listar doctores
                listarTodosDoctores(hospital);
                break;
            }
            
            case 8: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion no implementada en esta version." << endl;
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
                    cout << "Cita agendada exitosamente." << endl;
                }
                break;
            }
            
            case 7: {
                // Listar citas pendientes
                listarCitasPendientes(hospital);
                break;
            }
            
            case 9: {
                cout << "Volviendo al menu principal..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion no implementada en esta version." << endl;
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
                
                cout << "\nVerificando integridad de archivos..." << endl;
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
                cout << "\nESTADISTICAS DEL SISTEMA:" << endl;
                cout << "Total pacientes registrados: " << hospital->totalPacientesRegistrados << endl;
                cout << "Total doctores registrados: " << hospital->totalDoctoresRegistrados << endl;
                cout << "Total citas agendadas: " << hospital->totalCitasAgendadas << endl;
                cout << "Total consultas realizadas: " << hospital->totalConsultasRealizadas << endl;
                
                ArchivoHeader headerPac = leerHeader("pacientes.bin");
                ArchivoHeader headerDoc = leerHeader("doctores.bin");
                ArchivoHeader headerCitas = leerHeader("citas.bin");
                ArchivoHeader headerHist = leerHeader("historiales.bin");
                
                cout << "\nESTADISTICAS DE ARCHIVOS:" << endl;
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
                cout << "Modulo de consultas y reportes en desarrollo." << endl;
                break;
            case 5:
                ejecutarMenuMantenimiento(hospital);
                break;
            case 6:
                cout << "Guardando datos y saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
        
    } while (opcion != 6);
    
    // Guardar datos y liberar memoria
    guardarDatosHospital(hospital);
    delete hospital;
    
    cout << "Sistema cerrado correctamente. ¡Hasta pronto!" << endl;
    return 0;
}