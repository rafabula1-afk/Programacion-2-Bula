#include "operacionesPacientes.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

//Funciones de Gestion de Pacientes
void registrarPaciente(Hospital& hospital) {
    Formatos::mostrarEncabezado("REGISTRAR NUEVO PACIENTE");
    
    Paciente nuevoPaciente;
    
    if (!pedirDatosPaciente(nuevoPaciente, hospital, false)) {
        cout << "Registro cancelado." << endl;
        return;
    }
    
    int nuevoID = hospital.generarNuevoIDPaciente();
    nuevoPaciente.setId(nuevoID);
    
    if (GestorArchivos::guardarRegistro(nuevoPaciente, Rutas::PACIENTES)) {
        hospital.incrementarPacientesRegistrados();
        cout << "\nPaciente registrado exitosamente" << endl;
        cout << "ID asignado: " << nuevoID << endl;
    } else {
        cout << "\nError al registrar el paciente." << endl;
    }
}

void buscarPacientePorID() {
    Formatos::mostrarEncabezado("BUSCAR PACIENTE POR ID");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del paciente: ", 1, 10000)) {
        return;
    }
    
    Paciente paciente;
    if (GestorArchivos::buscarRegistroPorID(id, paciente, Rutas::PACIENTES)) {
        mostrarPacienteEncontrado(paciente);
    } else {
        cout << "\nNo se encontro un paciente con ID " << id << endl;
    }
}

void buscarPacientePorCedula() {
    Formatos::mostrarEncabezado("BUSCAR PACIENTE POR CEDULA");
    
    char cedula[20];
    if (!Formatos::leerCadena(cedula, 20, "Cedula del paciente: ")) {
        return;
    }
    
    int cantidad;
    Paciente* pacientes = GestorArchivos::listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
    
    if (pacientes == nullptr) {
        cout << "\nNo hay pacientes registrados." << endl;
        return;
    }
    
    bool encontrado = false;
    for (int i = 0; i < cantidad; i++) {
        if (Validaciones::compararCadenasCaseInsensitive(pacientes[i].getCedula(), cedula)) {
            mostrarPacienteEncontrado(pacientes[i]);
            encontrado = true;
            break;
        }
    }
    
    delete[] pacientes;
    
    if (!encontrado) {
        cout << "\nNo se encontro un paciente con cedula " << cedula << endl;
    }
}

void buscarPacientesPorNombre() {
    Formatos::mostrarEncabezado("BUSCAR PACIENTES POR NOMBRE");
    
    char nombre[100];
    if (!Formatos::leerCadena(nombre, 100, "Nombre o apellido a buscar: ")) {
        return;
    }
    
    int cantidadTotal;
    Paciente* todosPacientes = GestorArchivos::listarRegistrosActivos<Paciente>(cantidadTotal, Rutas::PACIENTES);
    
    if (todosPacientes == nullptr) {
        cout << "\nNo hay pacientes registrados." << endl;
        return;
    }
    
    int coincidencias = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        string nombreCompleto = string(todosPacientes[i].getNombre()) + " " + todosPacientes[i].getApellido();
        Formatos::convertirAMinusculas(const_cast<char*>(nombreCompleto.c_str()));
        
        char nombreBusqueda[100];
        strcpy(nombreBusqueda, nombre);
        Formatos::convertirAMinusculas(nombreBusqueda);
        
        if (strstr(nombreCompleto.c_str(), nombreBusqueda) != nullptr) {
            coincidencias++;
        }
    }
    
    if (coincidencias == 0) {
        cout << "\nNo se encontraron pacientes con ese nombre." << endl;
        delete[] todosPacientes;
        return;
    }
    
    cout << "\nSe encontraron " << coincidencias << " paciente(s):" << endl;
    cout << "\n+------+----------------------+--------------+------+----------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      |   CEDULA     | EDAD |  TELEFONO |" << endl;
    cout << "+------+----------------------+--------------+------+----------+" << endl;
    
    for (int i = 0; i < cantidadTotal; i++) {
        string nombreCompleto = string(todosPacientes[i].getNombre()) + " " + todosPacientes[i].getApellido();
        string nombreCompletoLower = nombreCompleto;
        Formatos::convertirAMinusculas(const_cast<char*>(nombreCompletoLower.c_str()));
        
        char nombreBusqueda[100];
        strcpy(nombreBusqueda, nombre);
        Formatos::convertirAMinusculas(nombreBusqueda);
        
        if (strstr(nombreCompletoLower.c_str(), nombreBusqueda) != nullptr) {
            cout << "| " << setw(4) << todosPacientes[i].getId() << " | "
                 << setw(20) << left << nombreCompleto.substr(0, 20) << " | "
                 << setw(12) << todosPacientes[i].getCedula() << " | "
                 << setw(4) << todosPacientes[i].getEdad() << " | "
                 << setw(10) << todosPacientes[i].getTelefono() << " |" << endl;
        }
    }
    
    cout << "+------+----------------------+--------------+------+----------+" << endl;
    
    delete[] todosPacientes;
}

void modificarPaciente() {
    Formatos::mostrarEncabezado("MODIFICAR DATOS DE PACIENTE");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del paciente a modificar: ", 1, 10000)) {
        return;
    }
    
    Paciente paciente;
    if (!GestorArchivos::buscarRegistroPorID(id, paciente, Rutas::PACIENTES)) {
        cout << "\n✗ No se encontro un paciente con ID " << id << endl;
        return;
    }
    
    cout << "\nPaciente encontrado:" << endl;
    paciente.mostrarInformacionBasica();
    
    cout << "\nIngrese los nuevos datos (deje vacio para mantener el actual):" << endl;
    
    Paciente pacienteModificado = paciente;
    Hospital hospitalTemp;
    
    if (pedirDatosPaciente(pacienteModificado, hospitalTemp, true)) {
        pacienteModificado.setId(id);
        pacienteModificado.setFechaCreacion(paciente.getFechaCreacion());
        pacienteModificado.setFechaModificacion(time(nullptr));
        
        int cantidad;
        Paciente* pacientes = GestorArchivos::listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
        int indice = -1;
        
        if (pacientes != nullptr) {
            for (int i = 0; i < cantidad; i++) {
                if (pacientes[i].getId() == id) {
                    indice = i;
                    break;
                }
            }
            delete[] pacientes;
        }
        
        if (indice != -1 && GestorArchivos::actualizarRegistroPorIndice(indice, pacienteModificado, Rutas::PACIENTES)) {
            cout << "\nDatos del paciente actualizados correctamente." << endl;
        } else {
            cout << "\nError al actualizar los datos." << endl;
        }
    }
}

void eliminarPaciente() {
    Formatos::mostrarEncabezado("ELIMINAR PACIENTE");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del paciente a eliminar: ", 1, 10000)) {
        return;
    }
    
    // Confirmar eliminación
    cout << "\n¿Está seguro que desea eliminar este paciente? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    Formatos::limpiarBuffer();
    
    if (tolower(confirmacion) != 's') {
        cout << "Eliminacion cancelada." << endl;
        return;
    }
    
    if (GestorArchivos::marcarRegistroComoEliminado<Paciente>(id, Rutas::PACIENTES)) {
        cout << "\nPaciente eliminado correctamente." << endl;
    } else {
        cout << "\nError al eliminar el paciente." << endl;
    }
}

void listarTodosPacientes() {
    Formatos::mostrarEncabezado("LISTA DE TODOS LOS PACIENTES");
    
    int cantidad;
    Paciente* pacientes = GestorArchivos::listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
    
    if (pacientes == nullptr) {
        cout << "\nNo hay pacientes registrados." << endl;
        return;
    }
    
    cout << "\nTotal de pacientes activos: " << cantidad << endl;
    cout << "\n+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    cout << "|  ID  | NOMBRE COMPLETO      |   CEDULA     | EDAD |  TELEFONO |         EMAIL          |" << endl;
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    
    for (int i = 0; i < cantidad; i++) {
        string nombreCompleto = string(pacientes[i].getNombre()) + " " + pacientes[i].getApellido();
        
        cout << "| " << setw(4) << pacientes[i].getId() << " | "
             << setw(20) << left << nombreCompleto.substr(0, 20) << " | "
             << setw(12) << pacientes[i].getCedula() << " | "
             << setw(4) << pacientes[i].getEdad() << " | "
             << setw(10) << pacientes[i].getTelefono() << " | "
             << setw(23) << left << string(pacientes[i].getEmail()).substr(0, 23) << " |" << endl;
    }
    
    cout << "+------+----------------------+--------------+------+----------+-------------------------+" << endl;
    
    delete[] pacientes;
}

void verHistorialPaciente() {
    Formatos::mostrarEncabezado("HISTORIAL MEDICO DEL PACIENTE");
    
    int id;
    if (!Formatos::leerEntero(id, "ID del paciente: ", 1, 10000)) {
        return;
    }
    
    Paciente paciente;
    if (!GestorArchivos::cargarPacientePorID(id, paciente)) {
        cout << "No se encontro un paciente con ID " << id << endl;
        return;
    }
    
    cout << "\nPACIENTE ENCONTRADO:" << endl;
    cout << "Nombre: " << paciente.getNombre() << " " << paciente.getApellido() << endl;
    cout << "Cedula: " << paciente.getCedula() << endl;
    cout << "Edad: " << paciente.getEdad() << " anios" << endl;
    cout << "Tipo de sangre: " << paciente.getTipoSangre() << endl;
    
    // Obtener historial del paciente
    int cantidadConsultas;
    HistorialMedico* consultas = GestorArchivos::cargarHistorialPorPaciente(id, cantidadConsultas);
    
    if (consultas == nullptr || cantidadConsultas == 0) {
        cout << "\nEste paciente no tiene consultas registradas en el historial." << endl;
        return;
    }
    
    cout << "\nHISTORIAL MÉDICO (" << cantidadConsultas << " consultas):" << endl;
    cout << "\n+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    cout << "|  ID  |   FECHA    | HORA  | DOC. |                   DIAGNOSTICO                    |       COSTO      |" << endl;
    cout << "+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    
    float costoTotal = 0.0f;
    for (int i = 0; i < cantidadConsultas; i++) {
        cout << "| " << setw(4) << consultas[i].getId() << " | "
             << setw(10) << consultas[i].getFecha() << " | "
             << setw(5) << consultas[i].getHora() << " | "
             << setw(4) << consultas[i].getDoctorID() << " | "
             << setw(48) << left << string(consultas[i].getDiagnostico()).substr(0, 48) << " | $"
             << setw(14) << fixed << setprecision(2) << consultas[i].getCosto() << " |" << endl;
        
        costoTotal += consultas[i].getCosto();
    }
    
    cout << "+------+------------+-------+------+--------------------------------------------------+------------------+" << endl;
    cout << "                                                                        TOTAL: $" << fixed << setprecision(2) << costoTotal << endl;
    
    cout << "\n¿Desea ver el detalle de alguna consulta? (s/n): ";
    char opcion;
    cin >> opcion;
    Formatos::limpiarBuffer();
    
    if (tolower(opcion) == 's') {
        int idConsulta;
        if (Formatos::leerEntero(idConsulta, "ID de la consulta: ", 1, 10000)) {
            bool encontrada = false;
            for (int i = 0; i < cantidadConsultas; i++) {
                if (consultas[i].getId() == idConsulta) {
                    cout << "\n=== DETALLE DE CONSULTA ===" << endl;
                    consultas[i].mostrarInformacionCompleta();
                    encontrada = true;
                    break;
                }
            }
            if (!encontrada) {
                cout << "No se encontro la consulta con ID " << idConsulta << " para este paciente." << endl;
            }
        }
    }
    
    delete[] consultas;
}

//Funciones Auxiliares
void mostrarMenuPacientes(Hospital& hospital) {
    int opcion;
    
    do {
        Formatos::mostrarEncabezado("GESTION DE PACIENTES");
        
        cout << "1. Registrar nuevo paciente" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Buscar por cedula" << endl;
        cout << "4. Buscar por nombre" << endl;
        cout << "5. Modificar datos" << endl;
        cout << "6. Eliminar" << endl;
        cout << "7. Listar todos" << endl;
        cout << "8. Ver historial medico" << endl;
        cout << "9. Volver al menu principal" << endl;
        cout << "\nOpcion: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: registrarPaciente(hospital); break;
            case 2: buscarPacientePorID(); break;
            case 3: buscarPacientePorCedula(); break;
            case 4: buscarPacientesPorNombre(); break;
            case 5: modificarPaciente(); break;
            case 6: eliminarPaciente(); break;
            case 7: listarTodosPacientes(); break;
            case 8: verHistorialPaciente(); break;
            case 9: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opción invalida." << endl;
        }
        
        if (opcion != 9) {
            Formatos::pausar();
        }
        
    } while (opcion != 9);
}

bool pedirDatosPaciente(Paciente& paciente, Hospital& hospital, bool esModificacion) {
    char buffer[500];
    
    do {
        cout << "Nombre" << (esModificacion ? " (actual: " + string(paciente.getNombre()) + ")" : "") << ": ";
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
            paciente.setNombre(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);
    
    do {
        cout << "Apellido" << (esModificacion ? " (actual: " + string(paciente.getApellido()) + ")" : "") << ": ";
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
            paciente.setApellido(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);
    
    do {
        cout << "Cedula" << (esModificacion ? " (actual: " + string(paciente.getCedula()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 20);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 20);
        }
        
        if (!Validaciones::validarCedula(buffer) && (!esModificacion || strlen(buffer) > 0)) {
            cout << "Error: Cedula invalida." << endl;
            if (!esModificacion) continue;
        }
        
        if (strlen(buffer) > 0) {
            paciente.setCedula(buffer);
        }
    } while (!esModificacion && strlen(buffer) == 0);
    
    do {
        cout << "Edad" << (esModificacion ? " (actual: " + to_string(paciente.getEdad()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 10);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 10);
        }
        
        if (strlen(buffer) > 0) {
            int edad = atoi(buffer);
            if (!Validaciones::validarEdad(edad)) {
                cout << "Error: Edad invalida. Debe estar entre 0 y 120 anios." << endl;
                if (!esModificacion) continue;
            } else {
                paciente.setEdad(edad);
            }
        }
    } while (!esModificacion && paciente.getEdad() == 0);
    
    do {
        cout << "Sexo (M/F)" << (esModificacion ? " (actual: " + string(1, paciente.getSexo()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 2);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 2);
        }
        
        if (strlen(buffer) > 0) {
            char sexo = toupper(buffer[0]);
            if (sexo != 'M' && sexo != 'F') {
                cout << "Error: Sexo invalido. Use M o F." << endl;
                if (!esModificacion) continue;
            } else {
                paciente.setSexo(sexo);
            }
        }
    } while (!esModificacion && paciente.getSexo() == ' ');
    
    do {
        cout << "Tipo de sangre (O+, O-, A+, A-, B+, B-, AB+, AB-)" 
             << (esModificacion ? " (actual: " + string(paciente.getTipoSangre()) + ")" : "") << ": ";
        if (esModificacion) {
            cin.getline(buffer, 5);
            if (strlen(buffer) == 0) {
                break;
            }
        } else {
            cin.getline(buffer, 5);
        }
        
        if (strlen(buffer) > 0) {
            if (!Validaciones::validarTipoSangre(buffer)) {
                cout << "Error: Tipo de sangre invalido." << endl;
                if (!esModificacion) continue;
            } else {
                paciente.setTipoSangre(buffer);
            }
        }
    } while (!esModificacion && strlen(paciente.getTipoSangre()) == 0);
    
    cout << "Telefono" << (esModificacion ? " (actual: " + string(paciente.getTelefono()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 15);
        if (strlen(buffer) > 0) {
            if (Validaciones::validarTelefono(buffer)) {
                paciente.setTelefono(buffer);
            } else {
                cout << "Error: Telefono invalido." << endl;
            }
        }
    } else {
        cin.getline(buffer, 15);
        if (Validaciones::validarTelefono(buffer)) {
            paciente.setTelefono(buffer);
        } else {
            cout << "Error: Teléfono invalido." << endl;
        }
    }
    
    cout << "Direccion" << (esModificacion ? " (actual: " + string(paciente.getDireccion()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 100);
        if (strlen(buffer) > 0) {
            paciente.setDireccion(buffer);
        }
    } else {
        cin.getline(buffer, 100);
        paciente.setDireccion(buffer);
    }
    
    cout << "Email" << (esModificacion ? " (actual: " + string(paciente.getEmail()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 50);
        if (strlen(buffer) > 0) {
            if (Validaciones::validarEmail(buffer)) {
                paciente.setEmail(buffer);
            } else {
                cout << "Error: Email invalido." << endl;
            }
        }
    } else {
        cin.getline(buffer, 50);
        if (Validaciones::validarEmail(buffer)) {
            paciente.setEmail(buffer);
        } else {
            cout << "Error: Email invalido." << endl;
        }
    }
    
    cout << "Alergias" << (esModificacion ? " (actual: " + string(paciente.getAlergias()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 500);
        if (strlen(buffer) > 0) {
            paciente.setAlergias(buffer);
        }
    } else {
        cin.getline(buffer, 500);
        paciente.setAlergias(buffer);
    }
    
    cout << "Observaciones" << (esModificacion ? " (actual: " + string(paciente.getObservaciones()) + ")" : "") << ": ";
    if (esModificacion) {
        cin.getline(buffer, 500);
        if (strlen(buffer) > 0) {
            paciente.setObservaciones(buffer);
        }
    } else {
        cin.getline(buffer, 500);
        paciente.setObservaciones(buffer);
    }
    
    return true;
}

void mostrarPacienteEncontrado(const Paciente& paciente) {
    paciente.mostrarInformacionCompleta();
}