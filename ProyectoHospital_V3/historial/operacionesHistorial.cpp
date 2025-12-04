#include "operacionesHistorial.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Funciones de Gestion de Hospital
void verHistorialPaciente() {
    Formatos::mostrarEncabezado("HISTORIAL MEDICO DE PACIENTE");
    
    int idPaciente;
    if (!Formatos::leerEntero(idPaciente, "ID del paciente: ", 1, 10000)) {
        return;
    }

    Paciente paciente;
    if (!GestorArchivos::buscarRegistroPorID(idPaciente, paciente, Rutas::PACIENTES)) {
        cout << "\nNo se encontro un paciente con ID " << idPaciente << endl;
        return;
    }
    
    cout << "\nPaciente: " << paciente.getNombre() << " " << paciente.getApellido() << endl;

    int cantidadTotal;
    HistorialMedico* todasConsultas = GestorArchivos::listarRegistrosActivos<HistorialMedico>(cantidadTotal, Rutas::HISTORIALES);
    
    if (todasConsultas == nullptr) {
        cout << "\nNo hay consultas registradas en el sistema." << endl;
        return;
    }

    int consultasPaciente = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasConsultas[i].getPacienteID() == idPaciente) {
            consultasPaciente++;
        }
    }
    
    if (consultasPaciente == 0) {
        cout << "\nEste paciente no tiene consultas registradas." << endl;
        delete[] todasConsultas;
        return;
    }
    
    cout << "\nTotal de consultas: " << consultasPaciente << endl;
    cout << "\n+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    cout << "|  ID  |   FECHA    | HORA  | DOC. |                   DIAGNOSTICO                    |       COSTO      |" << endl;
    cout << "+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasConsultas[i].getPacienteID() == idPaciente) {
            cout << "| " << setw(4) << todasConsultas[i].getId() << " | "
                 << setw(10) << todasConsultas[i].getFecha() << " | "
                 << setw(5) << todasConsultas[i].getHora() << " | "
                 << setw(4) << todasConsultas[i].getDoctorID() << " | "
                 << setw(48) << left << string(todasConsultas[i].getDiagnostico()).substr(0, 48) << " | $"
                 << setw(14) << fixed << setprecision(2) << todasConsultas[i].getCosto() << " |" << endl;
        }
    }
    
    cout << "+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    
    delete[] todasConsultas;
}

void agregarConsultaManual() {
    Formatos::mostrarEncabezado("AGREGAR CONSULTA MANUAL");
    
    HistorialMedico nuevaConsulta;
    Hospital hospitalTemp;
    
    if (!pedirDatosConsulta(nuevaConsulta, hospitalTemp)) {
        cout << "Registro cancelado." << endl;
        return;
    }

    int nuevoID = hospitalTemp.generarNuevoIDConsulta();
    nuevaConsulta.setId(nuevoID);

    if (GestorArchivos::guardarRegistro(nuevaConsulta, Rutas::HISTORIALES)) {
        cout << "\nConsulta registrada exitosamente" << endl;
        cout << "ID asignado: " << nuevoID << endl;
    } else {
        cout << "\nError al registrar la consulta." << endl;
    }
}

void buscarConsultaPorID() {
    Formatos::mostrarEncabezado("BUSCAR CONSULTA POR ID");
    
    int id;
    if (!Formatos::leerEntero(id, "ID de la consulta: ", 1, 10000)) {
        return;
    }
    
    HistorialMedico consulta;
    if (GestorArchivos::buscarRegistroPorID(id, consulta, Rutas::HISTORIALES)) {
        cout << "\nConsulta encontrada:" << endl;
        consulta.mostrarInformacionCompleta();
    } else {
        cout << "\nNo se encontro una consulta con ID " << id << endl;
    }
}

void listarTodasConsultas() {
    Formatos::mostrarEncabezado("LISTA DE TODAS LAS CONSULTAS");
    
    int cantidad;
    HistorialMedico* consultas = GestorArchivos::listarRegistrosActivos<HistorialMedico>(cantidad, Rutas::HISTORIALES);
    
    if (consultas == nullptr) {
        cout << "\nNo hay consultas registradas." << endl;
        return;
    }
    
    cout << "\nTotal de consultas: " << cantidad << endl;
    cout << "\n+------+------+------------+-------+------+--------------------------------+" << endl;
    cout << "|  ID  | PAC. |   FECHA    | HORA  | DOC. |         DIAGNOSTICO            |" << endl;
    cout << "+------+------+------------+-------+------+--------------------------------+" << endl;
    
    for (int i = 0; i < cantidad; i++) {
        cout << "| " << setw(4) << consultas[i].getId() << " | "
             << setw(4) << consultas[i].getPacienteID() << " | "
             << setw(10) << consultas[i].getFecha() << " | "
             << setw(5) << consultas[i].getHora() << " | "
             << setw(4) << consultas[i].getDoctorID() << " | "
             << setw(30) << left << string(consultas[i].getDiagnostico()).substr(0, 30) << " |" << endl;
    }
    
    cout << "+------+------+------------+-------+------+--------------------------------+" << endl;
    
    delete[] consultas;
}

//Funciones Auxiliares
void mostrarMenuHistorial(Hospital& hospital) {
    int opcion;
    
    do {
        Formatos::mostrarEncabezado("GESTION DE HISTORIAL MEDICO");
        
        cout << "1. Ver historial de paciente" << endl;
        cout << "2. Agregar consulta manual" << endl;
        cout << "3. Buscar consulta por ID" << endl;
        cout << "4. Listar todas las consultas" << endl;
        cout << "5. Volver al menu principal" << endl;
        cout << "\nOpcion: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: verHistorialPaciente(); break;
            case 2: agregarConsultaManual(); break;
            case 3: buscarConsultaPorID(); break;
            case 4: listarTodasConsultas(); break;
            case 5: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opcion invalida." << endl;
        }
        
        if (opcion != 5) {
            Formatos::pausar();
        }
        
    } while (opcion != 5);
}

bool pedirDatosConsulta(HistorialMedico& consulta, Hospital& hospital) {
    char buffer[200];

    int idPaciente;
    if (!Formatos::leerEntero(idPaciente, "ID del paciente: ", 1, 10000)) {
        return false;
    }

    Paciente paciente;
    if (!GestorArchivos::buscarRegistroPorID(idPaciente, paciente, Rutas::PACIENTES)) {
        cout << "\nNo se encontro un paciente con ID " << idPaciente << endl;
        return false;
    }
    
    consulta.setPacienteID(idPaciente);

    int idDoctor;
    if (!Formatos::leerEntero(idDoctor, "ID del doctor: ", 1, 10000)) {
        return false;
    }

    Doctor doctor;
    if (!GestorArchivos::buscarRegistroPorID(idDoctor, doctor, Rutas::DOCTORES)) {
        cout << "\nNo se encontro un doctor con ID " << idDoctor << endl;
        return false;
    }
    
    consulta.setDoctorID(idDoctor);

    if (!Formatos::leerFecha(buffer, "Fecha de la consulta")) {
        return false;
    }
    consulta.setFecha(buffer);

    if (!Formatos::leerHora(buffer, "Hora de la consulta")) {
        return false;
    }
    consulta.setHora(buffer);

    cout << "Diagnostico: ";
    cin.getline(buffer, 200);
    consulta.setDiagnostico(buffer);

    cout << "Tratamiento: ";
    cin.getline(buffer, 200);
    consulta.setTratamiento(buffer);

    cout << "Medicamentos: ";
    cin.getline(buffer, 150);
    consulta.setMedicamentos(buffer);

    float costo;
    if (!Formatos::leerFloat(costo, "Costo de la consulta: $", 0.0f, 100000.0f)) {
        return false;
    }
    consulta.setCosto(costo);
    
    return true;
}