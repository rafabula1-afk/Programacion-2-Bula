#include "Doctor.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>

using namespace std;

//Constructores
Doctor::Doctor() {
    id = 0;
    nombre[0] = '\0';
    apellido[0] = '\0';
    cedulaProfesional[0] = '\0';
    especialidad[0] = '\0';
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    horarioAtencion[0] = '\0';
    telefono[0] = '\0';
    email[0] = '\0';
    disponible = true;
    cantidadPacientes = 0;
    for (int i = 0; i < 50; i++) pacientesIDs[i] = -1;
    cantidadCitas = 0;
    for (int i = 0; i < 30; i++) citasIDs[i] = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Doctor::Doctor(int id, const char* nombre, const char* apellido, const char* cedulaProfesional, const char* especialidad) {
    this->id = id;
    strncpy(this->nombre, nombre, 49);
    this->nombre[49] = '\0';
    strncpy(this->apellido, apellido, 49);
    this->apellido[49] = '\0';
    strncpy(this->cedulaProfesional, cedulaProfesional, 19);
    this->cedulaProfesional[19] = '\0';
    strncpy(this->especialidad, especialidad, 49);
    this->especialidad[49] = '\0';
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    horarioAtencion[0] = '\0';
    telefono[0] = '\0';
    email[0] = '\0';
    disponible = true;
    cantidadPacientes = 0;
    for (int i = 0; i < 50; i++) pacientesIDs[i] = -1;
    cantidadCitas = 0;
    for (int i = 0; i < 30; i++) citasIDs[i] = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Doctor::Doctor(const Doctor& otro) {
    id = otro.id;
    strcpy(nombre, otro.nombre);
    strcpy(apellido, otro.apellido);
    strcpy(cedulaProfesional, otro.cedulaProfesional);
    strcpy(especialidad, otro.especialidad);
    aniosExperiencia = otro.aniosExperiencia;
    costoConsulta = otro.costoConsulta;
    strcpy(horarioAtencion, otro.horarioAtencion);
    strcpy(telefono, otro.telefono);
    strcpy(email, otro.email);
    disponible = otro.disponible;
    cantidadPacientes = otro.cantidadPacientes;
    for (int i = 0; i < 50; i++) pacientesIDs[i] = otro.pacientesIDs[i];
    cantidadCitas = otro.cantidadCitas;
    for (int i = 0; i < 30; i++) citasIDs[i] = otro.citasIDs[i];
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaModificacion = otro.fechaModificacion;
}

//Destructor
Doctor::~Doctor() {}

//Getters
int Doctor::getId() const { return id; }
const char* Doctor::getNombre() const { return nombre; }
const char* Doctor::getApellido() const { return apellido; }
const char* Doctor::getCedulaProfesional() const { return cedulaProfesional; }
const char* Doctor::getEspecialidad() const { return especialidad; }
int Doctor::getAniosExperiencia() const { return aniosExperiencia; }
float Doctor::getCostoConsulta() const { return costoConsulta; }
const char* Doctor::getHorarioAtencion() const { return horarioAtencion; }
const char* Doctor::getTelefono() const { return telefono; }
const char* Doctor::getEmail() const { return email; }
bool Doctor::isDisponible() const { return disponible; }
int Doctor::getCantidadPacientes() const { return cantidadPacientes; }
const int* Doctor::getPacientesIDs() const { return pacientesIDs; }
int Doctor::getCantidadCitas() const { return cantidadCitas; }
const int* Doctor::getCitasIDs() const { return citasIDs; }
bool Doctor::isEliminado() const { return eliminado; }
time_t Doctor::getFechaCreacion() const { return fechaCreacion; }
time_t Doctor::getFechaModificacion() const { return fechaModificacion; }

//Setters
void Doctor::setId(int id) { this->id = id; }

void Doctor::setNombre(const char* nombre) {
    strncpy(this->nombre, nombre, 49);
    this->nombre[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Doctor::setApellido(const char* apellido) {
    strncpy(this->apellido, apellido, 49);
    this->apellido[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Doctor::setCedulaProfesional(const char* cedula) {
    strncpy(this->cedulaProfesional, cedula, 19);
    this->cedulaProfesional[19] = '\0';
    fechaModificacion = time(nullptr);
}

void Doctor::setEspecialidad(const char* especialidad) {
    strncpy(this->especialidad, especialidad, 49);
    this->especialidad[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Doctor::setAniosExperiencia(int anios) {
    if (anios >= 0 && anios <= 80) {
        this->aniosExperiencia = anios;
        fechaModificacion = time(nullptr);
    }
}

void Doctor::setCostoConsulta(float costo) {
    if (costo >= 0.0f && costo <= 10000.0f) {
        this->costoConsulta = costo;
        fechaModificacion = time(nullptr);
    }
}

void Doctor::setHorarioAtencion(const char* horario) {
    strncpy(this->horarioAtencion, horario, 49);
    this->horarioAtencion[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Doctor::setTelefono(const char* telefono) {
    bool valido = true;
    for (int i = 0; telefono[i] != '\0'; i++) {
        if (!isdigit(telefono[i]) && telefono[i] != ' ' && 
            telefono[i] != '-' && telefono[i] != '(' && telefono[i] != ')') {
            valido = false;
            break;
        }
    }
    
    if (valido && strlen(telefono) <= 15) {
        strncpy(this->telefono, telefono, 14);
        this->telefono[14] = '\0';
        fechaModificacion = time(nullptr);
    }
}

void Doctor::setEmail(const char* email) {
    bool tieneArroba = false, tienePunto = false;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') tieneArroba = true;
        if (email[i] == '.' && tieneArroba) tienePunto = true;
    }
    
    if (tieneArroba && tienePunto && strlen(email) <= 50) {
        strncpy(this->email, email, 49);
        this->email[49] = '\0';
        fechaModificacion = time(nullptr);
    }
}

void Doctor::setDisponible(bool disponible) {
    this->disponible = disponible;
    fechaModificacion = time(nullptr);
}

void Doctor::setCantidadPacientes(int cantidad) {
    this->cantidadPacientes = cantidad;
    fechaModificacion = time(nullptr);
}

void Doctor::setCantidadCitas(int cantidad) {
    this->cantidadCitas = cantidad;
    fechaModificacion = time(nullptr);
}

void Doctor::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    this->disponible = !eliminado;
    fechaModificacion = time(nullptr);
}

void Doctor::setFechaCreacion(time_t fecha) {
    this->fechaCreacion = fecha;
}

void Doctor::setFechaModificacion(time_t fecha) {
    this->fechaModificacion = fecha;
}

//Metodos de Validacion
bool Doctor::validarDatos() const {
    return strlen(nombre) > 0 && strlen(apellido) > 0 && 
           strlen(cedulaProfesional) > 0 && strlen(especialidad) > 0 &&
           aniosExperiencia >= 0 && costoConsulta >= 0.0f &&
           strlen(horarioAtencion) > 0;
}

bool Doctor::especialidadEsValida() const {
    const char* especialidadesValidas[] = {
        "Cardiologia", "Pediatria", "Neurologia", "Dermatologia",
        "Oftalmologia", "Traumatologia", "Ginecologia", "Urologia",
        "Psiquiatria", "Oncologia", "Endocrinologia", "Nefrologia"
    };
    
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(especialidad, especialidadesValidas[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool Doctor::cedulaProfesionalValida() const {
    return strlen(cedulaProfesional) >= 5 && strlen(cedulaProfesional) <= 20;
}

bool Doctor::costoConsultaValido() const {
    return costoConsulta >= 0.0f && costoConsulta <= 10000.0f;
}

//Metodos de Gestion
bool Doctor::agregarPacienteID(int pacienteID) {
    if (cantidadPacientes >= 50) return false;
    
    for (int i = 0; i < cantidadPacientes; i++) {
        if (pacientesIDs[i] == pacienteID) return false;
    }
    
    pacientesIDs[cantidadPacientes] = pacienteID;
    cantidadPacientes++;
    fechaModificacion = time(nullptr);
    return true;
}

bool Doctor::eliminarPacienteID(int pacienteID) {
    for (int i = 0; i < cantidadPacientes; i++) {
        if (pacientesIDs[i] == pacienteID) {
            for (int j = i; j < cantidadPacientes - 1; j++) {
                pacientesIDs[j] = pacientesIDs[j + 1];
            }
            cantidadPacientes--;
            pacientesIDs[cantidadPacientes] = -1;
            fechaModificacion = time(nullptr);
            return true;
        }
    }
    return false;
}

bool Doctor::agregarCitaID(int citaID) {
    if (cantidadCitas >= 30) return false;

    for (int i = 0; i < cantidadCitas; i++) {
        if (citasIDs[i] == citaID) return false;
    }
    
    citasIDs[cantidadCitas] = citaID;
    cantidadCitas++;
    fechaModificacion = time(nullptr);
    return true;
}

bool Doctor::eliminarCitaID(int citaID) {
    for (int i = 0; i < cantidadCitas; i++) {
        if (citasIDs[i] == citaID) {
            for (int j = i; j < cantidadCitas - 1; j++) {
                citasIDs[j] = citasIDs[j + 1];
            }
            cantidadCitas--;
            citasIDs[cantidadCitas] = -1;
            fechaModificacion = time(nullptr);
            return true;
        }
    }
    return false;
}

bool Doctor::estaDisponible() const {
    return disponible && !eliminado;
}

void Doctor::setDisponible(bool disponible) {
    this->disponible = disponible;
    fechaModificacion = time(nullptr);
}

bool Doctor::tienePacientes() const {
    return cantidadPacientes > 0;
}

bool Doctor::tieneCitas() const {
    return cantidadCitas > 0;
}

// ============ MÉTODOS DE PRESENTACIÓN ============
void Doctor::mostrarInformacionBasica() const {
    cout << "\n=== INFORMACION BÁSICA DEL DOCTOR ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Cedula Profesional: " << cedulaProfesional << endl;
    cout << "Especialidad: " << especialidad << endl;
    cout << "Anios de experiencia: " << aniosExperiencia << endl;
    cout << "Costo consulta: $" << costoConsulta << endl;
    cout << "Disponible: " << (disponible ? "Si" : "No") << endl;
}

void Doctor::mostrarInformacionCompleta() const {
    cout << "\n=== INFORMACION COMPLETA DEL DOCTOR ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Cedula Profesional: " << cedulaProfesional << endl;
    cout << "Especialidad: " << especialidad << endl;
    cout << "Años de experiencia: " << aniosExperiencia << endl;
    cout << "Costo consulta: $" << costoConsulta << endl;
    cout << "Horario atencion: " << horarioAtencion << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Email: " << email << endl;
    cout << "Disponible: " << (disponible ? "Si" : "No") << endl;
    cout << "Pacientes asignados: " << cantidadPacientes << endl;
    cout << "Citas programadas: " << cantidadCitas << endl;
    char buffer[26];
    ctime_r(&fechaCreacion, buffer);
    cout << "Fecha de creacion: " << buffer;
    ctime_r(&fechaModificacion, buffer);
    cout << "Última modificacion: " << buffer;
}

//Operadores
Doctor& Doctor::operator=(const Doctor& otro) {
    if (this != &otro) {
        id = otro.id;
        strcpy(nombre, otro.nombre);
        strcpy(apellido, otro.apellido);
        strcpy(cedulaProfesional, otro.cedulaProfesional);
        strcpy(especialidad, otro.especialidad);
        aniosExperiencia = otro.aniosExperiencia;
        costoConsulta = otro.costoConsulta;
        strcpy(horarioAtencion, otro.horarioAtencion);
        strcpy(telefono, otro.telefono);
        strcpy(email, otro.email);
        disponible = otro.disponible;
        cantidadPacientes = otro.cantidadPacientes;
        for (int i = 0; i < 50; i++) pacientesIDs[i] = otro.pacientesIDs[i];
        cantidadCitas = otro.cantidadCitas;
        for (int i = 0; i < 30; i++) citasIDs[i] = otro.citasIDs[i];
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaModificacion = otro.fechaModificacion;
    }
    return *this;
}

//Metodo Estatico
size_t Doctor::obtenerTamanio() {
    return sizeof(Doctor);
}