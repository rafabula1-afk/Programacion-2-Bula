#include "operacionesDoctores.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Funciones de Gestion de Doctores
void registrarDoctor(Hospital& hospital) {
    Formatos::mostrarEncabezado("REGISTRAR NUEVO DOCTOR");
    
    Doctor nuevoDoctor;
    
    if (!pedirDatosDoctor(nuevoDoctor, hospital, false)) {
        cout << "Registro cancelado." << endl;
        return;
    }
    
    int nuevoID = hospital.generarNuevoIDDoctor();
    nuevoDoctor.setId(nuevoID);

    if (GestorArchivos::guardarRegistro(nuevoDoctor, Rutas::DOCTORES)) {
        hospital.incrementarDoctoresRegistrados();
        cout << "\nDoctor registrado exitosamente" << endl;
        cout << "ID asignado: " << nuevoID << endl;
    } else {
        cout << "\nError al registrar el doctor." << endl;
    }
}

void buscarDoctorPorID() {
    Formatos::mostrarEncabezado("BUSCAR DOCTOR POR ID");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del doctor: ", 1, 10000)) {
        return;
    }
    
    Doctor doctor;
    if (GestorArchivos::buscarRegistroPorID(id, doctor, Rutas::DOCTORES)) {
        mostrarDoctorEncontrado(doctor);
    } else {
        cout << "\nNo se encontro un doctor con ID " << id << endl;
    }
}

void listarTodosDoctores() {
    Formatos::mostrarEncabezado("LISTA DE TODOS LOS DOCTORES");
    
    int cantidad;
    Doctor* doctores = GestorArchivos::listarRegistrosActivos<Doctor>(cantidad, Rutas::DOCTORES);
    
    if (doctores == nullptr) {
        cout << "\nNo hay doctores registrados." << endl;
        return;
    }
    
    cout << "\nTotal de doctores activos: " << cantidad << endl;
    cout << "\n+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      | ESPECIALIDAD          | EXP. |   COSTO   |  PAC.  |    HORARIO     |" << endl;
    cout << "+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    
    for (int i = 0; i < cantidad; i++) {
        string nombreCompleto = string(doctores[i].getNombre()) + " " + doctores[i].getApellido();
        
        cout << "| " << setw(4) << doctores[i].getId() << " | "
             << setw(20) << left << nombreCompleto.substr(0, 20) << " | "
             << setw(21) << left << string(doctores[i].getEspecialidad()).substr(0, 21) << " | "
             << setw(4) << doctores[i].getAniosExperiencia() << " | $"
             << setw(8) << fixed << setprecision(2) << doctores[i].getCostoConsulta() << " | "
             << setw(6) << doctores[i].getCantidadPacientes() << " | "
             << setw(14) << left << string(doctores[i].getHorarioAtencion()).substr(0, 14) << " |" << endl;
    }
    
    cout << "+------+----------------------+-----------------------+------+-----------+--------+----------------+" << endl;
    
    delete[] doctores;
}

void listarDoctoresPorEspecialidad() {
    Formatos::mostrarEncabezado("BUSCAR DOCTORES POR ESPECIALIDAD");
    
    char especialidad[50];
    if (!Formatos::leerCadena(especialidad, 50, "Especialidad a buscar: ")) {
        return;
    }
    
    int cantidadTotal;
    Doctor* todosDoctores = GestorArchivos::listarRegistrosActivos<Doctor>(cantidadTotal, Rutas::DOCTORES);
    
    if (todosDoctores == nullptr) {
        cout << "\nNo hay doctores registrados." << endl;
        return;
    }

    int coincidencias = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        char especialidadLower[50];
        strcpy(especialidadLower, todosDoctores[i].getEspecialidad());
        Formatos::convertirAMinusculas(especialidadLower);
        
        char busquedaLower[50];
        strcpy(busquedaLower, especialidad);
        Formatos::convertirAMinusculas(busquedaLower);
        
        if (strstr(especialidadLower, busquedaLower) != nullptr) {
            coincidencias++;
        }
    }
    
    if (coincidencias == 0) {
        cout << "\nNo se encontraron doctores con esa especialidad." << endl;
        delete[] todosDoctores;
        return;
    }

    cout << "\n✓ Se encontraron " << coincidencias << " doctor(es):" << endl;
    cout << "\n+------+----------------------+-----------------------+------+-----------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      | ESPECIALIDAD          | EXP. |   COSTO   |" << endl;
    cout << "+------+----------------------+-----------------------+------+-----------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        char especialidadLower[50];
        strcpy(especialidadLower, todosDoctores[i].getEspecialidad());
        Formatos::convertirAMinusculas(especialidadLower);
        
        char busquedaLower[50];
        strcpy(busquedaLower, especialidad);
        Formatos::convertirAMinusculas(busquedaLower);
        
        if (strstr(especialidadLower, busquedaLower) != nullptr) {
            string nombreCompleto = string(todosDoctores[i].getNombre()) + " " + todosDoctores[i].getApellido();
            
            cout << "| " << setw(4) << todosDoctores[i].getId() << " | "
                 << setw(20) << left << nombreCompleto.substr(0, 20) << " | "
                 << setw(21) << left << string(todosDoctores[i].getEspecialidad()).substr(0, 21) << " | "
                 << setw(4) << todosDoctores[i].getAniosExperiencia() << " | $"
                 << setw(8) << fixed << setprecision(2) << todosDoctores[i].getCostoConsulta() << " |" << endl;
        }
    }
    
    cout << "+------+----------------------+-----------------------+------+-----------+" << endl;
    
    delete[] todosDoctores;
}

void modificarDoctor() {
    Formatos::mostrarEncabezado("MODIFICAR DATOS DE DOCTOR");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del doctor a modificar: ", 1, 10000)) {
        return;
    }

    Doctor doctor;
    if (!GestorArchivos::buscarRegistroPorID(id, doctor, Rutas::DOCTORES)) {
        cout << "\nNo se encontro un doctor con ID " << id << endl;
        return;
    }
    
    cout << "\nDoctor encontrado:" << endl;
    doctor.mostrarInformacionBasica();
    
    cout << "\nIngrese los nuevos datos (deje vacio para mantener el actual):" << endl;

    Doctor doctorModificado = doctor;
    Hospital hospitalTemp;
    
    if (pedirDatosDoctor(doctorModificado, hospitalTemp, true)) {
        doctorModificado.setId(id);
        doctorModificado.setFechaCreacion(doctor.getFechaCreacion());
        doctorModificado.setFechaModificacion(time(nullptr));

        int cantidad;
        Doctor* doctores = GestorArchivos::listarRegistrosActivos<Doctor>(cantidad, Rutas::DOCTORES);
        int indice = -1;
        
        if (doctores != nullptr) {
            for (int i = 0; i < cantidad; i++) {
                if (doctores[i].getId() == id) {
                    indice = i;
                    break;
                }
            }
            delete[] doctores;
        }
        
        if (indice != -1 && GestorArchivos::actualizarRegistroPorIndice(indice, doctorModificado, Rutas::DOCTORES)) {
            cout << "\nDatos del doctor actualizados correctamente." << endl;
        } else {
            cout << "\nError al actualizar los datos." << endl;
        }
    }
}

void eliminarDoctor() {
    Formatos::mostrarEncabezado("ELIMINAR DOCTOR");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del doctor a eliminar: ", 1, 10000)) {
        return;
    }

    cout << "\n¿Esta seguro que desea eliminar este doctor? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    Formatos::limpiarBuffer();
    
    if (tolower(confirmacion) != 's') {
        cout << "Eliminacion cancelada." << endl;
        return;
    }
    
    if (GestorArchivos::marcarRegistroComoEliminado<Doctor>(id, Rutas::DOCTORES)) {
        cout << "\nDoctor eliminado correctamente." << endl;
    } else {
        cout << "\nError al eliminar el doctor." << endl;
    }
}

void asignarPacienteADoctor() {
    Formatos::mostrarEncabezado("ASIGNAR PACIENTE A DOCTOR");
    
    int idDoctor, idPaciente;
    
    if (!Formatos::leerEntero(idDoctor, "ID del doctor: ", 1, 10000)) {
        return;
    }
    
    if (!Formatos::leerEntero(idPaciente, "ID del paciente: ", 1, 10000)) {
        return;
    }

    Doctor doctor;
    if (!GestorArchivos::cargarDoctorPorID(idDoctor, doctor)) {
        cout << "\nNo se encontro un doctor con ID " << idDoctor << endl;
        return;
    }

    Paciente paciente;
    if (!GestorArchivos::cargarPacientePorID(idPaciente, paciente)) {
        cout << "\nNo se encontro un paciente con ID " << idPaciente << endl;
        return;
    }
    
    cout << "\nDOCTOR ENCONTRADO:" << endl;
    cout << "Nombre: " << doctor.getNombre() << " " << doctor.getApellido() << endl;
    cout << "Especialidad: " << doctor.getEspecialidad() << endl;
    cout << "Pacientes actuales: " << doctor.getCantidadPacientes() << "/50" << endl;
    
    cout << "\nPACIENTE ENCONTRADO:" << endl;
    cout << "Nombre: " << paciente.getNombre() << " " << paciente.getApellido() << endl;
    cout << "Cedula: " << paciente.getCedula() << endl;
    cout << "Edad: " << paciente.getEdad() << " anios" << endl;

    const int* pacientesIDs = doctor.getPacientesIDs();
    bool yaAsignado = false;
    for (int i = 0; i < doctor.getCantidadPacientes(); i++) {
        if (pacientesIDs[i] == idPaciente) {
            yaAsignado = true;
            break;
        }
    }
    
    if (yaAsignado) {
        cout << "\nEste paciente ya esta asignado a este doctor." << endl;
        return;
    }

    if (doctor.getCantidadPacientes() >= 50) {
        cout << "\nEl doctor ya tiene el maximo de pacientes asignados (50)." << endl;
        return;
    }

    cout << "\n¿Asignar paciente al doctor? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    Formatos::limpiarBuffer();
    
    if (tolower(confirmacion) != 's') {
        cout << "Asignacion cancelada." << endl;
        return;
    }

    Doctor doctorModificado = doctor;
    if (doctorModificado.agregarPacienteID(idPaciente)) {
        if (GestorArchivos::actualizarDoctor(doctorModificado)) {
            cout << "\nPACIENTE ASIGNADO EXITOSAMENTE" << endl;
            cout << "================================" << endl;
            cout << "Doctor: " << doctor.getNombre() << " " << doctor.getApellido() << endl;
            cout << "Paciente: " << paciente.getNombre() << " " << paciente.getApellido() << endl;
            cout << "Total pacientes asignados ahora: " << doctorModificado.getCantidadPacientes() << endl;
        } else {
            cout << "\nError al actualizar los datos del doctor." << endl;
        }
    } else {
        cout << "\nError al asignar el paciente (limite alcanzado o error interno)." << endl;
    }
}

void listarPacientesDeDoctor() {
    Formatos::mostrarEncabezado("PACIENTES ASIGNADOS A DOCTOR");
    
    int idDoctor;
    if (!Formatos::leerEntero(idDoctor, "ID del doctor: ", 1, 10000)) {
        return;
    }

    Doctor doctor;
    if (!GestorArchivos::cargarDoctorPorID(idDoctor, doctor)) {
        cout << "\nNo se encontro un doctor con ID " << idDoctor << endl;
        return;
    }
    
    cout << "\nDOCTOR: " << doctor.getNombre() << " " << doctor.getApellido() << endl;
    cout << "Especialidad: " << doctor.getEspecialidad() << endl;
    cout << "Pacientes asignados: " << doctor.getCantidadPacientes() << endl;
    cout << "Disponible: " << (doctor.estaDisponible() ? "Si" : "No") << endl;
    
    if (doctor.getCantidadPacientes() == 0) {
        cout << "\nEste doctor no tiene pacientes asignados." << endl;
        return;
    }
    
    cout << "\nLISTA DE PACIENTES ASIGNADOS:" << endl;
    cout << "\n+------+----------------------+--------------+------+----------------------+----------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      |   CEDULA     | EDAD |   TELEFONO           | ULTIMA CONSULTA|" << endl;
    cout << "+------+----------------------+--------------+------+----------------------+----------------+" << endl;
    
    const int* pacientesIDs = doctor.getPacientesIDs();
    int totalCitas = 0;
    
    for (int i = 0; i < doctor.getCantidadPacientes(); i++) {
        int idPaciente = pacientesIDs[i];
        Paciente paciente;
        
        if (GestorArchivos::cargarPacientePorID(idPaciente, paciente)) {
            string nombreCompleto = string(paciente.getNombre()) + " " + paciente.getApellido();

            string ultimaConsulta = "Ninguna";
            int cantidadConsultas;
            HistorialMedico* consultas = GestorArchivos::cargarHistorialPorPaciente(idPaciente, cantidadConsultas);
            
            if (consultas != nullptr && cantidadConsultas > 0) {
                int indiceMasReciente = 0;
                for (int j = 1; j < cantidadConsultas; j++) {
                    if (strcmp(consultas[j].getFecha(), consultas[indiceMasReciente].getFecha()) > 0 ||
                        (strcmp(consultas[j].getFecha(), consultas[indiceMasReciente].getFecha()) == 0 &&
                         strcmp(consultas[j].getHora(), consultas[indiceMasReciente].getHora()) > 0)) {
                        indiceMasReciente = j;
                    }
                }
                ultimaConsulta = string(consultas[indiceMasReciente].getFecha()) + " " + consultas[indiceMasReciente].getHora();
                delete[] consultas;
            }
            
            cout << "| " << setw(4) << paciente.getId() << " | "
                 << setw(20) << left << nombreCompleto.substr(0, 20) << " | "
                 << setw(12) << paciente.getCedula() << " | "
                 << setw(4) << paciente.getEdad() << " | "
                 << setw(20) << paciente.getTelefono() << " | "
                 << setw(14) << ultimaConsulta << " |" << endl;
        }
    }
    
    cout << "+------+----------------------+--------------+------+----------------------+----------------+" << endl;

    cout << "\nESTADISTICAS:" << endl;
    cout << "• Total pacientes: " << doctor.getCantidadPacientes() << endl;
    cout << "• Capacidad disponible: " << (50 - doctor.getCantidadPacientes()) << "/50" << endl;

    cout << "\n¿Desea ver los detalles de algun paciente? (s/n): ";
    char opcion;
    cin >> opcion;
    Formatos::limpiarBuffer();
    
    if (tolower(opcion) == 's') {
        int idPaciente;
        if (Formatos::leerEntero(idPaciente, "ID del paciente: ", 1, 10000)) {
            bool pacienteAsignado = false;
            for (int i = 0; i < doctor.getCantidadPacientes(); i++) {
                if (pacientesIDs[i] == idPaciente) {
                    pacienteAsignado = true;
                    break;
                }
            }
            
            if (pacienteAsignado) {
                Paciente paciente;
                if (GestorArchivos::cargarPacientePorID(idPaciente, paciente)) {
                    cout << "\n=== DETALLES DEL PACIENTE ===" << endl;
                    paciente.mostrarInformacionCompleta();
                }
            } else {
                cout << "El paciente con ID " << idPaciente << " no esta asignado a este doctor." << endl;
            }
        }
    }
}

//Funciones auxiliares
void mostrarMenuDoctores(Hospital& hospital) {
    int opcion;
    
    do {
        Formatos::mostrarEncabezado("GESTION DE DOCTORES");
        
        cout << "1. Registrar nuevo doctor" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Listar todos" << endl;
        cout << "4. Listar por especialidad" << endl;
        cout << "5. Modificar datos" << endl;
        cout << "6. Eliminar" << endl;
        cout << "7. Asignar paciente" << endl;
        cout << "8. Ver pacientes asignados" << endl;
        cout << "9. Volver al menu principal" << endl;
        cout << "\nOpcion: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: registrarDoctor(hospital); break;
            case 2: buscarDoctorPorID(); break;
            case 3: listarTodosDoctores(); break;
            case 4: listarDoctoresPorEspecialidad(); break;
            case 5: modificarDoctor(); break;
            case 6: eliminarDoctor(); break;
            case 7: asignarPacienteADoctor(); break;
            case 8: listarPacientesDeDoctor(); break;
            case 9: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opcion inválida." << endl;
        }
        
        if (opcion != 9) {
            Formatos::pausar();
        }
        
    } while (opcion != 9);
}

bool pedirDatosDoctor(Doctor& doctor, Hospital& hospital, bool esModificacion) {
    char buffer[100];
    
    do {
        cout << "Nombre" << (esModificacion ? " (actual: " + string(doctor.getNombre()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 50);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 50);
        }
        
        if (!Validaciones::validarSoloLetras(buffer) && (!esModificacion || strlen(buffer) > 0)) {
            cout << "Error: El nombre solo puede contener letras y espacios." << endl;
            if (!esModificacion) continue;
        }
        
        if (strlen(buffer) > 0) {
            doctor.setNombre(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);

    do {
        cout << "Apellido" << (esModificacion ? " (actual: " + string(doctor.getApellido()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 50);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 50);
        }
        
        if (!Validaciones::validarSoloLetras(buffer) && (!esModificacion || strlen(buffer) > 0)) {
            cout << "Error: El apellido solo puede contener letras y espacios." << endl;
            if (!esModificacion) continue;
        }
        
        if (strlen(buffer) > 0) {
            doctor.setApellido(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);
    
    do {
        cout << "Cedula profesional" << (esModificacion ? " (actual: " + string(doctor.getCedulaProfesional()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 20);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 20);
        }
        
        if (!Validaciones::validarCedulaProfesional(buffer) && (!esModificacion || strlen(buffer) > 0)) {
            cout << "Error: Cedula profesional invalida." << endl;
            if (!esModificacion) continue;
        }
        
        if (strlen(buffer) > 0) {
            doctor.setCedulaProfesional(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);

    do {
        cout << "Especialidad" << (esModificacion ? " (actual: " + string(doctor.getEspecialidad()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 50);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 50);
        }
        
        if (strlen(buffer) > 0) {
            if (!Validaciones::validarEspecialidad(buffer)) {
                cout << "Advertencia: Especialidad no reconocida. Se registrara de todas formas." << endl;
            }
            doctor.setEspecialidad(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);

    do {
        cout << "Anios de experiencia" << (esModificacion ? " (actual: " + to_string(doctor.getAniosExperiencia()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 10);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 10);
        }
        
        if (strlen(buffer) > 0) {
            int anios = atoi(buffer);
            if (!Validaciones::validarAniosExperiencia(anios)) {
                cout << "Error: Anios de experiencia invalidos." << endl;
                if (!esModificacion) continue;
            } else {
                doctor.setAniosExperiencia(anios);
            }
        }
    } while (!esModificacion && doctor.getAniosExperiencia() == 0);

    do {
        cout << "Costo de consulta" << (esModificacion ? " (actual: $" + to_string(doctor.getCostoConsulta()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 20);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 20);
        }
        
        if (strlen(buffer) > 0) {
            float costo = atof(buffer);
            if (!Validaciones::validarCosto(costo)) {
                cout << "Error: Costo invalido." << endl;
                if (!esModificacion) continue;
            } else {
                doctor.setCostoConsulta(costo);
            }
        }
    } while (!esModificacion && doctor.getCostoConsulta() == 0.0f);

    cout << "Horario de atencion" << (esModificacion ? " (actual: " + string(doctor.getHorarioAtencion()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 50);
        if (strlen(buffer) > 0) {
            doctor.setHorarioAtencion(buffer);
        }
    } else {
        cin.getline(buffer, 50);
        doctor.setHorarioAtencion(buffer);
    }

    cout << "Telefono" << (esModificacion ? " (actual: " + string(doctor.getTelefono()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 15);
        if (strlen(buffer) > 0) {
            if (Validaciones::validarTelefono(buffer)) {
                doctor.setTelefono(buffer);
            } else {
                cout << "Error: Teléfono invalido." << endl;
            }
        }
    } else {
        cin.getline(buffer, 15);
        if (Validaciones::validarTelefono(buffer)) {
            doctor.setTelefono(buffer);
        } else {
            cout << "Error: Teléfono invalido." << endl;
        }
    }

    cout << "Email" << (esModificacion ? " (actual: " + string(doctor.getEmail()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 50);
        if (strlen(buffer) > 0) {
            if (Validaciones::validarEmail(buffer)) {
                doctor.setEmail(buffer);
            } else {
                cout << "Error: Email invalido." << endl;
            }
        }
    } else {
        cin.getline(buffer, 50);
        if (Validaciones::validarEmail(buffer)) {
            doctor.setEmail(buffer);
        } else {
            cout << "Error: Email invalido." << endl;
        }
    }
    
    return true;
}

void mostrarDoctorEncontrado(const Doctor& doctor) {
    doctor.mostrarInformacionCompleta();
}