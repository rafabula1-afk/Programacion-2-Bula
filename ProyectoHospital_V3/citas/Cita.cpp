#include "Cita.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>

using namespace std;

// ============ CONSTRUCTORES ============
Cita::Cita() {
    id = 0;
    pacienteID = -1;
    doctorID = -1;
    fecha[0] = '\0';
    hora[0] = '\0';
    motivo[0] = '\0';
    strcpy(estado, "Pendiente");
    observaciones[0] = '\0';
    atendida = false;
    consultaID = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Cita::Cita(int id, int pacienteID, int doctorID, const char* fecha, const char* hora, const char* motivo) {
    this->id = id;
    this->pacienteID = pacienteID;
    this->doctorID = doctorID;
    strncpy(this->fecha, fecha, 10);
    this->fecha[10] = '\0';
    strncpy(this->hora, hora, 5);
    this->hora[5] = '\0';
    strncpy(this->motivo, motivo, 149);
    this->motivo[149] = '\0';
    strcpy(this->estado, "Agendada");
    observaciones[0] = '\0';
    atendida = false;
    consultaID = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Cita::Cita(const Cita& otro) {
    id = otro.id;
    pacienteID = otro.pacienteID;
    doctorID = otro.doctorID;
    strcpy(fecha, otro.fecha);
    strcpy(hora, otro.hora);
    strcpy(motivo, otro.motivo);
    strcpy(estado, otro.estado);
    strcpy(observaciones, otro.observaciones);
    atendida = otro.atendida;
    consultaID = otro.consultaID;
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaModificacion = otro.fechaModificacion;
}

// ============ DESTRUCTOR ============
Cita::~Cita() {
    // No hay memoria dinámica que liberar
}

// ============ GETTERS ============
int Cita::getId() const { return id; }
int Cita::getPacienteID() const { return pacienteID; }
int Cita::getDoctorID() const { return doctorID; }
const char* Cita::getFecha() const { return fecha; }
const char* Cita::getHora() const { return hora; }
const char* Cita::getMotivo() const { return motivo; }
const char* Cita::getEstado() const { return estado; }
const char* Cita::getObservaciones() const { return observaciones; }
bool Cita::isAtendida() const { return atendida; }
int Cita::getConsultaID() const { return consultaID; }
bool Cita::isEliminado() const { return eliminado; }
time_t Cita::getFechaCreacion() const { return fechaCreacion; }
time_t Cita::getFechaModificacion() const { return fechaModificacion; }

// ============ SETTERS ============
void Cita::setId(int id) { this->id = id; }

void Cita::setPacienteID(int pacienteID) {
    this->pacienteID = pacienteID;
    fechaModificacion = time(nullptr);
}

void Cita::setDoctorID(int doctorID) {
    this->doctorID = doctorID;
    fechaModificacion = time(nullptr);
}

void Cita::setFecha(const char* fecha) {
    if (strlen(fecha) == 10 && fecha[4] == '-' && fecha[7] == '-') {
        strncpy(this->fecha, fecha, 10);
        this->fecha[10] = '\0';
        fechaModificacion = time(nullptr);
    }
}

void Cita::setHora(const char* hora) {
    if (strlen(hora) == 5 && hora[2] == ':') {
        strncpy(this->hora, hora, 5);
        this->hora[5] = '\0';
        fechaModificacion = time(nullptr);
    }
}

void Cita::setMotivo(const char* motivo) {
    strncpy(this->motivo, motivo, 149);
    this->motivo[149] = '\0';
    fechaModificacion = time(nullptr);
}

void Cita::setEstado(const char* estado) {
    strncpy(this->estado, estado, 19);
    this->estado[19] = '\0';
    fechaModificacion = time(nullptr);
}

void Cita::setObservaciones(const char* observaciones) {
    strncpy(this->observaciones, observaciones, 199);
    this->observaciones[199] = '\0';
    fechaModificacion = time(nullptr);
}

void Cita::setAtendida(bool atendida) {
    this->atendida = atendida;
    if (atendida) {
        strcpy(this->estado, "Atendida");
    }
    fechaModificacion = time(nullptr);
}

void Cita::setConsultaID(int consultaID) {
    this->consultaID = consultaID;
    fechaModificacion = time(nullptr);
}

void Cita::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    if (eliminado) {
        strcpy(this->estado, "Cancelada");
    }
    fechaModificacion = time(nullptr);
}

void Cita::setFechaCreacion(time_t fecha) {
    this->fechaCreacion = fecha;
}

void Cita::setFechaModificacion(time_t fecha) {
    this->fechaModificacion = fecha;
}

// ============ MÉTODOS DE VALIDACIÓN ============
bool Cita::validarDatos() const {
    return pacienteID > 0 && doctorID > 0 && 
           strlen(fecha) == 10 && strlen(hora) == 5 &&
           strlen(motivo) > 0;
}

bool Cita::fechaHoraValidas() const {
    // Validar formato fecha: YYYY-MM-DD
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    // Validar formato hora: HH:MM
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    // Extraer componentes
    int anio, mes, dia;
    int horaInt, minuto;
    
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return false;
    if (sscanf(hora, "%d:%d", &horaInt, &minuto) != 2) return false;
    
    // Validar rangos
    if (anio < 2023 || anio > 2030) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (horaInt < 0 || horaInt > 23) return false;
    if (minuto < 0 || minuto > 59) return false;
    
    // Validar días por mes
    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto && dia > 29) return false;
        if (!bisiesto && dia > 28) return false;
    } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return false;
    }
    
    return true;
}

bool Cita::pacienteYDoctorExisten() const {
    // Esta validación se hará en el contexto del sistema
    return pacienteID > 0 && doctorID > 0;
}

// ============ MÉTODOS DE GESTIÓN ============
void Cita::marcarComoAtendida() {
    atendida = true;
    strcpy(estado, "Atendida");
    fechaModificacion = time(nullptr);
}

void Cita::cancelar() {
    atendida = false;
    strcpy(estado, "Cancelada");
    fechaModificacion = time(nullptr);
}

void Cita::reprogramar(const char* nuevaFecha, const char* nuevaHora) {
    if (strlen(nuevaFecha) == 10 && strlen(nuevaHora) == 5) {
        strcpy(fecha, nuevaFecha);
        strcpy(hora, nuevaHora);
        strcpy(estado, "Reprogramada");
        fechaModificacion = time(nullptr);
    }
}

bool Cita::estaPendiente() const {
    return strcmp(estado, "Agendada") == 0 || strcmp(estado, "Reprogramada") == 0;
}

bool Cita::estaCancelada() const {
    return strcmp(estado, "Cancelada") == 0;
}

bool Cita::estaAtendida() const {
    return strcmp(estado, "Atendida") == 0;
}

// ============ MÉTODOS DE PRESENTACIÓN ============
void Cita::mostrarInformacionBasica() const {
    cout << "\n=== INFORMACIÓN BÁSICA DE LA CITA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Paciente ID: " << pacienteID << endl;
    cout << "Doctor ID: " << doctorID << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Motivo: " << motivo << endl;
    cout << "Estado: " << estado << endl;
    cout << "Atendida: " << (atendida ? "Sí" : "No") << endl;
}

void Cita::mostrarInformacionCompleta() const {
    cout << "\n=== INFORMACIÓN COMPLETA DE LA CITA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Paciente ID: " << pacienteID << endl;
    cout << "Doctor ID: " << doctorID << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Motivo: " << motivo << endl;
    cout << "Estado: " << estado << endl;
    cout << "Observaciones: " << observaciones << endl;
    cout << "Atendida: " << (atendida ? "Sí" : "No") << endl;
    cout << "Consulta ID asociada: " << (consultaID > 0 ? to_string(consultaID) : "Ninguna") << endl;
    char buffer[26];
    ctime_r(&fechaCreacion, buffer);
    cout << "Fecha de creación: " << buffer;
    ctime_r(&fechaModificacion, buffer);
    cout << "Última modificación: " << buffer;
}

// ============ OPERADORES ============
Cita& Cita::operator=(const Cita& otro) {
    if (this != &otro) {
        id = otro.id;
        pacienteID = otro.pacienteID;
        doctorID = otro.doctorID;
        strcpy(fecha, otro.fecha);
        strcpy(hora, otro.hora);
        strcpy(motivo, otro.motivo);
        strcpy(estado, otro.estado);
        strcpy(observaciones, otro.observaciones);
        atendida = otro.atendida;
        consultaID = otro.consultaID;
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaModificacion = otro.fechaModificacion;
    }
    return *this;
}

// ============ MÉTODO ESTÁTICO ============
size_t Cita::obtenerTamano() {
    return sizeof(Cita);
}