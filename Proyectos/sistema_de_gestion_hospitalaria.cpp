#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <cctype>
using namespace std;

// =============================================================================
// DECLARACIONES ANTICIPADAS DE ESTRUCTURAS
// =============================================================================

struct Paciente;
struct Doctor;
struct Cita;
struct HistorialMedico;

// =============================================================================
// ESTRUCTURAS PRINCIPALES
// =============================================================================

struct Hospital {
    char Nombre_hospital[100];
    char Direccion_hospital[150];
    char Telefono_hospital[15];

    Paciente* pacientes;
    int Cantidad_pacientes;
    int Capacidad_pacientes;

    Doctor* doctores;
    int Cantidad_doctores;
    int Capacidad_doctores;

    Cita* citas;
    int Cantidad_citas;
    int Capacidad_citas;

    int siguienteID_paciente;
    int siguienteID_doctor;
    int siguienteID_cita;
    int siguienteID_consulta;
};

struct HistorialMedico {
    int ID_consulta;
    char Fecha[11];
    char Hora[6];
    char Diagnostico[200];
    char Tratamiento_preescrito[200];
    char Medicamentos_recetados[150];
    int ID_doctor;
    float Costo_consulta;
};

struct Paciente {
    int ID_paciente;
    char Nombre_paciente[50];
    char Apellido_paciente[50];
    char Cedula_paciente[20];
    int Edad;
    char Sexo;
    char Tipo_sangre[5];
    char Telefono_paciente[15];
    char Direccion_paciente[100];
    char Email_paciente[50];
    
    HistorialMedico* historialesmedicos;
    int Cantidad_consultas;
    int Capacidad_consultas;

    int* Citas_agendadas;
    int Cantidad_citas_paciente;
    int Capacidad_citas_paciente;

    char Alergias[500];
    char Observaciones_generales[500];
    bool Activo;
};

struct Doctor {
    int ID_doctor;
    char Nombre_doctor[50];
    char Apellido_doctor[50];
    char Cedula_doctor[20];
    char Especialidad[50];
    int Anios_experiencia;
    float Costo_consulta;
    char Horario_atencion[50];
    char Telefono_doctor[15];
    char Email_doctor[50];

    int* Pacientes_asignados;
    int Cantidad_pacientes_asignados;
    int Capacidad_pacientes_asignados;

    int* Citas_agendadas;
    int Cantidad_citas_doctor;
    int Capacidad_citas_doctor;
    bool Disponible;
};

struct Cita {
    int ID_cita;
    int ID_paciente;
    int ID_doctor;
    char Fecha[11];
    char Hora[6];
    char Motivo_consulta[150];
    char Estado_cita[20];
    char Observaciones_cita[200];
    bool Atendida;
};

// =============================================================================
// PROTOTIPOS DE FUNCIONES
// =============================================================================


// MÓDULO DE UTILIDADES Y HELPERS

// #Gestión de memoria

Hospital* inicializarHospital(const char* nombre, const char* direccion, const char* telefono);
void destruirHospital(Hospital* hospital);
void redimensionarArrayPacientes(Hospital* hospital);
void redimensionarArrayDoctores(Hospital* hospital);
void redimensionarArrayCitas(Hospital* hospital);

// #Validaciones

bool validarFecha(const char* Fecha);
bool validarHora(const char* Hora);
int compararFechas(const char* Fecha1, const char* Fecha2);
bool validarCedula(const char* cedula);
bool validarEmail(const char* email);
char* copiarString(const char* origen);
Paciente* copiarPaciente(Paciente* original);
void limpiarBuffer();

// MÓDULO DE GESTIÓN DE PACIENTES

// #Funciones CRUD obligatorias

Paciente* crearPaciente(Hospital* hospital, const char* Nombre_paciente, const char* Apellido_paciente, const char* Cedula_paciente, int Edad, char Sexo, char* Telefono_paciente, char* Direccion_paciente,  char* Email_paciente, char* Tipo_sangre);
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* Cedula_paciente);
Paciente* buscarPacientePorId(Hospital* hospital, int ID_paciente);
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* Nombre_paciente, int* cantidad);
bool actualizarPaciente(Hospital* hospital, int ID_paciente);
bool eliminarPaciente(Hospital* hospital, int ID_paciente);
void listarPacientes(Hospital* hospital);

// #Funciones de historial médico (arrays dinámicos)

void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta);
HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad);
void mostrarHistorialMedico(Paciente* paciente);
HistorialMedico* obtenerUltimaConsulta(Paciente* paciente);

// MÓDULO DE GESTIÓN DE DOCTORES
Doctor* crearDoctor(Hospital* hospital, const char* Nombre_doctor, const char* Apellido_doctor, const char* Cedula_doctor, const char* Especialidad, int Anios_experiencia, float Costo_consulta);
Doctor* buscarDoctorPorId(Hospital* hospital, int ID_doctor);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* Especialidad, int* cantidad);
bool asignarPacienteADoctor(Doctor* doctor, int ID_paciente);
bool removerPacienteDeDoctor(Doctor* doctor, int ID_paciente);
void listarPacientesDeDoctor(Hospital* hospital, int ID_doctor);
void listarDoctores(Hospital* hospital);
bool eliminarDoctor(Hospital* hospital, int ID_doctor);

// MÓDULO DE GESTIÓN DE CITAS
Cita* agendarCita(Hospital* hospital, int ID_paciente, int ID_doctor, const char* Fecha, 
                  const char* Hora, const char* Motivo_consulta);
bool cancelarCita(Hospital* hospital, int ID_cita);
bool atenderCita(Hospital* hospital, int ID_cita, const char* Diagnostico,
                 const char* Tratamiento_preescrito, const char* Medicamentos_recetados);
Cita** obtenerCitasDePaciente(Hospital* hospital, int ID_paciente, int* cantidad);
Cita** obtenerCitasDeDoctor(Hospital* hospital, int ID_doctor, int* cantidad);
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* Fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);
bool verificarDisponibilidad(Hospital* hospital, int ID_doctor, const char* Fecha, const char* Hora);

// MODULO DE INTERFAZ DE USUARIO
void mostrarMenuPrincipal();
void mostrarMenuPacientes();
void mostrarMenuDoctores();
void mostrarMenuCitas();
void ejecutarMenuPacientes(Hospital* hospital);
void ejecutarMenuDoctores(Hospital* hospital);
void ejecutarMenuCitas(Hospital* hospital);

// =============================================================================
// IMPLEMENTACIÓN DE GESTIÓN DE MEMORIA
// =============================================================================


/*
 * Inicializa la estructura Hospital con capacidad inicial
 */

Hospital* inicializarHospital(const char* nombre, const char* direccion, const char* telefono){

    Hospital* hospital = new Hospital;

    // Información básica
    strncpy(hospital -> Nombre_hospital, nombre, 99);
    strncpy(hospital -> Direccion_hospital, direccion, 149);
    strncpy(hospital -> Telefono_hospital, telefono, 14);

    // Capacidades iniciales
    hospital -> Capacidad_pacientes = 10;
    hospital -> Capacidad_doctores = 10;
    hospital -> Capacidad_citas = 20;

    // Inicializar contadores
    hospital -> Cantidad_pacientes = 0;
    hospital -> Cantidad_doctores = 0;
    hospital -> Cantidad_citas = 0;

    // Inicializar arrays
    hospital -> pacientes = new Paciente[hospital->Capacidad_pacientes];
    hospital -> doctores = new Doctor[hospital->Capacidad_doctores];
    hospital -> citas = new Cita[hospital->Capacidad_citas];

    // IDs auto-increment
    hospital -> siguienteID_paciente = 1;
    hospital -> siguienteID_doctor = 1;
    hospital -> siguienteID_cita = 1;
    hospital -> siguienteID_consulta = 1;

    return hospital;
}


/*
 * Libera toda la memoria del hospital
 */

void destruirHospital(Hospital* hospital)
{
    if (hospital == nullptr) return;

    // Liberar pacientes y sus arrays dinámicos
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        Paciente& p = hospital -> pacientes[i];

        delete[] p.historialesmedicos;
        p.historialesmedicos = nullptr;
        delete[] p.Citas_agendadas;
        p.Citas_agendadas = nullptr;

    }

    delete[] hospital->pacientes;
    hospital -> pacientes = nullptr;

    // Liberar doctores y sus arrays dinámicos
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        Doctor& d = hospital -> doctores[i];

        delete[] d.Pacientes_asignados;
        d.Pacientes_asignados = nullptr;
        delete[] d.Citas_agendadas;
        d.Citas_agendadas = nullptr;

    }

    delete[] hospital -> doctores;
    hospital -> doctores = nullptr;

    // Liberar citas
    delete[] hospital -> citas;
    hospital -> citas = nullptr;

    // Liberar hospital
    delete hospital;
    hospital = nullptr;
}


/*
 * Redimensiona el array de pacientes (duplica capacidad)
 */

void redimensionarArrayPacientes(Hospital* hospital){

    int nuevaCapacidad = hospital -> Capacidad_pacientes * 2;
    Paciente* nuevoArray = new Paciente[nuevaCapacidad];

    // Copiar pacientes existentes
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        nuevoArray[i] = hospital -> pacientes[i];

    }

    // Liberar array antiguo y actualizar
    delete[] hospital -> pacientes;
    hospital -> pacientes = nuevoArray;
    hospital -> Capacidad_pacientes = nuevaCapacidad;

    cout << "Array de pacientes redimensionado a capacidad: " << hospital -> Capacidad_pacientes << endl;
}


/*
 * Redimensiona el array de doctores (duplica capacidad)
 */

void redimensionarArrayDoctores(Hospital* hospital){
    int nuevaCapacidad = hospital->Capacidad_doctores * 2;
    Doctor* nuevoArray = new Doctor[nuevaCapacidad];

    // Copiar doctores existentes
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        nuevoArray[i] = hospital -> doctores[i];

    }

    // Liberar array antiguo y actualizar
    delete[] hospital -> doctores;
    hospital -> doctores = nuevoArray;
    hospital -> Capacidad_doctores = nuevaCapacidad;

    cout << "Array de doctores redimensionado a capacidad: " << hospital -> Capacidad_doctores << endl;

}


/*
 * Redimensiona el array de citas (duplica capacidad)
 */

void redimensionarArrayCitas(Hospital* hospital){

    int nuevaCapacidad = hospital -> Capacidad_citas * 2;
    Cita* nuevoArray = new Cita[nuevaCapacidad];

    // Copiar citas existentes
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        nuevoArray[i] = hospital -> citas[i];

    }

    // Liberar array antiguo y actualizar
    delete[] hospital -> citas;
    hospital -> citas = nuevoArray;
    hospital -> Capacidad_citas = nuevaCapacidad;

    cout << "Array de citas redimensionado a capacidad: " << hospital -> Capacidad_citas << endl;
}

// =============================================================================
// IMPLEMENTACIÓN DE VALIDACIONES
// =============================================================================


/*
 * Valida la fecha establecida
 */

bool validarFecha(const char* Fecha){

    if (strlen(Fecha) != 10) return false;
    if (Fecha[4] != '-' || Fecha[7] != '-') return false;

    int anio, mes, dia;
    if (sscanf(Fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return false;

    if (anio < 1900 || anio > 2025) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    if (mes == 2){ // Febrero
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto && dia > 29) return false;
        if (!bisiesto && dia > 28) return false;

    } else if((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30){

        return false;

    }

    return true;

}


/*
 * Valida la hora establecida
 */

bool validarHora(const char* Hora){

    if (strlen(Hora) != 5) return false;
    if (Hora[2] != ':') return false;

    int hh, mm;
    if (sscanf(Hora, "%d:%d", &hh, &mm) != 2) return false;

    return (hh >= 0 && hh <= 23) && (mm >= 0 && mm <= 59);

}


/*
 * Compara dos fechas
 */

int compararFechas(const char* Fecha1, const char* Fecha2){

    return strcmp(Fecha1, Fecha2); // Retorna -1 si fecha1 < fecha 2
                                    // Retorna 0 si fecha1 = fecha 2
                                    // Retorna 1 si fecha 1 > fecha2

}


/*
 * Valida que la cédula no esté vacía y tenga longitud máxima
 */

bool validarCedula(const char* cedula){

    return cedula != nullptr && strlen(cedula) > 0 && strlen(cedula) <= 20;

}


/*
 * Valida basica de email
 */

bool validarEmail(const char* email){

    if (email == nullptr) return false;

    bool tieneArroba = false;
    bool tienePunto = false;

    for (int i = 0; email[i] != '\0'; i++){

        if (email[i] == '@') tieneArroba = true;
        if (email[i] == '.' && tieneArroba) tienePunto = true;

    }

    return tieneArroba && tienePunto;
}

// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE COPIA
// =============================================================================


/*
 * Copia un string en memoria dinámica
 */

char* copiarString(const char* origen){

    if (origen == nullptr) return nullptr;

    int longitud = strlen(origen);
    char* destino = new char[longitud + 1];
    strcpy(destino, origen);

    return destino;

}


// =============================================================================
// FUNCIÓN AUXILIAR LIMPIAR BUFFER
// =============================================================================

void limpiarBuffer(){

    cin.clear();
    cin.ignore(10000, '\n');

}

// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES CRUD DE PACIENTES
// =============================================================================


/*
 * Crea un nuevo paciente en el sistema
 */

Paciente* crearPaciente(Hospital* hospital, const char* Nombre_paciente, const char* Apellido_paciente, const char* Cedula_paciente, int Edad, char Sexo, char* Telefono_paciente, char* Direccion_paciente,  char* Email_paciente, char* Tipo_sangre){
    
    // Verificar cédula única
    if (buscarPacientePorCedula(hospital, Cedula_paciente) != nullptr){

        cout << "Error: Ya existe un paciente con esta cédula." << endl;

        return nullptr;

    }

    // Redimensionar si es necesario
    if (hospital -> Cantidad_pacientes >= hospital -> Capacidad_pacientes){

        redimensionarArrayPacientes(hospital);

    }

    // Crear nuevo paciente
    Paciente* nuevoPaciente = &hospital -> pacientes[hospital -> Cantidad_pacientes];

    // Asignar ID
    nuevoPaciente -> ID_paciente = hospital -> siguienteID_paciente++;

    // Agregar Datos Básicos
    strncpy(nuevoPaciente -> Nombre_paciente, Nombre_paciente, 49);
    strncpy(nuevoPaciente -> Apellido_paciente, Apellido_paciente, 49);
    strncpy(nuevoPaciente -> Cedula_paciente, Cedula_paciente, 19);
    nuevoPaciente -> Edad = Edad;
    nuevoPaciente -> Sexo = Sexo;
    strncpy(nuevoPaciente -> Telefono_paciente, Telefono_paciente, 14);
    strncpy(nuevoPaciente -> Direccion_paciente, Direccion_paciente, 99);
    strncpy(nuevoPaciente -> Email_paciente, Email_paciente, 49);
    strncpy(nuevoPaciente -> Tipo_sangre, Tipo_sangre, 4);

    // Inicializar arrays dinámicos
    nuevoPaciente -> Capacidad_consultas = 5;
    nuevoPaciente -> historialesmedicos = new HistorialMedico[nuevoPaciente -> Capacidad_consultas];
    nuevoPaciente -> Cantidad_consultas = 0;

    nuevoPaciente -> Capacidad_citas_paciente = 5;
    nuevoPaciente -> Citas_agendadas = new int[nuevoPaciente -> Capacidad_citas_paciente];
    nuevoPaciente -> Cantidad_citas_paciente = 0;

    // Inicializar strings vacíos;
    strcpy(nuevoPaciente -> Alergias, "");
    strcpy(nuevoPaciente -> Observaciones_generales, "");

    // Definir activo
    nuevoPaciente -> Activo = true;

    // Incrementar cantidad de pacientes
    hospital -> Cantidad_pacientes++;

    cout << "Paciente creado exitosamente. ID: " << nuevoPaciente -> ID_paciente << endl;

    return nuevoPaciente;

}


/*
 * Busca paciente por cédula
 */

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* Cedula_paciente){

    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        if (strcasecmp(hospital -> pacientes[i].Cedula_paciente, Cedula_paciente) == 0 && hospital -> pacientes[i].Activo){

            return &hospital -> pacientes[i];

        }

    }

    return nullptr;

}


/*
 * Busca paciente por ID
 */

Paciente* buscarPacientePorId(Hospital* hospital, int ID_paciente){

    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        if (hospital -> pacientes[i].ID_paciente == ID_paciente && hospital -> pacientes[i].Activo){

            return &hospital -> pacientes[i];
        
        }
    
    }
    
    return nullptr;

}

// =============================================================================
// FUNCIÓN BUSCAR PACIENTES POR NOMBRE
// =============================================================================


/*
 * Busca pacientes por nombre (búsqueda parcial case-insensitive)
 */

Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* Nombre_paciente, int* cantidad){

    *cantidad = 0;
    
    // Contar coincidencias
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        if (hospital -> pacientes[i].Activo){

            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", hospital -> pacientes[i].Nombre_paciente, hospital -> pacientes[i].Apellido_paciente);
            
            // Búsqueda case-insensitive manual (reemplazo de strcasestr)
            bool coincide = false;
            const char* haystack = nombreCompleto;
            const char* needle = Nombre_paciente;
            
            // Convertir a minúsculas para comparación
            char haystackLower[100], needleLower[100];
            strcpy(haystackLower, haystack);
            strcpy(needleLower, needle);
            
            // Convertir a minúsculas
            for (int j = 0; haystackLower[j]; j++) haystackLower[j] = tolower(haystackLower[j]);
            for (int j = 0; needleLower[j]; j++) needleLower[j] = tolower(needleLower[j]);
            
            // Buscar subcadena
            if (strstr(haystackLower, needleLower) != nullptr){

                coincide = true;

            }
            
            if (coincide){

                (*cantidad)++;

            }

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    // Crear array de resultados
    Paciente** resultados = new Paciente*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        if (hospital -> pacientes[i].Activo){

            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", hospital -> pacientes[i].Nombre_paciente, hospital -> pacientes[i].Apellido_paciente);
            
            // Búsqueda case-insensitive manual
            bool coincide = false;
            const char* haystack = nombreCompleto;
            const char* needle = Nombre_paciente;
            
            char haystackLower[100], needleLower[100];
            strcpy(haystackLower, haystack);
            strcpy(needleLower, needle);
            
            for (int j = 0; haystackLower[j]; j++) haystackLower[j] = tolower(haystackLower[j]);
            for (int j = 0; needleLower[j]; j++) needleLower[j] = tolower(needleLower[j]);
            
            if (strstr(haystackLower, needleLower) != nullptr){

                coincide = true;

            }
            
            if (coincide){

                resultados[index++] = &hospital -> pacientes[i];

            }

        }

    }

    return resultados;

}


/*
 * Actualiza datos del paciente
 */

bool actualizarPaciente(Hospital* hospital, int ID_paciente){

    Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);

    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;
    }

    cout << "\n=== ACTUALIZAR DATOS DEL PACIENTE ===" << endl;
    cout << "Paciente: " << paciente -> Nombre_paciente << " " << paciente -> Apellido_paciente << endl;
    
    char input[100];
    
    cout << "Nuevo nombre (" << paciente -> Nombre_paciente << "): ";
    cin.getline(input, 100);
    
    if (strlen(input) > 0){

        strncpy(paciente -> Nombre_paciente, input, 49);

    }
    
    cout << "Nuevo apellido (" << paciente -> Apellido_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(paciente -> Apellido_paciente, input, 49);

    }
    
    cout << "Nuevo teléfono (" << paciente -> Telefono_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(paciente -> Telefono_paciente, input, 14);

    }
    
    cout << "Nueva dirección (" << paciente -> Direccion_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(paciente -> Direccion_paciente, input, 99);

    }
    
    cout << "Nuevo email (" << paciente -> Email_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){ 

        if (validarEmail(input)){

            strncpy(paciente -> Email_paciente, input, 49);

        } else{

            cout << "Email inválido. No se actualizó." << endl;

        }
    }
    
    cout << "Nuevo tipo de sangre (" << paciente -> Tipo_sangre << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(paciente -> Tipo_sangre, input, 4);

    }
    
    cout << "Datos del paciente actualizados exitosamente." << endl;

    return true;
}


/*
 * Elimina un paciente del sistema
 */

bool eliminarPaciente(Hospital* hospital, int ID_paciente){

    Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);

    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;

    }

    // Liberar memoria de arrays dinámicos
    delete[] paciente -> historialesmedicos;
    paciente -> historialesmedicos = nullptr;
    delete[] paciente -> Citas_agendadas;
    paciente -> Citas_agendadas = nullptr;

    // Marcar como inactivo (eliminación lógica)
    paciente -> Activo = false;
    
    cout << "Paciente eliminado exitosamente." << endl;

    return true;

}


/*
 * Lista todos los pacientes en formato tabular
 */

void listarPacientes(Hospital* hospital){

    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    LISTA DE PACIENTES                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CÉDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
    cout << "¦-----+---------------------+--------------+------+----------¦" << endl;

    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        Paciente& p = hospital->pacientes[i];

        if (p.Activo){

            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", p.Nombre_paciente, p.Apellido_paciente);
            
            cout << "¦ " << setw(3) << p.ID_paciente << " ¦ "<< setw(19) << left << nombreCompleto << " ¦ "<< setw(12) << p.Cedula_paciente << " ¦ "<< setw(4) << p.Edad << " ¦ "<< setw(8) << p.Cantidad_consultas << "¦" << endl;
        
        }

    }
    
    cout << "+------------------------------------------------------------+" << endl;

}

// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE HISTORIAL MEDICO
// =============================================================================

/*
 * Agrega una consulta al historial médico del paciente
 */


void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta){

    // Redimensionar si es necesario
    if (paciente -> Cantidad_consultas >= paciente -> Capacidad_consultas){

        int nuevaCapacidad = paciente -> Capacidad_consultas * 2;
        HistorialMedico* nuevoArray = new HistorialMedico[nuevaCapacidad];

        // Copiar consultas existentes
        for (int i = 0; i < paciente -> Cantidad_consultas; i++){

            nuevoArray[i] = paciente -> historialesmedicos[i];

        }

        // Liberar array antiguo
        delete[] paciente -> historialesmedicos;
        
        // Actualizar puntero y capacidad
        paciente -> historialesmedicos = nuevoArray;
        paciente -> Capacidad_consultas = nuevaCapacidad;

        cout << "Historial médico redimensionado a capacidad: " << nuevaCapacidad << endl;

    }

    // Agregar nueva consulta
    paciente -> historialesmedicos[paciente -> Cantidad_consultas] = consulta;
    paciente -> Cantidad_consultas++;
}


/*
 * Obtiene el historial completo del paciente
 */

HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad){

    *cantidad = paciente -> Cantidad_consultas;

    return paciente -> historialesmedicos;

}


/*
 * Muestra el historial médico del paciente
 */

void mostrarHistorialMedico(Paciente* paciente){

    if (paciente -> Cantidad_consultas == 0){

        cout << "El paciente no tiene consultas registradas." << endl;

        return;

    }

    cout << "\n+----------------------------------------------------------------------------------+" << endl;
    cout << "¦                         HISTORIAL MÉDICO - " << paciente -> Nombre_paciente << " " << paciente -> Apellido_paciente << setw(25) << "¦" << endl;
    cout << "¦---------------------------------------------------------------------------------¦" << endl;
    cout << "¦ FECHA  ¦   HORA     ¦ DOCTOR ¦ DIAGNÓSTICO                                      ¦" << endl;
    cout << "¦--------+------------+--------+--------------------------------------------------¦" << endl;

    for (int i = 0; i < paciente -> Cantidad_consultas; i++){

        HistorialMedico& consulta = paciente -> historialesmedicos[i];

        cout << "¦ " << setw(6) << consulta.Fecha << " ¦ "
             << setw(8) << consulta.Hora << " ¦ "
             << setw(6) << consulta.ID_doctor << " ¦ "
             << setw(50) << left << consulta.Diagnostico << "¦" << endl;

    }
    
    cout << "+---------------------------------------------------------------------------------+" << endl;

}


/**
 * Obtiene la última consulta del paciente
 */

HistorialMedico* obtenerUltimaConsulta(Paciente* paciente){

    if (paciente -> Cantidad_consultas == 0){

        return nullptr;

    }

    return &paciente -> historialesmedicos[paciente -> Cantidad_consultas - 1];

}

// =============================================================================
// IMPLEMENTACIÓN DE MÓDULO DE GESTIÓN DE DOCTORES
// =============================================================================


/**
 * Crea un nuevo doctor en el sistema
 */

Doctor* crearDoctor(Hospital* hospital, const char* Nombre_doctor, const char* Apellido_doctor, const char* Cedula_doctor, const char* Especialidad, int Anios_experiencia, float Costo_consulta){
    
    // Verificar cédula única
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (strcasecmp(hospital -> doctores[i].Cedula_doctor, Cedula_doctor) == 0 && hospital->doctores[i].Disponible){

            cout << "Error: Ya existe un doctor con esta cédula." << endl;

            return nullptr;

        }

    }

    // Validar experiencia
    if (Anios_experiencia < 0){

        cout << "Error: Los años de experiencia no pueden ser negativos." << endl;

        return nullptr;

    }
    
    // Validar costo
    if (Costo_consulta <= 0){

        cout << "Error: El costo de consulta debe ser mayor a 0." << endl;

        return nullptr;

    }

    // Redimensionar si es necesario
    if (hospital->Cantidad_doctores >= hospital->Capacidad_doctores){

        redimensionarArrayDoctores(hospital);

    }

    // Crear nuevo doctor
    Doctor* nuevoDoctor = &hospital -> doctores[hospital -> Cantidad_doctores];
    
    // Datos básicos
    nuevoDoctor->ID_doctor = hospital -> siguienteID_doctor++;
    strncpy(nuevoDoctor -> Nombre_doctor, Nombre_doctor, 49);
    strncpy(nuevoDoctor -> Apellido_doctor, Apellido_doctor, 49);
    strncpy(nuevoDoctor -> Cedula_doctor, Cedula_doctor, 19);
    strncpy(nuevoDoctor -> Especialidad, Especialidad, 49);
    nuevoDoctor -> Anios_experiencia = Anios_experiencia;
    nuevoDoctor -> Costo_consulta = Costo_consulta;
    
    // Inicializar arrays dinámicos
    nuevoDoctor -> Capacidad_pacientes_asignados = 5;
    nuevoDoctor -> Pacientes_asignados = new int[nuevoDoctor -> Capacidad_pacientes_asignados];
    nuevoDoctor -> Cantidad_pacientes_asignados = 0;
    
    nuevoDoctor -> Capacidad_citas_doctor = 10;
    nuevoDoctor -> Citas_agendadas = new int[nuevoDoctor -> Capacidad_citas_doctor];
    nuevoDoctor -> Cantidad_citas_doctor = 0;
    
    // Inicializar strings vacíos
    strcpy(nuevoDoctor -> Horario_atencion, "");
    strcpy(nuevoDoctor -> Telefono_doctor, "");
    strcpy(nuevoDoctor -> Email_doctor, "");
    
    nuevoDoctor -> Disponible = true;
    
    hospital -> Cantidad_doctores++;
    
    cout << "Doctor creado exitosamente. ID: " << nuevoDoctor -> ID_doctor << endl;

    return nuevoDoctor;

}


/*
 * Busca doctor por ID
 */

Doctor* buscarDoctorPorId(Hospital* hospital, int ID_doctor){

    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (hospital -> doctores[i].ID_doctor == ID_doctor && hospital -> doctores[i].Disponible){

            return &hospital->doctores[i];

        }
    }

    return nullptr;

}


/*
 * Busca doctores por especialidad
 */

Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* Especialidad, int* cantidad){

    *cantidad = 0;
    
    // Primero contar
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (hospital -> doctores[i].Disponible && strcasecmp(hospital -> doctores[i].Especialidad, Especialidad) == 0){

            (*cantidad)++;

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    // Crear array de resultados
    Doctor** resultados = new Doctor*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (hospital -> doctores[i].Disponible && strcasecmp(hospital -> doctores[i].Especialidad, Especialidad) == 0){

            resultados[index++] = &hospital -> doctores[i];

        }

    }
    
    return resultados;
}


/*
 * Asigna un paciente a un doctor
 */

bool asignarPacienteADoctor(Doctor* doctor, int ID_paciente){
    // Verificar si ya está asignado
    for (int i = 0; i < doctor -> Cantidad_pacientes_asignados; i++) {

        if (doctor -> Pacientes_asignados[i] == ID_paciente){

            cout << "El paciente ya está asignado a este doctor." << endl;

            return false;

        }

    }
    
    // Redimensionar si es necesario
    if (doctor -> Cantidad_pacientes_asignados >= doctor -> Capacidad_pacientes_asignados){

        int nuevaCapacidad = doctor -> Capacidad_pacientes_asignados * 2;
        int* nuevoArray = new int[nuevaCapacidad];
        
        for (int i = 0; i < doctor -> Cantidad_pacientes_asignados; i++){

            nuevoArray[i] = doctor -> Pacientes_asignados[i];

        }
        
        delete[] doctor -> Pacientes_asignados;
        doctor -> Pacientes_asignados = nuevoArray;
        doctor -> Capacidad_pacientes_asignados = nuevaCapacidad;
    }
    
    // Asignar paciente
    doctor -> Pacientes_asignados[doctor -> Cantidad_pacientes_asignados] = ID_paciente;
    doctor -> Cantidad_pacientes_asignados++;
    
    cout << "Paciente asignado exitosamente al doctor." << endl;

    return true;
}


/*
 * Remueve un paciente de un doctor
 */

bool removerPacienteDeDoctor(Doctor* doctor, int ID_paciente){

    int posicion = -1;
    
    // Buscar paciente
    for (int i = 0; i < doctor -> Cantidad_pacientes_asignados; i++){

        if (doctor -> Pacientes_asignados[i] == ID_paciente){

            posicion = i;

            break;

        }
    
    }
    
    if (posicion == -1){

        cout << "El paciente no está asignado a este doctor." << endl;

        return false;

    }
    
    // Compactar array
    for (int i = posicion; i < doctor -> Cantidad_pacientes_asignados - 1; i++){

        doctor -> Pacientes_asignados[i] = doctor -> Pacientes_asignados[i + 1];

    }
    
    doctor -> Cantidad_pacientes_asignados--;
    cout << "Paciente removido exitosamente del doctor." << endl;

    return true;
}


/*
 * Lista pacientes asignados a un doctor
 */

void listarPacientesDeDoctor(Hospital* hospital, int ID_doctor){

    Doctor* doctor = buscarDoctorPorId(hospital, ID_doctor);

    if (doctor == nullptr){

        cout << "Doctor no encontrado." << endl;

        return;

    }
    
    if (doctor -> Cantidad_pacientes_asignados == 0){

        cout << "El doctor no tiene pacientes asignados." << endl;

        return;

    }
    
    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦           PACIENTES ASIGNADOS - Dr. " << doctor -> Nombre_doctor << " " << doctor -> Apellido_doctor << setw(15) << "¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ CÉDULA       ¦ EDAD ¦ CONSULTAS¦" << endl;
    cout << "¦-----+---------------------+--------------+------+----------¦" << endl;
    
    for (int i = 0; i < doctor -> Cantidad_pacientes_asignados; i++){

        Paciente* paciente = buscarPacientePorId(hospital, doctor -> Pacientes_asignados[i]);

        if (paciente != nullptr && paciente -> Activo){

            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", paciente -> Nombre_paciente, paciente -> Apellido_paciente);
            
            cout << "¦ " << setw(3) << paciente -> ID_paciente << " ¦ "
                 << setw(19) << left << nombreCompleto << " ¦ "
                 << setw(12) << paciente -> Cedula_paciente << " ¦ "
                 << setw(4) << paciente -> Edad << " ¦ "
                 << setw(8) << paciente -> Cantidad_consultas << "¦" << endl;

        }

    }
    
    cout << "+------------------------------------------------------------+" << endl;

}


/*
 * Lista todos los doctores en formato tabular
 */

void listarDoctores(Hospital* hospital){

    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                            LISTA DE DOCTORES                                ¦" << endl;
    cout << "¦-----------------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ NOMBRE COMPLETO     ¦ ESPECIALIDAD         ¦ EXP. ¦ COSTO    ¦ PAC.   ¦" << endl;
    cout << "¦-----+---------------------+----------------------+------+----------+--------¦" << endl;
    
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        Doctor& d = hospital -> doctores[i];

        if (d.Disponible){

            char nombreCompleto[100];
            snprintf(nombreCompleto, 100, "%s %s", d.Nombre_doctor, d.Apellido_doctor);
            
            cout << "¦ " << setw(3) << d.ID_doctor << " ¦ "
                 << setw(19) << left << nombreCompleto << " ¦ "
                 << setw(20) << d.Especialidad << " ¦ "
                 << setw(4) << d.Anios_experiencia << " ¦ $"
                 << setw(7) << fixed << setprecision(2) << d.Costo_consulta << " ¦ "
                 << setw(6) << d.Cantidad_pacientes_asignados << "¦" << endl;

        }

    }
    
    cout << "+-----------------------------------------------------------------------------+" << endl;

}


/*
 * Elimina un doctor del sistema
 */

bool eliminarDoctor(Hospital* hospital, int ID_doctor){

    Doctor* doctor = buscarDoctorPorId(hospital, ID_doctor);
    if (doctor == nullptr){

        cout << "Doctor no encontrado." << endl;

        return false;

    }
    
    // Liberar memoria de arrays dinámicos
    delete[] doctor -> Pacientes_asignados;
    doctor -> Pacientes_asignados = nullptr;
    delete[] doctor -> Citas_agendadas;
    doctor -> Citas_agendadas = nullptr;
    
    // Marcar como no disponible
    doctor -> Disponible = false;
    
    cout << "Doctor eliminado exitosamente." << endl;

    return true;

}

// =============================================================================
// IMPLEMENTACIÓN DE MÓDULO DE GESTIÓN DE CITAS
// =============================================================================


/*
 * Agenda una nueva cita médica
 */

Cita* agendarCita(Hospital* hospital, int ID_paciente, int ID_doctor, const char* Fecha, const char* Hora, const char* Motivo_consulta){
    
    // Verificar existencia
    Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);
    Doctor* doctor = buscarDoctorPorId(hospital, ID_doctor);
    
    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return nullptr;

    }

    if (doctor == nullptr){

        cout << "Doctor no encontrado." << endl;

        return nullptr;

    }
    
    // Validar fecha y hora
    if (!validarFecha(Fecha)){

        cout << "Formato de fecha inválido. Use YYYY-MM-DD." << endl;

        return nullptr;

    }
    
    if (!validarHora(Hora)){

        cout << "Formato de hora inválido. Use HH:MM." << endl;

        return nullptr;

    }
    
    // Verificar disponibilidad
    if (!verificarDisponibilidad(hospital, ID_doctor, Fecha, Hora)){

        cout << "El doctor no está disponible en esa fecha y hora." << endl;

        return nullptr;

    }
    
    // Redimensionar si es necesario
    if (hospital -> Cantidad_citas >= hospital -> Capacidad_citas){

        redimensionarArrayCitas(hospital);

    }
    
    // Crear nueva cita
    Cita* nuevaCita = &hospital->citas[hospital->Cantidad_citas];
    
    nuevaCita -> ID_cita = hospital -> siguienteID_cita++;
    nuevaCita -> ID_paciente = ID_paciente;
    nuevaCita -> ID_doctor = ID_doctor;
    strncpy(nuevaCita -> Fecha, Fecha, 10);
    strncpy(nuevaCita -> Hora, Hora, 5);
    strncpy(nuevaCita -> Motivo_consulta, Motivo_consulta, 149);
    strcpy(nuevaCita -> Estado_cita, "Agendada");
    strcpy(nuevaCita -> Observaciones_cita, "");
    nuevaCita -> Atendida = false;
    
    hospital -> Cantidad_citas++;
    
    // Agregar cita al paciente
    if (paciente -> Cantidad_citas_paciente >= paciente -> Capacidad_citas_paciente){

        int nuevaCapacidad = paciente -> Capacidad_citas_paciente * 2;
        int* nuevoArray = new int[nuevaCapacidad];
        
        for (int i = 0; i < paciente -> Cantidad_citas_paciente; i++){

            nuevoArray[i] = paciente -> Citas_agendadas[i];

        }
        
        delete[] paciente -> Citas_agendadas;
        paciente -> Citas_agendadas = nuevoArray;
        paciente -> Capacidad_citas_paciente = nuevaCapacidad;

    }

    paciente -> Citas_agendadas[paciente -> Cantidad_citas_paciente++] = nuevaCita -> ID_cita;
    
    // Agregar cita al doctor
    if (doctor -> Cantidad_citas_doctor >= doctor -> Capacidad_citas_doctor){

        int nuevaCapacidad = doctor -> Capacidad_citas_doctor * 2;
        int* nuevoArray = new int[nuevaCapacidad];
        
        for (int i = 0; i < doctor -> Cantidad_citas_doctor; i++){

            nuevoArray[i] = doctor -> Citas_agendadas[i];

        }
        
        delete[] doctor -> Citas_agendadas;
        doctor -> Citas_agendadas = nuevoArray;
        doctor -> Capacidad_citas_doctor = nuevaCapacidad;

    }

    doctor -> Citas_agendadas[doctor ->Cantidad_citas_doctor++] = nuevaCita -> ID_cita;
    
    cout << "Cita agendada exitosamente. ID: " << nuevaCita -> ID_cita << endl;

    return nuevaCita;

}


/*
 * Cancela una cita existente
 */

bool cancelarCita(Hospital* hospital, int ID_cita){

    Cita* cita = nullptr;
    int posicion = -1;
    
    // Buscar cita
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_cita == ID_cita){

            cita = &hospital -> citas[i];
            posicion = i;

            break;

        }

    }
    
    if (cita == nullptr){

        cout << "Cita no encontrada." << endl;

        return false;

    }
    
    if (cita -> Atendida){

        cout << "No se puede cancelar una cita ya atendida." << endl;

        return false;

    }
    
    // Actualizar estado
    strcpy(cita -> Estado_cita, "Cancelada");

    // Remover del array de paciente
    Paciente* paciente = buscarPacientePorId(hospital, cita -> ID_paciente);

    if (paciente != nullptr){

        for (int i = 0; i < paciente -> Cantidad_citas_paciente; i++){

            if (paciente -> Citas_agendadas[i] == ID_cita){

                // Compactar array del paciente
                for (int j = i; j < paciente -> Cantidad_citas_paciente - 1; j++){

                    paciente -> Citas_agendadas[j] = paciente -> Citas_agendadas[j + 1];

                }

                paciente -> Cantidad_citas_paciente--;

                break;

            }

        }

    }

    // Remover del array de doctor
    Doctor* doctor = buscarDoctorPorId(hospital, cita -> ID_doctor);

    if (doctor != nullptr){ 

        for (int i = 0; i < doctor -> Cantidad_citas_doctor; i++){

            if (doctor -> Citas_agendadas[i] == ID_cita){

                // Compactar array del doctor
                for (int j = i; j < doctor -> Cantidad_citas_doctor - 1; j++){

                    doctor -> Citas_agendadas[j] = doctor -> Citas_agendadas[j + 1];

                }

                doctor->Cantidad_citas_doctor--;

                break;

            }

        }

    }

    cout << "Cita cancelada exitosamente." << endl;

    return true;

}


/*
 * Atiende una cita y registra en historial
 */

bool atenderCita(Hospital* hospital, int ID_cita, const char* Diagnostico, const char* Tratamiento_preescrito, const char* Medicamentos_recetados){
    
    Cita* cita = nullptr;
    
    // Buscar cita
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_cita == ID_cita){

            cita = &hospital -> citas[i];

            break;

        }

    }
    
    if (cita == nullptr){

        cout << "Cita no encontrada." << endl;

        return false;

    }
    
    if (cita -> Atendida){

        cout << "La cita ya fue atendida anteriormente." << endl;

        return false;

    }
    
    // Buscar paciente
    Paciente* paciente = buscarPacientePorId(hospital, cita -> ID_paciente);

    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;

    }
    
    // Crear registro de historial
    HistorialMedico nuevaConsulta;
    nuevaConsulta.ID_consulta = hospital -> siguienteID_consulta++;
    strcpy(nuevaConsulta.Fecha, cita -> Fecha);
    strcpy(nuevaConsulta.Hora, cita -> Hora);
    strncpy(nuevaConsulta.Diagnostico, Diagnostico, 199);
    strncpy(nuevaConsulta.Tratamiento_preescrito, Tratamiento_preescrito, 199);
    strncpy(nuevaConsulta.Medicamentos_recetados, Medicamentos_recetados, 149);
    nuevaConsulta.ID_doctor = cita -> ID_doctor;
    
    // Obtener costo del doctor
    Doctor* doctor = buscarDoctorPorId(hospital, cita -> ID_doctor);
    if (doctor != nullptr){

        nuevaConsulta.Costo_consulta = doctor -> Costo_consulta;
    } else{

        nuevaConsulta.Costo_consulta = 0.0f;

    }
    
    // Agregar al historial
    agregarConsultaAlHistorial(paciente, nuevaConsulta);
    
    // Actualizar cita
    cita -> Atendida = true;
    strcpy(cita -> Estado_cita, "Atendida");
    strncpy(cita -> Observaciones_cita, Diagnostico, 199);
    
    cout << "Cita atendida y registrada en historial médico." << endl;

    return true;

}


/*
 * Obtiene todas las citas de un paciente
 */

Cita** obtenerCitasDePaciente(Hospital* hospital, int ID_paciente, int* cantidad){
    *cantidad = 0;
    
    // Primero contar
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_paciente == ID_paciente){

            (*cantidad)++;

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    // Crear array de resultados
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_paciente == ID_paciente){

            resultados[index++] = &hospital -> citas[i];

        }

    }
    
    return resultados;

}


/*
 * Obtiene todas las citas de un doctor
 */

Cita** obtenerCitasDeDoctor(Hospital* hospital, int ID_doctor, int* cantidad){

    *cantidad = 0;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_doctor == ID_doctor){

            (*cantidad)++;

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_doctor == ID_doctor){

            resultados[index++] = &hospital -> citas[i];

        }

    }
    
    return resultados;

}


/**
 * Obtiene citas por fecha específica
 */

Cita** obtenerCitasPorFecha(Hospital* hospital, const char* Fecha, int* cantidad){

    *cantidad = 0;
    
    if (!validarFecha(Fecha)){

        cout << "Fecha inválida." << endl;

        return nullptr;

    }
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (strcmp(hospital -> citas[i].Fecha, Fecha) == 0){

            (*cantidad)++;

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    Cita** resultados = new Cita*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (strcmp(hospital -> citas[i].Fecha, Fecha) == 0){

            resultados[index++] = &hospital->citas[i];

        }

    }
    
    return resultados;

}


/**
 * Lista citas pendientes
 */

void listarCitasPendientes(Hospital* hospital){

    cout << "\n+------------------------------------------------------------------------------+" << endl;
    cout << "¦                            CITAS PENDIENTES                                 ¦" << endl;
    cout << "¦-----------------------------------------------------------------------------¦" << endl;
    cout << "¦ ID  ¦ PAC.  ¦ DOC.  ¦ FECHA    ¦ HORA   ¦ MOTIVO                           ¦" << endl;
    cout << "¦-----+-------+-------+-------------------+-----------------------------------¦" << endl;
    
    bool hayCitas = false;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){ 
         
        Cita& c = hospital -> citas[i];
        if (!c.Atendida && strcmp(c.Estado_cita, "Cancelada") != 0){

            hayCitas = true;
            
            // Obtener nombres
            Paciente* p = buscarPacientePorId(hospital, c.ID_paciente);
            Doctor* d = buscarDoctorPorId(hospital, c.ID_doctor);
            
            char nombrePaciente[30] = "N/A";
            char nombreDoctor[30] = "N/A";
            
            if (p != nullptr){

                snprintf(nombrePaciente, 30, "%s", p -> Nombre_paciente);

            }

            if (d != nullptr){

                snprintf(nombreDoctor, 30, "%s", d -> Nombre_doctor);

            }
            
            cout << "¦ " << setw(3) << c.ID_cita << " ¦ "
                 << setw(5) << nombrePaciente << " ¦ "
                 << setw(5) << nombreDoctor << " ¦ "
                 << setw(8) << c.Fecha << " ¦ "
                 << setw(6) << c.Hora << " ¦ "
                 << setw(35) << left << c.Motivo_consulta << "¦" << endl;

        }

    }
    
    if (!hayCitas){

        cout << "¦                      NO HAY CITAS PENDIENTES                           ¦" << endl;

    }
    
    cout << "+-----------------------------------------------------------------------------+" << endl;

}


/*
 * Verifica disponibilidad de doctor en fecha y hora específicas
 */

bool verificarDisponibilidad(Hospital* hospital, int ID_doctor, const char* Fecha, const char* Hora){

    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        Cita& c = hospital -> citas[i];

        if (c.ID_doctor == ID_doctor && strcmp(c.Fecha, Fecha) == 0 && strcmp(c.Hora, Hora) == 0 && !c.Atendida && strcmp(c.Estado_cita, "Cancelada") != 0){

            return false; 

        }

    }

    return true;

}

// =============================================================================
// IMPLEMENTACIÓN DE INTERFAZ DE USUARIO
// =============================================================================

void mostrarMenuPrincipal(){

    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦              SISTEMA DE GESTIÓN HOSPITALARIA              ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Gestion de Pacientes                                   ¦" << endl;
    cout << "¦ 2. Gestion de Doctores                                    ¦" << endl;
    cout << "¦ 3. Gestion de Citas                                       ¦" << endl;
    cout << "¦ 4. Salir                                                  ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";

}

void mostrarMenuPacientes(){

    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                   GESTIÓN DE PACIENTES                    ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Registrar nuevo paciente                               ¦" << endl;
    cout << "¦ 2. Buscar paciente por cédula                             ¦" << endl;
    cout << "¦ 3. Buscar paciente por nombre                             ¦" << endl;
    cout << "¦ 4. Actualizar datos de paciente                           ¦" << endl;
    cout << "¦ 5. Eliminar paciente                                      ¦" << endl;
    cout << "¦ 6. Listar todos los pacientes                             ¦" << endl;
    cout << "¦ 7. Ver historial médico                                   ¦" << endl;
    cout << "¦ 8. Volver al menú principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";

}

void mostrarMenuDoctores(){

    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                    GESTIÓN DE DOCTORES                    ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Registrar nuevo doctor                                 ¦" << endl;
    cout << "¦ 2. Buscar doctor por ID                                   ¦" << endl;
    cout << "¦ 3. Buscar doctores por especialidad                       ¦" << endl;
    cout << "¦ 4. Asignar paciente a doctor                              ¦" << endl;
    cout << "¦ 5. Listar pacientes de doctor                             ¦" << endl;
    cout << "¦ 6. Listar todos los doctores                              ¦" << endl;
    cout << "¦ 7. Eliminar doctor                                        ¦" << endl;
    cout << "¦ 8. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";

}

void mostrarMenuCitas(){

    cout << "\n+------------------------------------------------------------+" << endl;
    cout << "¦                     GESTIÓN DE CITAS                      ¦" << endl;
    cout << "¦------------------------------------------------------------¦" << endl;
    cout << "¦ 1. Agendar nueva cita                                     ¦" << endl;
    cout << "¦ 2. Cancelar cita                                          ¦" << endl;
    cout << "¦ 3. Atender cita                                           ¦" << endl;
    cout << "¦ 4. Buscar citas por paciente                              ¦" << endl;
    cout << "¦ 5. Buscar citas por doctor                                ¦" << endl;
    cout << "¦ 6. Buscar citas por fecha                                 ¦" << endl;
    cout << "¦ 7. Listar citas pendientes                                ¦" << endl;
    cout << "¦ 8. Verificar disponibilidad                               ¦" << endl;
    cout << "¦ 9. Volver al menu principal                               ¦" << endl;
    cout << "+------------------------------------------------------------+" << endl;
    cout << "Seleccione una opcion: ";

}

void ejecutarMenuPacientes(Hospital* hospital){

    int opcion;
    char input[200];
    
    do {

        mostrarMenuPacientes();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion){

            case 1: {

                // Registrar nuevo paciente
                char nombre[50], apellido[50], cedula[20];
                int edad;
                char sexo;
                char telefono[15], direccion[100], email[50], tipo_sangre[5];
                
                cout << "Nombre: ";
                cin.getline(nombre, 50);
                cout << "Apellido: ";
                cin.getline(apellido, 50);
                cout << "Cedula: ";
                cin.getline(cedula, 20);
                cout << "Edad: ";
                cin >> edad;
                cout << "Sexo (M/F): ";
                cin >> sexo;
                cout << "Telefono: ";
                cin.getline(telefono, 15);
                cout << "Direccion: ";
                cin.getline(direccion, 100);
                cout << "Email: ";
                cin.getline(email, 50);
                cout << "Tipo de Sangre: ";
                cin.getline(tipo_sangre,5);
                limpiarBuffer();
                
                crearPaciente(hospital, nombre, apellido, cedula, edad, sexo, telefono, direccion, email, tipo_sangre);

                break;

            }
            
            case 2: {

                // Buscar por cédula
                cout << "Cédula: ";
                cin.getline(input, 20);
                
                Paciente* paciente = buscarPacientePorCedula(hospital, input);

                if (paciente != nullptr){

                    cout << "\nPaciente encontrado:" << endl;
                    cout << "ID: " << paciente -> ID_paciente << endl;
                    cout << "Nombre: " << paciente -> Nombre_paciente << " " << paciente -> Apellido_paciente << endl;
                    cout << "Edad: " << paciente -> Edad << endl;
                    cout << "Telefono: " << paciente -> Telefono_paciente << endl;

                } else{

                    cout << "Paciente no encontrado." << endl;

                }

                break;

            }
            
            case 3: {

                // Buscar por nombre
                cout << "Nombre: ";
                cin.getline(input, 100);
                
                int cantidad;
                Paciente** resultados = buscarPacientesPorNombre(hospital, input, &cantidad);
                
                if (resultados != nullptr){

                    cout << "Se encontraron " << cantidad << " pacientes:" << endl;

                    for (int i = 0; i < cantidad; i++){

                        cout << i+1 << ". " << resultados[i] -> Nombre_paciente << " " << resultados[i] -> Apellido_paciente << " (Cédula: " << resultados[i] -> Cedula_paciente << ")" << endl;
                    
                    }

                    delete[] resultados;

                } else{

                    cout << "No se encontraron pacientes." << endl;

                }

                break;

            }
            
            case 4: {

                // Actualizar paciente
                int ID_paciente;
                cout << "ID del paciente a actualizar: ";
                cin >> ID_paciente;
                limpiarBuffer();
                
                actualizarPaciente(hospital, ID_paciente);

                break;
            }
            
            case 5: {

                // Eliminar paciente
                int ID_paciente;
                cout << "ID del paciente a eliminar: ";
                cin >> ID_paciente;
                limpiarBuffer();
                
                eliminarPaciente(hospital, ID_paciente);

                break;
            }
            
            case 6: {

                // Listar pacientes
                listarPacientes(hospital);

                break;

            }
            
            case 7: {

                // Ver historial médico
                int ID_paciente;
                cout << "ID del paciente: ";
                cin >> ID_paciente;
                limpiarBuffer();
                
                Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);

                if (paciente != nullptr){

                    mostrarHistorialMedico(paciente);

                } else{

                    cout << "Paciente no encontrado." << endl;

                }

                break;

            }
            
            case 8: {

                cout << "Volviendo al menú principal..." << endl;

                break;

            }
            
            default: {

                cout << "Opción invalida." << endl;

            }  

        }

    } while (opcion != 8);

}

void ejecutarMenuDoctores(Hospital* hospital){
    int opcion;
    char input[200];
    
    do {

        mostrarMenuDoctores();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion){

            case 1: {

                // Registrar nuevo doctor
                char nombre[50], apellido[50], cedula[20], especialidad[50];
                int aniosExperiencia;
                float costoConsulta;
                
                cout << "Nombre: ";
                cin.getline(nombre, 50);
                cout << "Apellido: ";
                cin.getline(apellido, 50);
                cout << "Cedula: ";
                cin.getline(cedula, 20);
                cout << "Especialidad: ";
                cin.getline(especialidad, 50);
                cout << "Años de experiencia: ";
                cin >> aniosExperiencia;
                cout << "Costo de consulta: ";
                cin >> costoConsulta;
                limpiarBuffer();
                
                crearDoctor(hospital, nombre, apellido, cedula, especialidad, aniosExperiencia, costoConsulta);

                break;
            }
            
            case 2: {

                // Buscar doctor por ID
                int ID_doctor;
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                Doctor* doctor = buscarDoctorPorId(hospital, ID_doctor);
                if (doctor != nullptr){

                    cout << "\nDoctor encontrado:" << endl;
                    cout << "Nombre: " << doctor -> Nombre_doctor << " " << doctor -> Apellido_doctor << endl;
                    cout << "Especialidad: " << doctor -> Especialidad << endl;
                    cout << "Experiencia: " << doctor -> Anios_experiencia << " años" << endl;
                    cout << "Costo: $" << doctor -> Costo_consulta << endl;

                } else{

                    cout << "Doctor no encontrado." << endl;

                }

                break;

            }
            
            case 3: {

                // Buscar por especialidad
                cout << "Especialidad: ";
                cin.getline(input, 50);
                
                int cantidad;
                Doctor** resultados = buscarDoctoresPorEspecialidad(hospital, input, &cantidad);
                
                if (resultados != nullptr){

                    cout << "\nSe encontraron " << cantidad << " doctores:" << endl;
                    for (int i = 0; i < cantidad; i++){

                        cout << i+1 << ". " << resultados[i] -> Nombre_doctor << " " << resultados[i] -> Apellido_doctor << " (ID: " << resultados[i] -> ID_doctor << ")" << endl;

                    }

                    delete[] resultados;

                } else{

                    cout << "No se encontraron doctores." << endl;

                }

                break;

            }
            
            case 4: {

                // Asignar paciente a doctor
                int ID_doctor, ID_paciente;
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                cout << "ID del paciente: ";
                cin >> ID_paciente;
                limpiarBuffer();
                
                Doctor* doctor = buscarDoctorPorId(hospital, ID_doctor);

                if (doctor != nullptr) {

                    asignarPacienteADoctor(doctor, ID_paciente);

                } else{

                    cout << "Doctor no encontrado." << endl;

                }

                break;

            }
            
            case 5: {

                // Listar pacientes de doctor
                int ID_doctor;
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                listarPacientesDeDoctor(hospital, ID_doctor);

                break;

            }
            
            case 6: {

                // Listar doctores
                listarDoctores(hospital);

                break;

            }
            
            case 7: {

                // Eliminar doctor
                int ID_doctor;
                cout << "ID del doctor a eliminar: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                eliminarDoctor(hospital, ID_doctor);

                break;

            }
            
            case 8: {

                cout << "Volviendo al menu principal..." << endl;

                break;

            }
                
            default: {

                cout << "Opcion invalida." << endl;

            }
        }
        
    } while (opcion != 8);

}

void ejecutarMenuCitas(Hospital* hospital){
    int opcion;
    char input[200];
    
    do{

        mostrarMenuCitas();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion){

            case 1: {
                // Agendar cita
                int ID_paciente, ID_doctor;
                char fecha[11], hora[6], motivo[150];
                
                cout << "ID del paciente: ";
                cin >> ID_paciente;
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                cout << "Hora (HH:MM): ";
                cin.getline(hora, 6);
                cout << "Motivo: ";
                cin.getline(motivo, 150);
                
                agendarCita(hospital, ID_paciente, ID_doctor, fecha, hora, motivo);

                break;

            }
            
            case 2: {

                // Cancelar cita
                int ID_cita;
                cout << "ID de la cita a cancelar: ";
                cin >> ID_cita;
                limpiarBuffer();
                
                cancelarCita(hospital, ID_cita);

                break;

            }
            
            case 3: {

                // Atender cita
                int ID_cita;
                char diagnostico[200], tratamiento[200], medicamentos[150];
                
                cout << "ID de la cita a atender: ";
                cin >> ID_cita;
                limpiarBuffer();
                
                cout << "Diagnostico: ";
                cin.getline(diagnostico, 200);
                cout << "Tratamiento: ";
                cin.getline(tratamiento, 200);
                cout << "Medicamentos: ";
                cin.getline(medicamentos, 150);
                
                atenderCita(hospital, ID_cita, diagnostico, tratamiento, medicamentos);

                break;

            }
            
            case 4: {

                // Citas por paciente
                int ID_paciente;
                cout << "ID del paciente: ";
                cin >> ID_paciente;
                limpiarBuffer();
                
                int cantidad;
                Cita** citas = obtenerCitasDePaciente(hospital, ID_paciente, &cantidad);
                
                if (citas != nullptr){

                    cout << "\nSe encontraron " << cantidad << " citas:" << endl;

                    for (int i = 0; i < cantidad; i++){

                        cout << "Cita #" << citas[i] -> ID_cita << " - " << citas[i] -> Fecha << " " << citas[i] -> Hora << " - " << citas[i] -> Estado_cita << endl;
                    
                    }

                    delete[] citas;

                } else{

                    cout << "No se encontraron citas." << endl;

                }

                break;

            }
            
            case 5: {

                // Citas por doctor
                int ID_doctor;
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                int cantidad;
                Cita** citas = obtenerCitasDeDoctor(hospital, ID_doctor, &cantidad);
                
                if (citas != nullptr){

                    cout << "\nSe encontraron " << cantidad << " citas:" << endl;

                    for (int i = 0; i < cantidad; i++){

                        cout << "Cita #" << citas[i] -> ID_cita << " - " << citas[i] -> Fecha << " " << citas[i] -> Hora << " - " << citas[i] -> Estado_cita << endl;
                    
                    }

                    delete[] citas;

                } else{

                    cout << "No se encontraron citas." << endl;

                }

                break;

            }
            
            case 6: {

                // Citas por fecha
                char fecha[11];
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                
                int cantidad;
                Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                
                if (citas != nullptr){

                    cout << "\nSe encontraron " << cantidad << " citas:" << endl;

                    for (int i = 0; i < cantidad; i++){

                        cout << "Cita #" << citas[i] -> ID_cita << " - " << citas[i] -> Hora << " - Doctor: " << citas[i] -> ID_doctor << " - " << citas[i] -> Estado_cita << endl;

                    }

                    delete[] citas;

                } else{

                    cout << "No se encontraron citas." << endl;

                }

                break;

            }
            
            case 7: {

                // Listar citas pendientes
                listarCitasPendientes(hospital);

                break;

            }
            
            case 8: {

                // Verificar disponibilidad
                int ID_doctor;
                char fecha[11], hora[6];
                
                cout << "ID del doctor: ";
                cin >> ID_doctor;
                limpiarBuffer();
                
                cout << "Fecha (YYYY-MM-DD): ";
                cin.getline(fecha, 11);
                cout << "Hora (HH:MM): ";
                cin.getline(hora, 6);
                
                if (verificarDisponibilidad(hospital, ID_doctor, fecha, hora)){

                    cout << "El doctor esta disponible en esa fecha y hora." << endl;

                } else{

                    cout << "El doctor no está disponible en esa fecha y hora." << endl;

                }

                break;

            }
            
            case 9: {

                cout << "Volviendo al menu principal..." << endl;

                break;

            }
                
            default: {

                cout << "Opcion invalida." << endl;

            }

        }
        
    } while (opcion != 9);

}

// =============================================================================
// FUNCIÓN PRINCIPAL
// =============================================================================

int main() {
    cout << "Inicializando Sistema de Gestion Hospitalaria..." << endl;
    
    // Crear hospital
    char nombreHospital[100];
    char direccionHospital[150];
    char telefonoHospital[15];

    cout << "Nombre del hospital: ";
    cin.getline(nombreHospital, 100);
    cout << "Direccion del hospital: ";
    cin.getline(direccionHospital, 150);
    cout << "Telefono del hospital: ";
    cin.getline(telefonoHospital, 15);

    Hospital* hospital = inicializarHospital(nombreHospital, direccionHospital, telefonoHospital);
   
    cout << "Sistema inicializado correctamente." << endl;
    cout << "Hospital: " << hospital->Nombre_hospital << endl;

    // Menú principal
    int opcion;
    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                ejecutarMenuPacientes(hospital);
                break;
            }
            case 2: {
                ejecutarMenuDoctores(hospital);
                break;
            }
            case 3: {
                ejecutarMenuCitas(hospital);
                break;
            }
            case 4: {
                cout << "Gracias por usar el Sistema de Gestión Hospitalaria." << endl;
                break;
            }
            default: {
                cout << "Opción inválida. Intente nuevamente." << endl;
            }
        }
        
    } while (opcion != 4);
    
    // Liberar memoria
    destruirHospital(hospital);
    
    return 0;
}
