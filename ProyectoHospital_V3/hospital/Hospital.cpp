#include "Hospital.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

//Constructores
Hospital::Hospital() {
    strcpy(nombre, "Hospital Central");
    strcpy(direccion, "Direccion no especificada");
    strcpy(telefono, "000-000-0000");
    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;
    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

Hospital::Hospital(const char* nombre, const char* direccion, const char* telefono) {
    strncpy(this->nombre, nombre, 99);
    this->nombre[99] = '\0';
    strncpy(this->direccion, direccion, 149);
    this->direccion[149] = '\0';
    strncpy(this->telefono, telefono, 14);
    this->telefono[14] = '\0';
    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;
    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

Hospital::Hospital(const Hospital& otro) {
    strcpy(nombre, otro.nombre);
    strcpy(direccion, otro.direccion);
    strcpy(telefono, otro.telefono);
    siguienteIDPaciente = otro.siguienteIDPaciente;
    siguienteIDDoctor = otro.siguienteIDDoctor;
    siguienteIDCita = otro.siguienteIDCita;
    siguienteIDConsulta = otro.siguienteIDConsulta;
    totalPacientesRegistrados = otro.totalPacientesRegistrados;
    totalDoctoresRegistrados = otro.totalDoctoresRegistrados;
    totalCitasAgendadas = otro.totalCitasAgendadas;
    totalConsultasRealizadas = otro.totalConsultasRealizadas;
}

//Destructor
Hospital::~Hospital() {}

//Getters
const char* Hospital::getNombre() const { 
    return nombre; 
}

const char* Hospital::getDireccion() const { 
    return direccion; 
}

const char* Hospital::getTelefono() const { 
    return telefono; 
}

int Hospital::getSiguienteIDPaciente() const { 
    return siguienteIDPaciente; 
}

int Hospital::getSiguienteIDDoctor() const { 
    return siguienteIDDoctor; 
}

int Hospital::getSiguienteIDCita() const { 
    return siguienteIDCita; 
}

int Hospital::getSiguienteIDConsulta() const { 
    return siguienteIDConsulta; 
}

int Hospital::getTotalPacientesRegistrados() const { 
    return totalPacientesRegistrados; 
}

int Hospital::getTotalDoctoresRegistrados() const { 
    return totalDoctoresRegistrados; 
}

int Hospital::getTotalCitasAgendadas() const { 
    return totalCitasAgendadas; 
}

int Hospital::getTotalConsultasRealizadas() const { 
    return totalConsultasRealizadas; 
}

//Setters
void Hospital::setNombre(const char* nombre) {
    strncpy(this->nombre, nombre, 99);
    this->nombre[99] = '\0';
}

void Hospital::setDireccion(const char* direccion) {
    strncpy(this->direccion, direccion, 149);
    this->direccion[149] = '\0';
}

void Hospital::setTelefono(const char* telefono) {
    strncpy(this->telefono, telefono, 14);
    this->telefono[14] = '\0';
}

void Hospital::setSiguienteIDPaciente(int id) {
    siguienteIDPaciente = id;
}

void Hospital::setSiguienteIDDoctor(int id) {
    siguienteIDDoctor = id;
}

void Hospital::setSiguienteIDCita(int id) {
    siguienteIDCita = id;
}

void Hospital::setSiguienteIDConsulta(int id) {
    siguienteIDConsulta = id;
}

void Hospital::setTotalPacientesRegistrados(int cantidad) {
    totalPacientesRegistrados = cantidad;
}

void Hospital::setTotalDoctoresRegistrados(int cantidad) {
    totalDoctoresRegistrados = cantidad;
}

void Hospital::setTotalCitasAgendadas(int cantidad) {
    totalCitasAgendadas = cantidad;
}

void Hospital::setTotalConsultasRealizadas(int cantidad) {
    totalConsultasRealizadas = cantidad;
}

//Metodos de Gestion
int Hospital::generarNuevoIDPaciente() {
    return siguienteIDPaciente++;
}

int Hospital::generarNuevoIDDoctor() {
    return siguienteIDDoctor++;
}

int Hospital::generarNuevoIDCita() {
    return siguienteIDCita++;
}

int Hospital::generarNuevoIDConsulta() {
    return siguienteIDConsulta++;
}

void Hospital::incrementarPacientesRegistrados() {
    totalPacientesRegistrados++;
}

void Hospital::incrementarDoctoresRegistrados() {
    totalDoctoresRegistrados++;
}

void Hospital::incrementarCitasAgendadas() {
    totalCitasAgendadas++;
}

void Hospital::incrementarConsultasRealizadas() {
    totalConsultasRealizadas++;
}

void Hospital::decrementarPacientesRegistrados() {
    if (totalPacientesRegistrados > 0) {
        totalPacientesRegistrados--;
    }
}

void Hospital::decrementarDoctoresRegistrados() {
    if (totalDoctoresRegistrados > 0) {
        totalDoctoresRegistrados--;
    }
}

void Hospital::decrementarCitasAgendadas() {
    if (totalCitasAgendadas > 0) {
        totalCitasAgendadas--;
    }
}

void Hospital::decrementarConsultasRealizadas() {
    if (totalConsultasRealizadas > 0) {
        totalConsultasRealizadas--;
    }
}

//Metodos de Validacion
bool Hospital::validarDatos() const {
    return strlen(nombre) > 0 && strlen(direccion) > 0 && strlen(telefono) > 0;
}

//Metodos de Presentacion
void Hospital::mostrarInformacion() const {
    cout << "\n=== INFORMACION DEL HOSPITAL ===" << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Direccion: " << direccion << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Proximo ID Paciente: " << siguienteIDPaciente << endl;
    cout << "Proximo ID Doctor: " << siguienteIDDoctor << endl;
    cout << "Proximo ID Cita: " << siguienteIDCita << endl;
    cout << "Proximo ID Consulta: " << siguienteIDConsulta << endl;
}

void Hospital::mostrarEstadisticas() const {
    cout << "\n=== ESTADISTICAS DEL HOSPITAL ===" << endl;
    cout << "Total pacientes registrados: " << totalPacientesRegistrados << endl;
    cout << "Total doctores registrados: " << totalDoctoresRegistrados << endl;
    cout << "Total citas agendadas: " << totalCitasAgendadas << endl;
    cout << "Total consultas realizadas: " << totalConsultasRealizadas << endl;
    
    if (totalDoctoresRegistrados > 0) {
        float pacientesPorDoctor = (float)totalPacientesRegistrados / totalDoctoresRegistrados;
        cout << "Pacientes por doctor: " << fixed << setprecision(1) << pacientesPorDoctor << endl;
    }
    
    if (totalPacientesRegistrados > 0) {
        float consultasPorPaciente = (float)totalConsultasRealizadas / totalPacientesRegistrados;
        cout << "Consultas por paciente: " << fixed << setprecision(2) << consultasPorPaciente << endl;
    }
    
    if (totalDoctoresRegistrados > 0) {
        int horasDisponibles = totalDoctoresRegistrados * 8 * 20; // 8 horas/día * 20 días/mes
        int horasCitas = totalCitasAgendadas; // 1 cita = 1 hora
        float tasaOcupacion = (float)horasCitas / horasDisponibles * 100;
        cout << "Tasa de ocupacion: " << fixed << setprecision(1) << tasaOcupacion << "%" << endl;
    }
}

//Operadores
Hospital& Hospital::operator=(const Hospital& otro) {
    if (this != &otro) {
        strcpy(nombre, otro.nombre);
        strcpy(direccion, otro.direccion);
        strcpy(telefono, otro.telefono);
        siguienteIDPaciente = otro.siguienteIDPaciente;
        siguienteIDDoctor = otro.siguienteIDDoctor;
        siguienteIDCita = otro.siguienteIDCita;
        siguienteIDConsulta = otro.siguienteIDConsulta;
        totalPacientesRegistrados = otro.totalPacientesRegistrados;
        totalDoctoresRegistrados = otro.totalDoctoresRegistrados;
        totalCitasAgendadas = otro.totalCitasAgendadas;
        totalConsultasRealizadas = otro.totalConsultasRealizadas;
    }
    return *this;
}

//Metodo Estatico
size_t Hospital::obtenerTamanio() {
    return sizeof(Hospital);
}