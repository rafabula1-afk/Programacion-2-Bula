#include "Paciente.hpp"
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

Paciente::Paciente() {
    id = 0;
    nombre[0] = '\0';
    apellido[0] = '\0';
    cedula[0] = '\0';
    edad = 0;
    sexo = ' ';
    tipoSangre[0] = '\0';
    telefono[0] = '\0';
    direccion[0] = '\0';
    email[0] = '\0';
    alergias[0] = '\0';
    observaciones[0] = '\0';
    activo = true;
    cantidadConsultas = 0;
    primerConsultaID = -1;
    cantidadCitas = 0;
    for (int i = 0; i < 20; i++) citasIDs[i] = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Paciente::Paciente(int id, const char* nombre, const char* apellido, const char* cedula) {
    this->id = id;
    strncpy(this->nombre, nombre, 49);
    strncpy(this->apellido, apellido, 49);
    strncpy(this->cedula, cedula, 19);
    edad = 0;
    sexo = ' ';
    tipoSangre[0] = '\0';
    telefono[0] = '\0';
    direccion[0] = '\0';
    email[0] = '\0';
    alergias[0] = '\0';
    observaciones[0] = '\0';
    activo = true;
    cantidadConsultas = 0;
    primerConsultaID = -1;
    cantidadCitas = 0;
    for (int i = 0; i < 20; i++) citasIDs[i] = -1;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Paciente::Paciente(const Paciente& otro) {
    id = otro.id;
    strcpy(nombre, otro.nombre);
    strcpy(apellido, otro.apellido);
    strcpy(cedula, otro.cedula);
    edad = otro.edad;
    sexo = otro.sexo;
    strcpy(tipoSangre, otro.tipoSangre);
    strcpy(telefono, otro.telefono);
    strcpy(direccion, otro.direccion);
    strcpy(email, otro.email);
    strcpy(alergias, otro.alergias);
    strcpy(observaciones, otro.observaciones);
    activo = otro.activo;
    cantidadConsultas = otro.cantidadConsultas;
    primerConsultaID = otro.primerConsultaID;
    cantidadCitas = otro.cantidadCitas;
    for (int i = 0; i < 20; i++) citasIDs[i] = otro.citasIDs[i];
    eliminado = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaModificacion = otro.fechaModificacion;
}

Paciente::~Paciente() {}

Paciente& Paciente::operator=(const Paciente& otro) {
    if (this != &otro) {
        id = otro.id;
        strcpy(nombre, otro.nombre);
        strcpy(apellido, otro.apellido);
        strcpy(cedula, otro.cedula);
        edad = otro.edad;
        sexo = otro.sexo;
        strcpy(tipoSangre, otro.tipoSangre);
        strcpy(telefono, otro.telefono);
        strcpy(direccion, otro.direccion);
        strcpy(email, otro.email);
        strcpy(alergias, otro.alergias);
        strcpy(observaciones, otro.observaciones);
        activo = otro.activo;
        cantidadConsultas = otro.cantidadConsultas;
        primerConsultaID = otro.primerConsultaID;
        cantidadCitas = otro.cantidadCitas;
        for (int i = 0; i < 20; i++) citasIDs[i] = otro.citasIDs[i];
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaModificacion = otro.fechaModificacion;
    }
    return *this;
}

//Getters
int Paciente::getId() const { return id; }
const char* Paciente::getNombre() const { return nombre; }
const char* Paciente::getApellido() const { return apellido; }
const char* Paciente::getCedula() const { return cedula; }
int Paciente::getEdad() const { return edad; }
char Paciente::getSexo() const { return sexo; }
const char* Paciente::getTipoSangre() const { return tipoSangre; }
const char* Paciente::getTelefono() const { return telefono; }
const char* Paciente::getDireccion() const { return direccion; }
const char* Paciente::getEmail() const { return email; }
const char* Paciente::getAlergias() const { return alergias; }
const char* Paciente::getObservaciones() const { return observaciones; }
bool Paciente::isActivo() const { return activo; }
int Paciente::getCantidadConsultas() const { return cantidadConsultas; }
int Paciente::getPrimerConsultaID() const { return primerConsultaID; }
int Paciente::getCantidadCitas() const { return cantidadCitas; }
const int* Paciente::getCitasIDs() const { return citasIDs; }
bool Paciente::isEliminado() const { return eliminado; }
time_t Paciente::getFechaCreacion() const { return fechaCreacion; }
time_t Paciente::getFechaModificacion() const { return fechaModificacion; }

//Setters
void Paciente::setNombre(const char* nombre) {
    strncpy(this->nombre, nombre, 49);
    this->nombre[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setApellido(const char* apellido) {
    strncpy(this->apellido, apellido, 49);
    this->apellido[49] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setCedula(const char* cedula) {
    strncpy(this->cedula, cedula, 19);
    this->cedula[19] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setEdad(int edad) {
    if (edad >= 0 && edad <= 120) {
        this->edad = edad;
        fechaModificacion = time(nullptr);
    }
}

void Paciente::setSexo(char sexo) {
    if (sexo == 'M' || sexo == 'F' || sexo == 'm' || sexo == 'f') {
        this->sexo = toupper(sexo);
        fechaModificacion = time(nullptr);
    }
}

void Paciente::setTipoSangre(const char* tipoSangre) {
    const char* tiposValidos[] = {"O+", "O-", "A+", "A-", "B+", "B-", "AB+", "AB-"};
    for (int i = 0; i < 8; i++) {
        if (strcasecmp(tipoSangre, tiposValidos[i]) == 0) {
            strcpy(this->tipoSangre, tiposValidos[i]);
            fechaModificacion = time(nullptr);
            return;
        }
    }
}

void Paciente::setTelefono(const char* telefono) {
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

void Paciente::setDireccion(const char* direccion) {
    strncpy(this->direccion, direccion, 99);
    this->direccion[99] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setEmail(const char* email) {
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

void Paciente::setAlergias(const char* alergias) {
    strncpy(this->alergias, alergias, 499);
    this->alergias[499] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setObservaciones(const char* observaciones) {
    strncpy(this->observaciones, observaciones, 499);
    this->observaciones[499] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setActivo(bool activo) {
    this->activo = activo;
    fechaModificacion = time(nullptr);
}

void Paciente::setPrimerConsultaID(int id) {
    this->primerConsultaID = id;
    fechaModificacion = time(nullptr);
}

void Paciente::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    this->activo = !eliminado;
    fechaModificacion = time(nullptr);
}

//Metodos de Validación
bool Paciente::validarDatos() const {
    return strlen(nombre) > 0 && strlen(apellido) > 0 && 
           strlen(cedula) > 0 && edad >= 0 && edad <= 120 &&
           (sexo == 'M' || sexo == 'F') && strlen(tipoSangre) > 0;
}

bool Paciente::esMayorDeEdad() const {
    return edad >= 18;
}

bool Paciente::cedulaEsValida() const {
    return strlen(cedula) > 5 && strlen(cedula) <= 20;
}

bool Paciente::tieneContactoEmergencia() const {
    return strlen(telefono) > 0 || strlen(email) > 0;
}

// Metodos Gestion de Relaciones
bool Paciente::agregarCitaID(int citaID) {
    if (cantidadCitas >= 20) return false;
    
    for (int i = 0; i < cantidadCitas; i++) {
        if (citasIDs[i] == citaID) return false;
    }
    
    citasIDs[cantidadCitas] = citaID;
    cantidadCitas++;
    fechaModificacion = time(nullptr);
    return true;
}

bool Paciente::eliminarCitaID(int citaID) {
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

bool Paciente::tieneCitas() const {
    return cantidadCitas > 0;
}

bool Paciente::tieneConsultas() const {
    return cantidadConsultas > 0;
}

// Metodos de Presentacion
void Paciente::mostrarInformacionBasica() const {
    cout << "\n=== INFORMACION BASICA DEL PACIENTE ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Edad: " << edad << endl;
    cout << "Sexo: " << sexo << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Estado: " << (activo ? "Activo" : "Inactivo") << endl;
}

void Paciente::mostrarInformacionCompleta() const {
    cout << "\n=== INFORMACION COMPLETA DEL PACIENTE ===" << endl;
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Cédula: " << cedula << endl;
    cout << "Edad: " << edad << endl;
    cout << "Sexo: " << sexo << endl;
    cout << "Tipo de Sangre: " << tipoSangre << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Direccion: " << direccion << endl;
    cout << "Email: " << email << endl;
    cout << "Alergias: " << alergias << endl;
    cout << "Observaciones: " << observaciones << endl;
    cout << "Citas programadas: " << cantidadCitas << endl;
    cout << "Consultas realizadas: " << cantidadConsultas << endl;
    cout << "Fecha de creacion: " << ctime(&fechaCreacion);
    cout << "Ultima modificacion: " << ctime(&fechaModificacion);
}

size_t Paciente::obtenerTamanio() {
    return sizeof(Paciente);
}