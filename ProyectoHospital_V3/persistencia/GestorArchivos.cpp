#include "GestorArchivos.hpp"
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#endif

using namespace std;

//Metodos Privados
bool GestorArchivos::crearDirectorioSiNoExiste(const char* ruta) {
    struct stat info;
    if (stat(ruta, &info) != 0) {
        return mkdir(ruta, 0777) == 0;
    } else if (info.st_mode & S_IFDIR) {
        return true;
    }
    return false;
}

//Metodos para Hospital
bool GestorArchivos::guardarHospital(const Hospital& hospital) {
    ofstream archivo(Rutas::HOSPITAL, ios::binary);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir " << Rutas::HOSPITAL << endl;
        return false;
    }
    
    const char* bytes = reinterpret_cast<const char*>(&hospital);
    archivo.write(bytes, sizeof(Hospital));
    
    bool exito = !archivo.fail();
    archivo.close();
    
    if (exito) {
        cout << "✓ Hospital guardado correctamente." << endl;
    } else {
        cerr << "✗ Error al guardar hospital." << endl;
    }
    
    return exito;
}

bool GestorArchivos::cargarHospital(Hospital& hospital) {
    ifstream archivo(Rutas::HOSPITAL, ios::binary);
    if (!archivo.is_open()) {
        hospital = Hospital("Hospital General", "Calle Principal #123", "555-1234");
        return guardarHospital(hospital);
    }
    
    char* bytes = reinterpret_cast<char*>(&hospital);
    archivo.read(bytes, sizeof(Hospital));
    
    bool exito = !archivo.fail();
    archivo.close();
    
    if (exito) {
        cout << "✓ Hospital cargado correctamente." << endl;
    } else {
        cerr << "✗ Error al cargar hospital." << endl;
    }
    
    return exito;
}

//Metodos para Pacientes
bool GestorArchivos::guardarPaciente(const Paciente& paciente) {
    return guardarRegistro(paciente, Rutas::PACIENTES);
}

bool GestorArchivos::cargarPacientePorID(int id, Paciente& paciente) {
    return buscarRegistroPorID(id, paciente, Rutas::PACIENTES);
}

bool GestorArchivos::actualizarPaciente(const Paciente& paciente) {
    int cantidad;
    Paciente* pacientes = listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
    
    if (pacientes == nullptr) {
        return false;
    }
    
    int indice = -1;
    for (int i = 0; i < cantidad; i++) {
        if (pacientes[i].getId() == paciente.getId()) {
            indice = i;
            break;
        }
    }
    
    delete[] pacientes;
    
    if (indice == -1) {
        return false;
    }
    
    return actualizarRegistroPorIndice(indice, paciente, Rutas::PACIENTES);
}

bool GestorArchivos::eliminarPacienteLogico(int id) {
    return marcarRegistroComoEliminado<Paciente>(id, Rutas::PACIENTES);
}

Paciente* GestorArchivos::listarPacientesActivos(int& cantidad) {
    return listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
}

Paciente* GestorArchivos::buscarPacientePorCedula(const char* cedula) {
    int cantidad;
    Paciente* pacientes = listarRegistrosActivos<Paciente>(cantidad, Rutas::PACIENTES);
    
    if (pacientes == nullptr) {
        return nullptr;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (strcasecmp(pacientes[i].getCedula(), cedula) == 0) {
            Paciente* resultado = new Paciente(pacientes[i]);
            delete[] pacientes;
            return resultado;
        }
    }
    
    delete[] pacientes;
    return nullptr;
}

Paciente* GestorArchivos::buscarPacientesPorNombre(const char* nombre, int& cantidadResultados) {
    int cantidadTotal;
    Paciente* todosPacientes = listarRegistrosActivos<Paciente>(cantidadTotal, Rutas::PACIENTES);
    
    if (todosPacientes == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    char nombreBusqueda[100];
    strcpy(nombreBusqueda, nombre);
    for (int i = 0; nombreBusqueda[i]; i++) {
        nombreBusqueda[i] = tolower(nombreBusqueda[i]);
    }
    
    for (int i = 0; i < cantidadTotal; i++) {
        string nombreCompleto = string(todosPacientes[i].getNombre()) + " " + todosPacientes[i].getApellido();
        for (size_t j = 0; j < nombreCompleto.length(); j++) {
            nombreCompleto[j] = tolower(nombreCompleto[j]);
        }
        
        if (nombreCompleto.find(nombreBusqueda) != string::npos) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todosPacientes;
        return nullptr;
    }

    Paciente* resultados = new Paciente[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        string nombreCompleto = string(todosPacientes[i].getNombre()) + " " + todosPacientes[i].getApellido();
        for (size_t j = 0; j < nombreCompleto.length(); j++) {
            nombreCompleto[j] = tolower(nombreCompleto[j]);
        }
        
        if (nombreCompleto.find(nombreBusqueda) != string::npos) {
            resultados[indice] = todosPacientes[i];
            indice++;
        }
    }
    
    delete[] todosPacientes;
    return resultados;
}

//Metodos para Doctores
bool GestorArchivos::guardarDoctor(const Doctor& doctor) {
    return guardarRegistro(doctor, Rutas::DOCTORES);
}

bool GestorArchivos::cargarDoctorPorID(int id, Doctor& doctor) {
    return buscarRegistroPorID(id, doctor, Rutas::DOCTORES);
}

bool GestorArchivos::actualizarDoctor(const Doctor& doctor) {
    int cantidad;
    Doctor* doctores = listarRegistrosActivos<Doctor>(cantidad, Rutas::DOCTORES);
    
    if (doctores == nullptr) {
        return false;
    }
    
    int indice = -1;
    for (int i = 0; i < cantidad; i++) {
        if (doctores[i].getId() == doctor.getId()) {
            indice = i;
            break;
        }
    }
    
    delete[] doctores;
    
    if (indice == -1) {
        return false;
    }
    
    return actualizarRegistroPorIndice(indice, doctor, Rutas::DOCTORES);
}

bool GestorArchivos::eliminarDoctorLogico(int id) {
    return marcarRegistroComoEliminado<Doctor>(id, Rutas::DOCTORES);
}

Doctor* GestorArchivos::listarDoctoresActivos(int& cantidad) {
    return listarRegistrosActivos<Doctor>(cantidad, Rutas::DOCTORES);
}

Doctor* GestorArchivos::buscarDoctoresPorEspecialidad(const char* especialidad, int& cantidadResultados) {
    int cantidadTotal;
    Doctor* todosDoctores = listarRegistrosActivos<Doctor>(cantidadTotal, Rutas::DOCTORES);
    
    if (todosDoctores == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    char especialidadBusqueda[50];
    strcpy(especialidadBusqueda, especialidad);
    for (int i = 0; especialidadBusqueda[i]; i++) {
        especialidadBusqueda[i] = tolower(especialidadBusqueda[i]);
    }
    
    for (int i = 0; i < cantidadTotal; i++) {
        char especialidadDoctor[50];
        strcpy(especialidadDoctor, todosDoctores[i].getEspecialidad());
        for (int j = 0; especialidadDoctor[j]; j++) {
            especialidadDoctor[j] = tolower(especialidadDoctor[j]);
        }
        
        if (strstr(especialidadDoctor, especialidadBusqueda) != nullptr) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todosDoctores;
        return nullptr;
    }

    Doctor* resultados = new Doctor[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        char especialidadDoctor[50];
        strcpy(especialidadDoctor, todosDoctores[i].getEspecialidad());
        for (int j = 0; especialidadDoctor[j]; j++) {
            especialidadDoctor[j] = tolower(especialidadDoctor[j]);
        }
        
        if (strstr(especialidadDoctor, especialidadBusqueda) != nullptr) {
            resultados[indice] = todosDoctores[i];
            indice++;
        }
    }
    
    delete[] todosDoctores;
    return resultados;
}

//Metodos para Citas
bool GestorArchivos::guardarCita(const Cita& cita) {
    return guardarRegistro(cita, Rutas::CITAS);
}

bool GestorArchivos::cargarCitaPorID(int id, Cita& cita) {
    return buscarRegistroPorID(id, cita, Rutas::CITAS);
}

bool GestorArchivos::actualizarCita(const Cita& cita) {
    int cantidad;
    Cita* citas = listarRegistrosActivos<Cita>(cantidad, Rutas::CITAS);
    
    if (citas == nullptr) {
        return false;
    }
    
    int indice = -1;
    for (int i = 0; i < cantidad; i++) {
        if (citas[i].getId() == cita.getId()) {
            indice = i;
            break;
        }
    }
    
    delete[] citas;
    
    if (indice == -1) {
        return false;
    }
    
    return actualizarRegistroPorIndice(indice, cita, Rutas::CITAS);
}

bool GestorArchivos::eliminarCitaLogico(int id) {
    return marcarRegistroComoEliminado<Cita>(id, Rutas::CITAS);
}

Cita* GestorArchivos::listarCitasActivas(int& cantidad) {
    return listarRegistrosActivos<Cita>(cantidad, Rutas::CITAS);
}

Cita* GestorArchivos::buscarCitasPorPaciente(int idPaciente, int& cantidadResultados) {
    int cantidadTotal;
    Cita* todasCitas = listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getPacienteID() == idPaciente) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todasCitas;
        return nullptr;
    }

    Cita* resultados = new Cita[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getPacienteID() == idPaciente) {
            resultados[indice] = todasCitas[i];
            indice++;
        }
    }
    
    delete[] todasCitas;
    return resultados;
}

Cita* GestorArchivos::buscarCitasPorDoctor(int idDoctor, int& cantidadResultados) {
    int cantidadTotal;
    Cita* todasCitas = listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getDoctorID() == idDoctor) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todasCitas;
        return nullptr;
    }

    Cita* resultados = new Cita[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasCitas[i].getDoctorID() == idDoctor) {
            resultados[indice] = todasCitas[i];
            indice++;
        }
    }
    
    delete[] todasCitas;
    return resultados;
}

Cita* GestorArchivos::buscarCitasPorFecha(const char* fecha, int& cantidadResultados) {
    int cantidadTotal;
    Cita* todasCitas = listarRegistrosActivos<Cita>(cantidadTotal, Rutas::CITAS);
    
    if (todasCitas == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getFecha(), fecha) == 0) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todasCitas;
        return nullptr;
    }

    Cita* resultados = new Cita[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (strcmp(todasCitas[i].getFecha(), fecha) == 0) {
            resultados[indice] = todasCitas[i];
            indice++;
        }
    }
    
    delete[] todasCitas;
    return resultados;
}

//Metodos para Historiales Medicos
bool GestorArchivos::guardarConsulta(const HistorialMedico& consulta) {
    return guardarRegistro(consulta, Rutas::HISTORIALES);
}

HistorialMedico* GestorArchivos::cargarHistorialPorPaciente(int idPaciente, int& cantidadResultados) {
    int cantidadTotal;
    HistorialMedico* todasConsultas = listarRegistrosActivos<HistorialMedico>(cantidadTotal, Rutas::HISTORIALES);
    
    if (todasConsultas == nullptr) {
        cantidadResultados = 0;
        return nullptr;
    }

    cantidadResultados = 0;
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasConsultas[i].getPacienteID() == idPaciente) {
            cantidadResultados++;
        }
    }
    
    if (cantidadResultados == 0) {
        delete[] todasConsultas;
        return nullptr;
    }

    HistorialMedico* resultados = new HistorialMedico[cantidadResultados];
    int indice = 0;
    
    for (int i = 0; i < cantidadTotal; i++) {
        if (todasConsultas[i].getPacienteID() == idPaciente) {
            resultados[indice] = todasConsultas[i];
            indice++;
        }
    }
    
    delete[] todasConsultas;
    return resultados;
}

//Metodos Auxiliares
bool GestorArchivos::verificarDisponibilidadDoctor(int idDoctor, const char* fecha, const char* hora) {
    int cantidad;
    Cita* citas = listarCitasActivas(cantidad);
    
    if (citas == nullptr) {
        return true;
    }
    
    for (int i = 0; i < cantidad; i++) {
        if (citas[i].getDoctorID() == idDoctor &&
            strcmp(citas[i].getFecha(), fecha) == 0 &&
            strcmp(citas[i].getHora(), hora) == 0 &&
            strcmp(citas[i].getEstado(), "Cancelada") != 0) {
            delete[] citas;
            return false;
        }
    }
    
    delete[] citas;
    return true;
}