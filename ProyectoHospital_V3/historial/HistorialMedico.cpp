#include "HistorialMedico.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>

using namespace std;

//Constructores
HistorialMedico::HistorialMedico() {
    id = 0;
    pacienteID = -1;
    fecha[0] = '\0';
    hora[0] = '\0';
    diagnostico[0] = '\0';
    tratamiento[0] = '\0';
    medicamentos[0] = '\0';
    doctorID = -1;
    costo = 0.0f;
    siguienteConsultaID = -1;
    eliminado = false;
    fechaRegistro = time(nullptr);
}

HistorialMedico::HistorialMedico(int id, int pacienteID, int doctorID, const char* fecha, const char* hora, const char* diagnostico) {
    this->id = id;
    this->pacienteID = pacienteID;
    strncpy(this->fecha, fecha, 10);
    this->fecha[10] = '\0';
    strncpy(this->hora, hora, 5);
    this->hora[5] = '\0';
    strncpy(this->diagnostico, diagnostico, 199);
    this->diagnostico[199] = '\0';
    tratamiento[0] = '\0';
    medicamentos[0] = '\0';
    this->doctorID = doctorID;
    costo = 0.0f;
    siguienteConsultaID = -1;
    eliminado = false;
    fechaRegistro = time(nullptr);
}

HistorialMedico::HistorialMedico(const HistorialMedico& otro) {
    id = otro.id;
    pacienteID = otro.pacienteID;
    strcpy(fecha, otro.fecha);
    strcpy(hora, otro.hora);
    strcpy(diagnostico, otro.diagnostico);
    strcpy(tratamiento, otro.tratamiento);
    strcpy(medicamentos, otro.medicamentos);
    doctorID = otro.doctorID;
    costo = otro.costo;
    siguienteConsultaID = otro.siguienteConsultaID;
    eliminado = otro.eliminado;
    fechaRegistro = otro.fechaRegistro;
}

//Destructores
HistorialMedico::~HistorialMedico() {}

//Getters
int HistorialMedico::getId() const { return id; }
int HistorialMedico::getPacienteID() const { return pacienteID; }
const char* HistorialMedico::getFecha() const { return fecha; }
const char* HistorialMedico::getHora() const { return hora; }
const char* HistorialMedico::getDiagnostico() const { return diagnostico; }
const char* HistorialMedico::getTratamiento() const { return tratamiento; }
const char* HistorialMedico::getMedicamentos() const { return medicamentos; }
int HistorialMedico::getDoctorID() const { return doctorID; }
float HistorialMedico::getCosto() const { return costo; }
int HistorialMedico::getSiguienteConsultaID() const { return siguienteConsultaID; }
bool HistorialMedico::isEliminado() const { return eliminado; }
time_t HistorialMedico::getFechaRegistro() const { return fechaRegistro; }

//Setters
void HistorialMedico::setId(int id) { this->id = id; }

void HistorialMedico::setPacienteID(int pacienteID) {
    this->pacienteID = pacienteID;
}

void HistorialMedico::setFecha(const char* fecha) {
    if (strlen(fecha) == 10 && fecha[4] == '-' && fecha[7] == '-') {
        strncpy(this->fecha, fecha, 10);
        this->fecha[10] = '\0';
    }
}

void HistorialMedico::setHora(const char* hora) {
    if (strlen(hora) == 5 && hora[2] == ':') {
        strncpy(this->hora, hora, 5);
        this->hora[5] = '\0';
    }
}

void HistorialMedico::setDiagnostico(const char* diagnostico) {
    strncpy(this->diagnostico, diagnostico, 199);
    this->diagnostico[199] = '\0';
}

void HistorialMedico::setTratamiento(const char* tratamiento) {
    strncpy(this->tratamiento, tratamiento, 199);
    this->tratamiento[199] = '\0';
}

void HistorialMedico::setMedicamentos(const char* medicamentos) {
    strncpy(this->medicamentos, medicamentos, 149);
    this->medicamentos[149] = '\0';
}

void HistorialMedico::setDoctorID(int doctorID) {
    this->doctorID = doctorID;
}

void HistorialMedico::setCosto(float costo) {
    if (costo >= 0.0f && costo <= 100000.0f) {
        this->costo = costo;
    }
}

void HistorialMedico::setSiguienteConsultaID(int siguienteConsultaID) {
    this->siguienteConsultaID = siguienteConsultaID;
}

void HistorialMedico::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void HistorialMedico::setFechaRegistro(time_t fecha) {
    this->fechaRegistro = fecha;
}

//Metodos de Validacion
bool HistorialMedico::validarDatos() const {
    return pacienteID > 0 && doctorID > 0 && 
           strlen(fecha) == 10 && strlen(hora) == 5 &&
           strlen(diagnostico) > 0 && costo >= 0.0f;
}

bool HistorialMedico::fechaHoraValidas() const {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int anio, mes, dia;
    int horaInt, minuto;
    
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return false;
    if (sscanf(hora, "%d:%d", &horaInt, &minuto) != 2) return false;
    
    if (anio < 2000 || anio > 2030) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (horaInt < 0 || horaInt > 23) return false;
    if (minuto < 0 || minuto > 59) return false;
    
    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto && dia > 29) return false;
        if (!bisiesto && dia > 28) return false;
    } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return false;
    }
    
    return true;
}

bool HistorialMedico::costoValido() const {
    return costo >= 0.0f && costo <= 100000.0f;
}

//Metodos de Navegacion
bool HistorialMedico::tieneSiguienteConsulta() const {
    return siguienteConsultaID > 0;
}

void HistorialMedico::enlazarSiguienteConsulta(int siguienteID) {
    siguienteConsultaID = siguienteID;
}

void HistorialMedico::desenlazarSiguienteConsulta() {
    siguienteConsultaID = -1;
}

//Metodos de Presentacion
void HistorialMedico::mostrarInformacionBasica() const {
    cout << "\n=== INFORMACIÓN BÁSICA DE CONSULTA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Paciente ID: " << pacienteID << endl;
    cout << "Doctor ID: " << doctorID << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Diagnóstico: " << diagnostico << endl;
    cout << "Costo: $" << costo << endl;
}

void HistorialMedico::mostrarInformacionCompleta() const {
    cout << "\n=== INFORMACIÓN COMPLETA DE CONSULTA ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Paciente ID: " << pacienteID << endl;
    cout << "Doctor ID: " << doctorID << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "Hora: " << hora << endl;
    cout << "Diagnóstico: " << diagnostico << endl;
    cout << "Tratamiento: " << tratamiento << endl;
    cout << "Medicamentos: " << medicamentos << endl;
    cout << "Costo: $" << costo << endl;
    cout << "Siguiente consulta ID: " << (siguienteConsultaID > 0 ? to_string(siguienteConsultaID) : "Ninguna") << endl;
    char buffer[26];
    ctime_r(&fechaRegistro, buffer);
    cout << "Fecha de registro: " << buffer;
}

//Operadores
HistorialMedico& HistorialMedico::operator=(const HistorialMedico& otro) {
    if (this != &otro) {
        id = otro.id;
        pacienteID = otro.pacienteID;
        strcpy(fecha, otro.fecha);
        strcpy(hora, otro.hora);
        strcpy(diagnostico, otro.diagnostico);
        strcpy(tratamiento, otro.tratamiento);
        strcpy(medicamentos, otro.medicamentos);
        doctorID = otro.doctorID;
        costo = otro.costo;
        siguienteConsultaID = otro.siguienteConsultaID;
        eliminado = otro.eliminado;
        fechaRegistro = otro.fechaRegistro;
    }
    return *this;
}

//Metodo Estatico
size_t HistorialMedico::obtenerTamanio() {
    return sizeof(HistorialMedico);
}