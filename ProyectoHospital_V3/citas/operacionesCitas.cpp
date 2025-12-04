#include "operacionesCitas.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Funciones de Gestion de Citas
void agendarCita(Hospital& hospital) {
    Formatos::mostrarEncabezado("AGENDAR NUEVA CITA");
    
    Cita nuevaCita;
    
    if (!pedirDatosCita(nuevaCita, hospital, false)) {
        cout << "Agendamiento cancelado." << endl;
        return;
    }

    Paciente paciente;
    Doctor doctor;
    
    if (!GestorArchivos::buscarRegistroPorID(nuevaCita.getPacienteID(), paciente, Rutas::PACIENTES)) {
        cout << "\nNo se encontro un paciente con ID " << nuevaCita.getPacienteID() << endl;
        return;
    }
    
    if (!GestorArchivos::buscarRegistroPorID(nuevaCita.getDoctorID(), doctor, Rutas::DOCTORES)) {
        cout << "\nNo se encontro un doctor con ID " << nuevaCita.getDoctorID() << endl;
        return;
    }

    if (!doctor.estaDisponible()) {
        cout << "\nEl doctor no esta disponible actualmente." << endl;
        return;
    }

    int nuevoID = hospital.generarNuevoIDCita();
    nuevaCita.setId(nuevoID);
    nuevaCita.setEstado("Agendada");

    if (GestorArchivos::guardarRegistro(nuevaCita, Rutas::CITAS)) {
        hospital.incrementarCitasAgendadas();

        paciente.agregarCitaID(nuevoID);
        doctor.agregarCitaID(nuevoID);

        int cantidad;

        Paciente* pacientes = GestorArchivos::listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
        if (pacientes != nullptr) {
            for (int i = 0; i < cantidad; i++) {
                if (pacientes[i].getId() == nuevaCita.getPacienteID()) {
                    GestorArchivos::actualizarRegistroPorIndice(i, paciente, Rutas::PACIENTES);
                    break;
                }
            }
            delete[] pacientes;
        }

        Doctor* doctores = GestorArchivos::listarRegistrosActivos<Doctor>(cantidad, Rutas::DOCTORES);
        if (doctores != nullptr) {
            for (int i = 0; i < cantidad; i++) {
                if (doctores[i].getId() == nuevaCita.getDoctorID()) {
                    GestorArchivos::actualizarRegistroPorIndice(i, doctor, Rutas::DOCTORES);
                    break;
                }
            }
            delete[] doctores;
        }
        
        cout << "\nCita agendada exitosamente" << endl;
        cout << "ID asignado: " << nuevoID << endl;
    } else {
        cout << "\nError al agendar la cita." << endl;
    }
}

void cancelarCita() {
    Formatos::mostrarEncabezado("CANCELAR CITA");
    
    int id;
    if (!Formatos::leerEntero(id, "ID de la cita a cancelar: ", 1, 10000)) {
        return;
    }

    Cita cita;
    if (!GestorArchivos::buscarRegistroPorID(id, cita, Rutas::CITAS)) {
        cout << "\nNo se encontro una cita con ID " << id << endl;
        return;
    }

    if (cita.estaAtendida()) {
        cout << "\nNo se puede cancelar una cita ya atendida." << endl;
        return;
    }

    cout << "\n¿Esta seguro que desea cancelar esta cita? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    Formatos::limpiarBuffer();
    
    if (tolower(confirmacion) != 's') {
        cout << "Cancelacion abortada." << endl;
        return;
    }

    cita.cancelar();

    int cantidad;
    Cita* citas = GestorArchivos::listarRegistrosActivos<Cita>(cantidad, Rutas::CITAS);
    int indice = -1;
    
    if (citas != nullptr) {
        for (int i = 0; i < cantidad; i++) {
            if (citas[i].getId() == id) {
                indice = i;
                break;
            }
        }
        
        if (indice != -1 && GestorArchivos::actualizarRegistroPorIndice(indice, cita, Rutas::CITAS)) {
            cout << "\nCita cancelada correctamente." << endl;
        } else {
            cout << "\nError al cancelar la cita." << endl;
        }
        delete[] citas;
    }
}

void atenderCita() {
    Formatos::mostrarEncabezado("ATENDER CITA");
    
    int id;
    if (!Formatos::leerEntero(id, "ID de la cita a atender: ", 1, 10000)) {
        return;
    }

    Cita cita;
    if (!GestorArchivos::cargarCitaPorID(id, cita)) {
        cout << "\nNo se encontro una cita con ID " << id << endl;
        return;
    }

    if (cita.isAtendida()) {
        cout << "\nEsta cita ya fue atendida anteriormente." << endl;
        cita.mostrarInformacionBasica();
        return;
    }

    if (cita.estaCancelada()) {
        cout << "\nEsta cita está cancelada, no se puede atender." << endl;
        return;
    }
    
    cout << "\nCITA ENCONTRADA:" << endl;
    cita.mostrarInformacionBasica();

    Paciente paciente;
    Doctor doctor;
    
    if (GestorArchivos::cargarPacientePorID(cita.getPacienteID(), paciente)) {
        cout << "\nPACIENTE:" << endl;
        cout << "Nombre: " << paciente.getNombre() << " " << paciente.getApellido() << endl;
        cout << "Cedula: " << paciente.getCedula() << endl;
        cout << "Edad: " << paciente.getEdad() << endl;
        cout << "Tipo de sangre: " << paciente.getTipoSangre() << endl;
        cout << "Alergias: " << (strlen(paciente.getAlergias()) > 0 ? paciente.getAlergias() : "Ninguna") << endl;
    }
    
    if (GestorArchivos::cargarDoctorPorID(cita.getDoctorID(), doctor)) {
        cout << "\nDOCTOR:" << endl;
        cout << "Nombre: " << doctor.getNombre() << " " << doctor.getApellido() << endl;
        cout << "Especialidad: " << doctor.getEspecialidad() << endl;
        cout << "Costo de consulta: $" << fixed << setprecision(2) << doctor.getCostoConsulta() << endl;
    }

    char diagnostico[200], tratamiento[200], medicamentos[150];
    
    cout << "\nINGRESE LOS DATOS DE LA CONSULTA:" << endl;
    cout << "------------------------------------" << endl;
    
    do {
        cout << "Diagnostico (requerido): ";
        cin.getline(diagnostico, 200);
    } while (strlen(diagnostico) == 0);
    
    cout << "Tratamiento: ";
    cin.getline(tratamiento, 200);
    
    cout << "Medicamentos recetados: ";
    cin.getline(medicamentos, 150);

    cout << "\n¿Confirmar atencion de la cita? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    Formatos::limpiarBuffer();
    
    if (tolower(confirmacion) != 's') {
        cout << "Atencion cancelada." << endl;
        return;
    }

    HistorialMedico consulta;

    Hospital hospitalTemp;
    int nuevoIDConsulta = hospitalTemp.generarNuevoIDConsulta();
    
    consulta.setId(nuevoIDConsulta);
    consulta.setPacienteID(cita.getPacienteID());
    consulta.setDoctorID(cita.getDoctorID());
    consulta.setFecha(cita.getFecha());
    consulta.setHora(cita.getHora());
    consulta.setDiagnostico(diagnostico);
    consulta.setTratamiento(tratamiento);
    consulta.setMedicamentos(medicamentos);
    consulta.setCosto(doctor.getCostoConsulta());

    if (GestorArchivos::guardarConsulta(consulta)) {
        cita.marcarComoAtendida();
        cita.setConsultaID(nuevoIDConsulta);
        cita.setObservaciones(diagnostico);

        if (GestorArchivos::actualizarCita(cita)) {
            paciente.setCantidadConsultas(paciente.getCantidadConsultas() + 1);
            if (paciente.getPrimerConsultaID() == -1) {
                paciente.setPrimerConsultaID(nuevoIDConsulta);
            }
            
            GestorArchivos::actualizarPaciente(paciente);
            
            cout << "\nCITA ATENDIDA EXITOSAMENTE" << endl;
            cout << "=============================" << endl;
            cout << "✓ Cita marcada como atendida" << endl;
            cout << "✓ Historial medico actualizado" << endl;
            cout << "✓ ID de consulta: " << nuevoIDConsulta << endl;
            cout << "✓ Costo: $" << fixed << setprecision(2) << doctor.getCostoConsulta() << endl;
            cout << "✓ Diagnostico registrado" << endl;
            
        } else {
            cout << "\nError al actualizar la cita." << endl;
        }
    } else {
        cout << "\nError al registrar en el historial medico." << endl;
    }
}

void buscarCitasPorPaciente() {
    Formatos::mostrarEncabezado("BUSCAR CITAS POR PACIENTE");
    
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
    Cita* todasCitas = GestorArchivos::listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cout << "\nNo hay citas registradas." << endl;
        return;
    }

    int citasPaciente = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getPacienteID() == idPaciente) {
            citasPaciente++;
        }
    }
    
    if (citasPaciente == 0) {
        cout << "\nEste paciente no tiene citas registradas." << endl;
        delete[] todasCitas;
        return;
    }
    
    cout << "\nTotal de citas: " << citasPaciente << endl;
    cout << "\n+------+------------+-------+------+-----------------------------+" << endl;
    cout << "|  ID  |   FECHA    | HORA  | ESTADO |         MOTIVO            |" << endl;
    cout << "+------+------------+-------+--------+---------------------------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getPacienteID() == idPaciente) {
            cout << "| " << setw(4) << todasCitas[i].getId() << " | "
                 << setw(10) << todasCitas[i].getFecha() << " | "
                 << setw(5) << todasCitas[i].getHora() << " | "
                 << setw(6) << todasCitas[i].getEstado() << " | "
                 << setw(25) << left << string(todasCitas[i].getMotivo()).substr(0, 25) << " |" << endl;
        }
    }
    
    cout << "+------+------------+-------+--------+---------------------------+" << endl;
    
    delete[] todasCitas;
}

void buscarCitasPorDoctor() {
    Formatos::mostrarEncabezado("BUSCAR CITAS POR DOCTOR");
    
    int idDoctor;
    if (!Formatos::leerEntero(idDoctor, "ID del doctor: ", 1, 10000)) {
        return;
    }

    Doctor doctor;
    if (!GestorArchivos::buscarRegistroPorID(idDoctor, doctor, Rutas::DOCTORES)) {
        cout << "\nNo se encontro un doctor con ID " << idDoctor << endl;
        return;
    }
    
    cout << "\nDoctor: " << doctor.getNombre() << " " << doctor.getApellido() << endl;

    int cantidadTotal;
    Cita* todasCitas = GestorArchivos::listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cout << "\nNo hay citas registradas." << endl;
        return;
    }

    int citasDoctor = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getDoctorID() == idDoctor) {
            citasDoctor++;
        }
    }
    
    if (citasDoctor == 0) {
        cout << "\nEste doctor no tiene citas registradas." << endl;
        delete[] todasCitas;
        return;
    }
    
    cout << "\nTotal de citas: " << citasDoctor << endl;
    cout << "\n+------+------------+-------+--------+-----------------------------+" << endl;
    cout << "|  ID  |   FECHA    | HORA  | ESTADO |         MOTIVO            |" << endl;
    cout << "+------+------------+-------+--------+---------------------------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getDoctorID() == idDoctor) {
            cout << "| " << setw(4) << todasCitas[i].getId() << " | "
                 << setw(10) << todasCitas[i].getFecha() << " | "
                 << setw(5) << todasCitas[i].getHora() << " | "
                 << setw(6) << todasCitas[i].getEstado() << " | "
                 << setw(25) << left << string(todasCitas[i].getMotivo()).substr(0, 25) << " |" << endl;
        }
    }
    
    cout << "+------+------------+-------+--------+---------------------------+" << endl;
    
    delete[] todasCitas;
}

void buscarCitasPorFecha() {
    Formatos::mostrarEncabezado("BUSCAR CITAS POR FECHA");
    
    char fecha[11];
    if (!Formatos::leerFecha(fecha, "Fecha")) {
        return;
    }

    int cantidadTotal;
    Cita* todasCitas = GestorArchivos::listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cout << "\nNo hay citas registradas." << endl;
        return;
    }

    int citasFecha = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getFecha(), fecha) == 0) {
            citasFecha++;
        }
    }
    
    if (citasFecha == 0) {
        cout << "\nNo hay citas registradas para la fecha " << fecha << endl;
        delete[] todasCitas;
        return;
    }
    
    cout << "\nCitas para " << fecha << ": " << citasFecha << " encontrada(s)" << endl;
    cout << "\n+------+-------+------+------+-----------------------------+" << endl;
    cout << "|  ID  | HORA  | PAC. | DOC. |         MOTIVO            |" << endl;
    cout << "+------+-------+------+------+---------------------------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getFecha(), fecha) == 0) {
            cout << "| " << setw(4) << todasCitas[i].getId() << " | "
                 << setw(5) << todasCitas[i].getHora() << " | "
                 << setw(4) << todasCitas[i].getPacienteID() << " | "
                 << setw(4) << todasCitas[i].getDoctorID() << " | "
                 << setw(25) << left << string(todasCitas[i].getMotivo()).substr(0, 25) << " |" << endl;
        }
    }
    
    cout << "+------+-------+------+------+---------------------------+" << endl;
    
    delete[] todasCitas;
}

void listarCitasPendientes() {
    Formatos::mostrarEncabezado("CITAS PENDIENTES");

    int cantidadTotal;
    Cita* todasCitas = GestorArchivos::listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cout << "\nNo hay citas registradas." << endl;
        return;
    }

    int citasPendientes = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getEstado(), "Agendada") == 0 || 
            strcmp(todasCitas[i].getEstado(), "Reprogramada") == 0) {
            citasPendientes++;
        }
    }
    
    if (citasPendientes == 0) {
        cout << "\nNo hay citas pendientes." << endl;
        delete[] todasCitas;
        return;
    }
    
    cout << "\nTotal de citas pendientes: " << citasPendientes << endl;
    cout << "\n+------+------------+-------+------+------+-----------------------------+" << endl;
    cout << "|  ID  |   FECHA    | HORA  | PAC. | DOC. |         MOTIVO            |" << endl;
    cout << "+------+------------+-------+------+------+---------------------------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getEstado(), "Agendada") == 0 || 
            strcmp(todasCitas[i].getEstado(), "Reprogramada") == 0) {
            cout << "| " << setw(4) << todasCitas[i].getId() << " | "
                 << setw(10) << todasCitas[i].getFecha() << " | "
                 << setw(5) << todasCitas[i].getHora() << " | "
                 << setw(4) << todasCitas[i].getPacienteID() << " | "
                 << setw(4) << todasCitas[i].getDoctorID() << " | "
                 << setw(25) << left << string(todasCitas[i].getMotivo()).substr(0, 25) << " |" << endl;
        }
    }
    
    cout << "+------+------------+-------+------+------+---------------------------+" << endl;
    
    delete[] todasCitas;
}

void verificarDisponibilidad() {
    Formatos::mostrarEncabezado("VERIFICAR DISPONIBILIDAD DE DOCTOR");
    
    int idDoctor;
    char fecha[11], hora[6];
    
    if (!Formatos::leerEntero(idDoctor, "ID del doctor: ", 1, 10000)) {
        return;
    }
    
    if (!Formatos::leerFecha(fecha, "Fecha")) {
        return;
    }
    
    if (!Formatos::leerHora(hora, "Hora")) {
        return;
    }

    Doctor doctor;
    if (!GestorArchivos::buscarRegistroPorID(idDoctor, doctor, Rutas::DOCTORES)) {
        cout << "\nNo se encontro un doctor con ID " << idDoctor << endl;
        return;
    }
    
    if (!doctor.estaDisponible()) {
        cout << "\nEl doctor no está disponible actualmente." << endl;
        return;
    }

    int cantidadTotal;
    Cita* todasCitas = GestorArchivos::listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    bool disponible = true;
    
    if (todasCitas != nullptr) {
        for (int i = 0; i < cantidadTotal; i++) {
            if (todasCitas[i].getDoctorID() == idDoctor &&
                strcmp(todasCitas[i].getFecha(), fecha) == 0 &&
                strcmp(todasCitas[i].getHora(), hora) == 0 &&
                !todasCitas[i].estaAtendida() && 
                !todasCitas[i].estaCancelada()) {
                disponible = false;
                break;
            }
        }
        delete[] todasCitas;
    }
    
    if (disponible) {
        cout << "\nEl doctor esta disponible en " << fecha << " a las " << hora << endl;
    } else {
        cout << "\nEl doctor NO esta disponible en " << fecha << " a las " << hora << endl;
        cout << "Ya tiene una cita agendada en ese horario." << endl;
    }
}

//Funciones Auxiliares
void mostrarMenuCitas(Hospital& hospital) {
    int opcion;
    
    do {
        Formatos::mostrarEncabezado("GESTIoN DE CITAS");
        
        cout << "1. Agendar nueva cita" << endl;
        cout << "2. Cancelar cita" << endl;
        cout << "3. Atender cita" << endl;
        cout << "4. Buscar por paciente" << endl;
        cout << "5. Buscar por doctor" << endl;
        cout << "6. Buscar por fecha" << endl;
        cout << "7. Listar pendientes" << endl;
        cout << "8. Verificar disponibilidad" << endl;
        cout << "9. Volver al menu principal" << endl;
        cout << "\nOpcion: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: agendarCita(hospital); break;
            case 2: cancelarCita(); break;
            case 3: atenderCita(); break;
            case 4: buscarCitasPorPaciente(); break;
            case 5: buscarCitasPorDoctor(); break;
            case 6: buscarCitasPorFecha(); break;
            case 7: listarCitasPendientes(); break;
            case 8: verificarDisponibilidad(); break;
            case 9: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opcion inválida." << endl;
        }
        
        if (opcion != 9) {
            Formatos::pausar();
        }
        
    } while (opcion != 9);
}

bool pedirDatosCita(Cita& cita, Hospital& hospital, bool esModificacion) {
    char buffer[200];

    do {
        cout << "ID del paciente" << (esModificacion ? " (actual: " + to_string(cita.getPacienteID()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 20);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 20);
        }
        
        if (strlen(buffer) > 0) {
            int id = atoi(buffer);
            if (id <= 0) {
                cout << "Error: ID invalido." << endl;
                if (!esModificacion) continue;
            } else {
                cita.setPacienteID(id);
            }
        }
    } while (!esModificacion && cita.getPacienteID() <= 0);

    do {
        cout << "ID del doctor" << (esModificacion ? " (actual: " + to_string(cita.getDoctorID()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 20);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 20);
        }
        
        if (strlen(buffer) > 0) {
            int id = atoi(buffer);
            if (id <= 0) {
                cout << "Error: ID invalido." << endl;
                if (!esModificacion) continue;
            } else {
                cita.setDoctorID(id);
            }
        }
    } while (!esModificacion && cita.getDoctorID() <= 0);

    do {
        cout << "Fecha" << (esModificacion ? " (actual: " + string(cita.getFecha()) + ")" : "") << ": ";
        if (esModificacion) {
            Formatos::leerFecha(buffer, "Fecha");
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            if (!Formatos::leerFecha(buffer, "Fecha")) {
                return false;
            }
        }
        
        if (strlen(buffer) > 0) {
            if (!Validaciones::validarFecha(buffer)) {
                cout << "Error: Fecha invalida." << endl;
                if (!esModificacion) continue;
            } else {
                cita.setFecha(buffer);
            }
        }
    } while (!esModificacion && strlen(cita.getFecha()) == 0);

    do {
        cout << "Hora" << (esModificacion ? " (actual: " + string(cita.getHora()) + ")" : "") << ": ";
        if (esModificacion) {
            Formatos::leerHora(buffer, "Hora");
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            if (!Formatos::leerHora(buffer, "Hora")) {
                return false;
            }
        }
        
        if (strlen(buffer) > 0) {
            if (!Validaciones::validarHora(buffer)) {
                cout << "Error: Hora invalida." << endl;
                if (!esModificacion) continue;
            } else {
                cita.setHora(buffer);
            }
        }
    } while (!esModificacion && strlen(cita.getHora()) == 0);

    cout << "Motivo" << (esModificacion ? " (actual: " + string(cita.getMotivo()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 150);
        if (strlen(buffer) > 0) {
            cita.setMotivo(buffer);
        }
    } else {
        cin.getline(buffer, 150);
        cita.setMotivo(buffer);
    }
    
    return true;
}

void mostrarCitaEncontrada(const Cita& cita) {
    cita.mostrarInformacionCompleta();
}