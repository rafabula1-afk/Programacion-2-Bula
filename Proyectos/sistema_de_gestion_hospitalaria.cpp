#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
using namespace std;


struct Hospital
{
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

struct Paciente
{
    int ID_paciente;
    char Nombre_paciente[50];
    char Apellido_paciente[50];
    char Cedula_paciente[20];

    int Edad;
    char Sexo_paciente;
    char Tipo_sangre[5];

    char Telefono_paciente[15];
    char Direccion_paciente[100];
    char Email_paciente[50];
    
    
    HistorialMedico* historialesmedicos;
    int Cantidad_consultas;
    int Capacidad_Consultas;

    int* Citas_agendadas;
    int Cantidad_citas;
    int Capacidad_citas;

    char Alergias[500];
    char Observaciones_generales[500];

    bool Activo_Inactivo;

};

struct HistorialMedico
{
    int ID_consulta;
    char Fecha[11];
    char Hora[6];
    char Diagnostico[200];
    char Tratamiento_preescrito[200];
    char Medicamentos_recetados[150];
    int ID_doctor;
    float Costo_consulta;

};

struct Doctor
{
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

    int* Pacientes_agendados;
    int Cantidad_pAgendados;
    int Capacidad_pAgendados;

    int* Citas_agendadas;
    int Cantidad_cAgendadas;
    int Capacidad_cAgendadas;

    bool Disponibilidad;

};

struct Cita
{
    int ID_cita;
    Paciente* pacientes;
    Doctor* doctores;
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

Hospital* inicializarHospital(const char* Nombre_hospital, int Capacidad_);
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


// MÓDULO DE GESTIÓN DE PACIENTES

// #Funciones CRUD obligatorias

Paciente* crearPaciente(Hospital* hospital, const char* Nombre_paciente, const char* Apellido_paciente, const char* Cedula_paciente, int Edad_paciente, char Sexo_paciente);
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* Cedula_paciente);
Paciente* buscarPacientePorId(Hospital* hospital, int ID_paciente);
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* Nombre_paciente, int* cantidad);
bool actualizarPaciente(Hospital* hospital, int ID_paciente);
bool eliminarPaciente(Hospital* hospital, int ID_paciente);
void listarPacientes(Hospital* hospital);

// #Funciones de historial médico (arrays dinámicos)

void agregarConsultaAlHistorial(Paciente* pacientes, HistorialMedico consulta);
HistorialMedico* obtenerHistorialCompleto(Paciente* pacientes, int* cantidad);
void mostrarHistorialMedico(Paciente* pacientes);
HistorialMedico* obtenerUltimaConsulta(Paciente* pacientes);


//MÓDULO DE GESTIÓN DE DOCTORES

Doctor* crearDoctor(Hospital* hospital, const char* Nombre_doctor, const char* Apellido_doctor, const char* Cedula_doctor,const char* Especialidad, int Anios_experiencia, float Costo_consulta);
Doctor* buscarDoctorPorId(Hospital* hospital, int ID_doctor);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* Especialidad, int* cantidad);
bool asignarPacienteADoctor(Doctor* doctor, int ID_paciente);
bool removerPacienteDeDoctor(Doctor* doctor, int ID_paciente);
void listarPacientesDeDoctor(Hospital* hospital, int ID_doctor);
void listarDoctores(Hospital* hospital);
bool eliminarDoctor(Hospital* hospital, int ID_doctor);


//MÓDULO DE GESTIÓN DE CITAS

Cita* agendarCita(Hospital* hospital, int ID_paciente, int ID_doctor, const char* Fecha, const char* Hora, const char* Motivo_consulta);
bool cancelarCita(Hospital* hospital, int ID_cita);
bool atenderCita(Hospital* hospital, int ID_cita, const char* Diagnostico, const char* Tratamiento_preescrito, const char* Medicamentos_recetados);
Cita** obtenerCitasDePaciente(Hospital* hospital, int ID_Paciente, int* cantidad);
Cita** obtenerCitasDeDoctor(Hospital* hospital, int ID_doctor, int* cantidad);
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* Fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);
bool verificarDisponibilidad(Hospital* hospital, int ID_doctor, const char* Fecha, const char* Hora);


// =============================================================================
// IMPLEMENTACIÓN DE GESTIÓN DE MEMORIA
// =============================================================================


/*
 * Inicializa la estructura Hospital con capacidad inicial
 */

Hospital* inicializarHospital(const char* Nombre_hospital){
    
    Hospital* hospital = new Hospital;

    // Información básica
    strncpy(hospital -> Nombre_hospital, Nombre_hospital, 99);
    strncpy(hospital -> Direccion_hospital, Direccion_hospital, 149);
    strncpy(hospital -> Telefono_hospital, Telefono_hospital, 14);

    // Capacidades iniciales
    hospital -> Capacidad_pacientes = 10;
    hospital -> Capacidad_doctores = 10;
    hospital -> Capacidad_citas = 20;

    // Inicializar arrays

    hospital -> pacientes = new Paciente[hospital -> Capacidad_pacientes];
    hospital -> doctores = new Doctor[hospital -> Capacidad_doctores];
    hospital -> citas = new Cita[hospital -> Capacidad_citas];

    // IDs auto-increment
    hospital -> siguienteID_paciente = 1;
    hospital -> siguienteID_doctor = 1;
    hospital -> siguienteID_cita = 1;
    hospital - siguienteID_consulta = 1;

    return hospital;
}


/**
 * Libera toda la memoria del hospital
 */

void destruirHospital(Hospital* hospital){
    
    // Verificar si existe un hospital registrado
    if (hospital == nullptr) return;

    // Liberar pacientes y sus arrays dinamicos
    delete[] = hospital -> pacientes;
    hospital -> pacientes = nullptr;

    // Liberar doctores y sus arrays dinamicos
    delete[] = hospital -> doctores;
    hospital -> doctores = nullptr;

    // Liberar citas
    delete[] = hospital -> citas;
    hospital -> citas = nullptr; 

}


/*
 * Redimensiona el array de pacientes (duplica capacidad)
 */

void redimensionarArrayPacientes(Hospital* hospital){

    // Redefinir la capacidad de pacientes
    int nuevaCapacidad_pacientes = Capacidad_pacientes * 2;
    
    // Inicializar nuevo array dinamico de pacientes
    Paciente* nuevoArray_pacientes = new Paciente[nuevaCapacidad_pacientes];


    // Copiar pacientes existenes
    for(i = 0; i < Cantidad_pacientes; i++){

        nuevoArray_pacientes[i] = hospital -> pacientes[i];

    }

    // Liberar el antiguo y actualizar
    delete[] = hospital -> pacientes;
    hospital -> pacientes = nuevoArray_pacientes;

    hospital -> Capacidad_pacientes = nuevaCapacidad_pacientes;

    cout <<"Array de pacientes redimensionado a capacidad:"<<hospital -> Capacidad_pacientes<<endl;

}


/*
 * Redimensiona el array de pacientes (duplica capacidad)
 */

void redimensionarArrayDoctores(Hospital* hospital){

    // Redefinir la capacidad de pacientes
    int nuevaCapacidad_doctores = Capacidad_doctores * 2;
    
    // Inicializar nuevo array dinamico de pacientes
    Doctor* nuevoArray_doctores = new Doctor[nuevaCapacidad_doctores];


    // Copiar pacientes existenes
    for(i = 0; i < Cantidad_doctores; i++){

        nuevoArray_doctores[i] = hospital -> doctores[i];

    }

    // Liberar el antiguo y actualizar
    delete[] = hospital -> doctores;
    hospital -> doctores = nuevoArray_doctores;

    hospital -> Capacidad_doctores = nuevaCapacidad_doctores;


    cout <<"Array de doctores redimensionado a capacidad:"<<hospital -> Capacidad_doctores<<endl;
}


/*
 * Redimensiona el array de pacientes (duplica capacidad)
 */

void redimensionarArrayCitas(Hospital* hospital){
    
    // Redefinir la capacidad de citas
    int nuevaCapacidad_citas = Capacidad_citas * 2;
    
    // Inicializar nuevo array dinamico de pacientes
    Cita* nuevoArray_citas = new Cita[nuevaCapacidad_citas];


    // Copiar pacientes existenes
    for(i = 0; i < Cantidad_citas; i++){

        nuevoArray_citas[i] = hospital -> citas[i];

    }

    // Liberar el antiguo y actualizar
    delete[] = hospital -> citas;
    hospital -> citas = nuevoArray_citas;

    hospital -> Capacidad_citas = nuevaCapacidad_citas;

    cout <<"Array de citas redimensionado a capacidad:"<<hospital -> Capacidad_citas<<endl;
}


// =============================================================================
// IMPLEMENTACIÓN DE VALIDACIONES
// =============================================================================

    
/*
 * Valida la fecha establecida
 */

bool validarFecha(const char* Fecha){

    // Valida el formato de fecha YYYY-MM-DD
    if(strlen(Fecha) != 10) return false;
    if(Fecha[4] != '-' || Fecha[7] != '-') return false;

    // Extraer año, mes y dia
    int anio, mes, dia;
    if(sscanf(Fecha, "%d-%d-%d", &anio, &mes, &dia) !=3) return false;

    // Validar rangos basicos
    if(anio < 1900 || anio > 2025) return false;
    if(mes < 1 || mes > 12) return false;
    if(dia < 1 || dia > 31) return false;

    // Validar dias segun mes
    if(mes = 2){ // Febrero
        
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 = 0);
        if(bisiesto && dia > 29) return false;
        if(bisiesto && dia > 28) return false;

    }

    else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30){
        
        return false;

    }

    return true;

}


/*
 * Valida la hora establecida
 */

 bool validarHora(const char* Hora){

    // Validar el formato de hora HH:MM
    if(strlen(Hora) != 5) return false;
    if(Hora[2] != ':') return false;

    // Extraer horas y minutos
    int hh, mm;
    if(sscanf(Hora, "%d:%d", &hh, &mm) != 2) return false;

    // Validar rangos basicos
    return (hh >= 0 && hh <= 23) && (mm >= 0 && mm <= 59);

 }


/*
 * Valida la hora establecida
 */

int compararFechas(const char* Fecha1, const char* Fecha2){

    return strcmp(Fecha1, Fecha2);  // Retorna -1 si fecha1 < fecha 2
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

    // Validar que el email no este vaio
    if (email == nullptr) return false;

    // Verificar presencia de '@' y '.'
    bool tieneArroba = false;
    bool tieneArroba = false;

    for(int i = 0; email[i] != '\0'; i++){
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

    // Verificar si origen es un puntero nulo
    if (origen == nullptr) {

        return nullptr;

    }

    // Calcular longitud
    int longitud = strlen(origen);

    // Inicializar nuevo array dinamico
    char* destino = new char[longitud + 1];

    // Copiar la linea de caracteres
    strcpy(destino, origen);

    return destino;
}


// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES CRUD OBLIGATORIAS
// =============================================================================


/*
 * Crea un nuevo paciente en el sistema
 */

Paciente* crearPaciente(Hospital* hospital, const char* Nombre_paciente, const char* Apellido_paciente, const char* Cedula_paciente, int Edad_paciente, char Sexo_paciente){

    // Verificar cédula única
    if (buscarPacientePorCedula(hospital, Cedula_paciente) != nullptr){

        cout << "❌ Error: Ya existe un paciente con esta cédula." << endl;

        return nullptr;

    }

    // Redimensionar si es necesario
    if (hospital -> Cantidad_pacientes >= hospital -> Capacidad_pacientes){

        redimensionarArrayPacientes(hospital);

    }

     // Crear nuevo paciente
    Paciente* Nuevo_paciente = &hospital -> pacientes[hospital->Cantidad_pacientes];

    // Asignar ID
    Nuevo_paciente -> ID_paciente = hospital -> siguienteID_paciente++;

    // Agregar Datos Basicos
    strncpy(Nuevo_paciente -> Nombre_paciente, Nombre_paciente, 49);
    strncpy(Nuevo_paciente -> Apellido_paciente, Apellido_paciente, 49);
    strncpy(Nuevo_paciente -> Cedula_paciente, Cedula_paciente, 19);
    Nuevo_paciente -> Edad_paciente = Edad_paciente;
    Nuevo_paciente -> Sexo_paciente = Sexo_paciente;

    // Inicializar arrays dinámicos
    Nuevo_paciente -> Capacidad_consultas = 5;
    Nuevo_paciente -> historialesmedicos = new HistorialMedico[Nuevo_paciente -> Capacidad_consultas];
    Nuevo_paciente -> Cantidad_consultas = 0;

    Nuevo_paciente -> Capacidad_citas = 5;
    Nuevo_paciente -> citas = new Cita[Nuevo_paciente -> Capacidad_citas];
    Nuevo_paciente -> Cantidad_citas = 0;

    // Inicializar strings vacíos
    strcpy(Nuevo_paciente -> Tipo_sangre, "");
    strcpy(Nuevo_paciente -> Telefono_paciente, "");
    strcpy(Nuevo_paciente -> Direccion_paciente, "");
    strcpy(Nuevo_paciente -> Email_paciente, "");
    strcpy(Nuevo_paciente -> Alergias, "");
    strcpy(Nuevo_paciente -> Observaciones_generales, "");

    // Definir activo
    Nuevo_paciente-> Activo_Inactivo = true;

    // Subir cantidad de pacientes
    hospital->cantidadPacientes++;


    cout << "Paciente creado exitosamente. ID: " << Nuevo_paciente->ID_paciente << endl;

    return Nuevo_paciente;

}


/*
 * Busca paciente por cédula
 */
 
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* Cedula_paciente){

    for (int i = 0; i < hospital->Cantidad_pacientes; i++) {

        if (strcasecmp(hospital->pacientes[i].Cedula_paciente, Cedula_paciente) == 0) {

            return &hospital->pacientes[i];
        }
    }

    return nullptr;
}


/*
 * Busca paciente por ID
 */

Paciente* buscarPacientePorId(Hospital* hospital, int ID_paciente){

    for (int i = 0; i < hospital-> Cantidad_pacientes; i++) {

        if (hospital -> pacientes[i].ID_paciente == ID_paciente && hospital -> pacientes[i].Activo_Inactivo) {

            return &hospital->pacientes[i];
        }

    }

    return nullptr;
}


/*
 * Busca pacientes por nombre (búsqueda parcial case-insensitive)
 */

Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* Nombre_paciente, int* Cantidad_coincidencias){

    *Cantidad_coincidencias = 0;

    // Contar las coincidencias
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++) {

        if (hospital->pacientes[i].activo) {

            char Nombre_completoP[100];

            snprintf(Nombre_completoP, 100, "%s %s", hospital->pacientes[i].Nombre_paciente, hospital->pacientes[i].Apellido_paciente);
            
            if (strcasestr(Nombre_completoP, Nombre_paciente) != nullptr) {

                (*Cantidad_coincidencias)++;

            }

        }

        
    }

    // Verificar si existen coincidencias
    if (*cantidad == 0) return nullptr;

    // Crear array de resultados
    Paciente** resultados = new Paciente*[*cantidad];

    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_pacientes; i++){

        if (hospital -> pacientes[i].Activo_Inactivo){

            char Nombre_completoP[100];

            snprintf(Nombre_completoP, 100, "%s %s", hospital -> pacientes[i].Nombre_paciente, hospital-> pacientes[i].Apellido_paciente);
            
            if (strcasestr(Nombre_completoP, Nombre_paciente) != nullptr){

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

    // Buscar paciente por ID
    Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);

    if (paciente == nullptr){

        cout << "❌ Paciente no encontrado." << endl;

        return false;

    }

    // Actualizar datos basicos
    cout << "\n=== ACTUALIZAR DATOS DEL PACIENTE ===" << endl;
    cout << "Paciente: " << pacientes -> Nombre_paciente << " " << pacientes -> Nombre_paciente << endl;
    
    char input[100];
    
    cout << "Nuevo nombre (" << pacientes -> Nombre_paciente << "): ";
    cin.getline(input, 100);
    
    if (strlen(input) > 0){

        strncpy(pacientes -> Nombre_paciente, input, 49);

    }
    
    cout << "Nuevo apellido (" << pacientes -> Apellido_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(pacientes -> Apellido_paciente, input, 49);

    }
    
    cout << "Nuevo teléfono (" << pacientes -> Telefono_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(pacientes -> Telefono_paciente, input, 14);

    }
    
    cout << "Nueva dirección (" << pacientes -> Direccion_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){

        strncpy(pacientes -> Direccion_paciente, input, 99);

    }
    
    cout << "Nuevo email (" << pacientes -> Email_paciente << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0){ 

        if (validarEmail(input)){

            strncpy(pacientes -> Email_paciente, input, 49);

        } else{

            cout << "Email inválido. No se actualizó." << endl;

        }
    }
    
    cout << "Nuevo tipo de sangre (" << pacientes -> Tipo_sangre << "): ";
    cin.getline(input, 100);

    if (strlen(input) > 0) {

        strncpy(pacientes -> Tipo_sangre, input, 4);

    }
    
    cout << "Datos del paciente actualizados exitosamente." << endl;

    return true;
    
}


/*
 * Elimina un paciente del sistema
 */

bool eliminarPaciente(Hospital* hospital, int ID_paciente){

    // Buscar paciente por ID
    Paciente* pacientes = buscarPacientePorId(hospital, ID_paciente);

    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;

    }

    // Liberar memoria de arrays dinámicos
    delete[] pacientes -> historialmedicos;
    delete[] pacientes -> citas;


}


/*
 * Lista todos los pacientes en formato tabular
 */

void listarPacientes(Hospital* hospital){

     cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                    LISTA DE PACIENTES                      ║" << endl;
    cout << "╠═════╦═════════════════════╦══════════════╦══════╦══════════╣" << endl;
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ CÉDULA       ║ EDAD ║ CONSULTAS║" << endl;
    cout << "╠═════╬═════════════════════╬══════════════╬══════╬══════════╣" << endl;

    // Rellenar los campos
    for (int i = 0; i < hospital->Cantidad_pacientes; i++) {

        Paciente& p = hospital->pacientes[i];
        
        if (p.Activo_Inactivo) {
            char Nombre_completo[100];
            snprintf(Nombre_completo, 100, "%s %s", p.Nombre_paciente, p.Apellido_paciente);
            
            cout << "║ " << setw(3) << p.ID_paciente << " ║ "
                 << setw(19) << left << Nombre_completo << " ║ "
                 << setw(12) << p.Cedula_paciente << " ║ "
                 << setw(4) << p.Edad_paciente << " ║ "
                 << setw(8) << p.Cantidad_consultas << "║" << endl;

        }

    }
    
    cout << "╚═════╩═════════════════════╩══════════════╩══════╩══════════╝" << endl;

}


// =============================================================================
// IMPLEMENTACIÓN DE FUNCIONES DE HISTORIAL MEDICO
// =============================================================================


/*
 * Agrega una consulta al historial médico del paciente
 */

void agregarConsultaAlHistorial(Paciente* pacientes, HistorialMedico consulta){

    // Redimensionar si es necesario
    if(pacientes -> Cantidad_consultas > pacientes -> Capacidad_consultas){

        int nuevaCapacidad_consultas = pacientes -> Capacidad_consultas * 2;

        HistorialMedico* nuevoArray_consultas = new HistorialMedico[nuevaCapacidad_consultas];

    // Copiar consultas existentes
    for (int i = 0; i < paciente -> Cantidad_consultas; i++) {

            nuevoArray_consultas[i] = pacientes -> historialesmedicos[i];

    }

    // Liberar array antiguo
    delete[] pacientes -> historialesmedicos;
        
    // Actualizar puntero y capacidad
    pacientes -> historialesmedicos = nuevoArray_consultas;

    pacientes -> Capacidad_consultas = nuevaCapacidad_consultas;

    cout << "Historial médico redimensionado a capacidad: " << nuevaCapacidad_consultas << endl;

    }

    // Agregar nueva consulta
    pacientes -> historialesmedicos[paciente->Cantidad_consultas] = consulta;
    pacientes -> Cantidad_consultas++;

}


/*
 * Obtiene el historial completo del paciente
 */

HistorialMedico* obtenerHistorialCompleto(Paciente* pacientes, int* cantidad){

    *cantidad = pacientes->cantidad_consultas;

    return pacientes->historialesmedicos;

}


/*
 * Muestra el historial médico del paciente
 */

void mostrarHistorialMedico(Paciente* pacientes){

    // Verificar si hay consultas registradas
    if (pacientes -> Cantidad_consultas == 0) {

        cout << "El paciente no tiene consultas registradas." << endl;

        return;
    }

     cout << "\n╔══════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                         HISTORIAL MÉDICO - " << paciente -> Nombre_paciente << " " << paciente->Apellido_paciente << setw(25) << "║" << endl;
    cout << "╠════════╦════════════╦════════╦══════════════════════════════════════════════════╣" << endl;
    cout << "║ FECHA  ║   HORA     ║ DOCTOR ║ DIAGNÓSTICO                                      ║" << endl;
    cout << "╠════════╬════════════╬════════╬══════════════════════════════════════════════════╣" << endl;

    //Rellenar los campos
    for (int i = 0; i < pacientes->Cantidad_consultas; i++) {

        HistorialMedico& consulta = pacientes->historialesmedicos[i];

        cout << "║ " << setw(6) << consulta.Fecha << " ║ "
             << setw(8) << consulta.Hora << " ║ "
             << setw(6) << consulta.ID_doctor << " ║ "
             << setw(50) << left << consulta.Diagnostico << "║" << endl;
    }
    
    cout << "╚════════╩════════════╩════════╩══════════════════════════════════════════════════╝" << endl;

}


/**
 * Obtiene la última consulta del paciente
 */

HistorialMedico* obtenerUltimaConsulta(Paciente* pacientes) {

    if (pacientes -> Cantidad_consultas == 0) {

        return nullptr;
    }

    return &pacientes -> historialesmedicos[paciente->Cantidad_consultas - 1];
}


// =============================================================================
// IMPLEMENTACIÓN DE MODULO DE GESTION DE DOCTORES
// =============================================================================


/**
 * Crea un nuevo doctor en el sistema
 */

Doctor* crearDoctor(Hospital* hospital, const char* Nombre_doctor, const char* Apellido_doctor, const char* Cedula_doctor,const char* Especialidad, int Anios_experiencia, float Costo_consulta){

    // Verificar cédula única
    for (int i = 0; i < hospital -> cantidadDoctores; i++){

        if (strcasecmp(hospital -> doctores[i].Cedula_doctor, Cedula_doctor) == 0){

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
    if (hospital -> Cantidad_doctores >= hospital -> Capacidad_doctores){

        redimensionarArrayDoctores(hospital);

    }

    // Crear nuevo doctor
    Doctor* Nuevo_doctor = &hospital -> doctores[hospital -> Cantidad_doctores];
    
    // Datos básicos
    Nuevo_doctor -> ID_doctor = hospital -> siguienteID_doctor++;
    strncpy(Nuevo_doctor -> Nombre_doctor, Nombre_doctor, 49);
    strncpy(Nuevo_doctor -> Apellido_doctor, Apellido_doctor, 49);
    strncpy(Nuevo_doctor -> Cedula_doctor, Cedula_doctor, 19);
    strncpy(Nuevo_doctor -> Especialidad_doctor, Especialidad, 49);
    Nuevo_doctor -> Anios_experiencia = Anios_experiencia;
    Nuevo_doctor -> Costo_Consulta = Costo_consulta;
    
    // Inicializar arrays dinámicos
    Nuevo_doctor -> Capacidad_pAgendados = 5;
    Nuevo_doctor -> Pacientes_agendados = new int[Nuevo_doctor->Capacidad_pAgendados];
    Nuevo_doctor -> Cantidad_pAgendados = 0;
    
    Nuevo_doctor -> Capacidad_cAgendadas = 10;
    Nuevo_doctor -> Citas_agendadas = new int[Nuevo_doctor->Capacidad_cAgendadas];
    Nuevo_doctor -> Cantidad_cAgendadas = 0;
    
    // Inicializar strings vacíos
    strcpy(Nuevo_doctor -> Horario_atencion, "");
    strcpy(Nuevo_doctor -> Telefono_Doctor, "");
    strcpy(Nuevo_doctor -> Email_Doctor, "");
    
    Nuevo_doctor -> Disponibilidad = true;
    
    hospital -> Cantidad_doctores++;
    
    cout << "Doctor creado exitosamente. ID: " << Nuevo_doctor -> ID_doctor << endl;

    return Nuevo_doctor;

}


/*
 * Busca doctor por ID
 */

Doctor* buscarDoctorPorId(Hospital* hospital, int ID_doctor){

    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (hospital -> doctores[i].ID_doctor == id && hospital->doctores[i].Disponibilidad){

            return &hospital -> doctores[i];

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

        if (hospital -> doctores[i].Disponibilidad && strcasecmp(hospital -> doctores[i].Especialidad, Especialidad) == 0){

            (*cantidad)++;

        }

    }
    
    if (*cantidad == 0) return nullptr;
    
    // Crear array de resultados
    Doctor** resultados = new Doctor*[*cantidad];
    int index = 0;
    
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        if (hospital -> doctores[i].Disponibilidad && strcasecmp(hospital -> doctores[i].Especialidad, Especialidad) == 0){

            resultados[index++] = &hospital -> doctores[i];

        }

    }
    
    return resultados;

}


/*
 * Asigna un paciente a un doctor
 */

bool asignarPacienteADoctor(Doctor* doctores, int ID_paciente){

    // Verificar si ya está asignado
    for (int i = 0; i < doctores -> Cantidad_pAgendados; i++){

        if (doctores -> Pacientes_agendados[i] == ID_paciente){

            cout << "El paciente ya está asignado a este doctor." << endl;

            return false;

        }

    }
    
    // Redimensionar si es necesario
    if (doctores -> Cantidad_pAgendados >= doctores -> Capacidad_pAgendados){

        int nuevaCapacidad_pAgendados = doctores -> Capacidad_pAgendados * 2;
        int* nuevoArray_pAgendados = new int[nuevaCapacidad_pAgendados];
        
        for (int i = 0; i < doctores -> Cantidad_pAgendados; i++){

            nuevoArray_pAgendados[i] = doctores -> Pacientes_agendados[i];

        }
        
        delete[] doctores -> Pacientes_agendados;
        doctores -> Pacientes_agendados = nuevoArray_pAgendados;
        doctores -> Capacidad_pAgendados = nuevaCapacidad_pAgendados;
    }
    
    // Asignar paciente
    doctores -> Pacientes_agendados[doctor->Cantidad_pAgendados] = ID_paciente;
    doctores -> Cantidad_pAgendados++;
    
    cout << "Paciente asignado exitosamente al doctor." << endl;

    return true;

}


/*
 * Remueve un paciente de un doctor
 */

bool removerPacienteDeDoctor(Doctor* doctores, int ID_paciente){
    int posicion = -1;
    
    // Buscar paciente
    for (int i = 0; i < doctores -> Cantidad_pAgendados; i++){

        if (doctores -> Pacientes_agendados[i] == ID_paciente){

            posicion = i;

            break;

        }
    }
    
    if (posicion == -1){

        cout << "El paciente no está asignado a este doctor." << endl;
        return false;

    }
    
    // Compactar array
    for (int i = posicion; i < doctores -> Cantidad_pAgendados - 1; i++){
        doctores -> Pacientes_agendados[i] = doctores ->Pacientes_agendados[i + 1];
    }
    
    doctores -> Cantidad_pAgendados--;

    cout << "Paciente removido exitosamente del doctor." << endl;

    return true;

}

/*
 * Lista pacientes asignados a un doctor
 */
void listarPacientesDeDoctor(Hospital* hospital, int ID_doctor){

    Doctor* doctores = buscarDoctorPorId(hospital, ID_doctores);

    if (doctores == nullptr){

        cout << "Doctor no encontrado." << endl;

        return;

    }
    
    if (doctores -> Cantidad_pAgendados == 0){

        cout << "El doctor no tiene pacientes agendados." << endl;

        return;

    }
    
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║           PACIENTES ASIGNADOS - Dr. " << doctores -> Nombre_doctor << " " << doctores -> Apellido_doctor << setw(15) << "║" << endl;
    cout << "╠═════╦═════════════════════╦══════════════╦══════╦══════════╣" << endl;
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ CÉDULA       ║ EDAD ║ CONSULTAS║" << endl;
    cout << "╠═════╬═════════════════════╬══════════════╬══════╬══════════╣" << endl;
    
    for (int i = 0; i < doctor -> Cantidad_pAgendados; i++){

        Paciente* pacientes = buscarPacientePorId(hospital, doctores -> Pacientes_agendados[i]);

        if (pacientes != nullptr && pacientes -> Activo_Inactivo){
            char Nombre_completo[100];
            snprintf(Nombre_completo, 100, "%s %s", pacientes -> Nombre_paciente, pacientes -> Apellido_paciente);
            
            cout << "║ " << setw(3) << pacientes -> ID_paciente << " ║ "
                 << setw(19) << left << Nombre_completo << " ║ "
                 << setw(12) << pacientes -> Cedula_paciente << " ║ "
                 << setw(4) << pacientes -> Edad_paciente << " ║ "
                 << setw(8) << pacientes -> Cantidad_consultas << "║" << endl;

        }
    }

    
    cout << "╚═════╩═════════════════════╩══════════════╩══════╩══════════╝" << endl;

}


/*
 * Lista todos los doctores en formato tabular
 */

void listarDoctores(Hospital* hospital){

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                            LISTA DE DOCTORES                                ║" << endl;
    cout << "╠═════╦═════════════════════╦══════════════════════╦══════╦══════════╦════════╣" << endl;
    cout << "║ ID  ║ NOMBRE COMPLETO     ║ ESPECIALIDAD         ║ EXP. ║ COSTO    ║ PAC.   ║" << endl;
    cout << "╠═════╬═════════════════════╬══════════════════════╬══════╬══════════╬════════╣" << endl;
    
    for (int i = 0; i < hospital -> Cantidad_doctores; i++){

        Doctor& d = hospital -> Doctores[i];

        if (d.Disponibilidad){

            char Nombre_completo[100];
            snprintf(Nombre_completo, 100, "%s %s", d.Nombre_doctor, d.Apellido_doctor);
            
            cout << "║ " << setw(3) << d.ID_doctor << " ║ "
                 << setw(19) << left << nombreCompleto << " ║ "
                 << setw(20) << d.Especialidad << " ║ "
                 << setw(4) << d.Anios_experiencia << " ║ $"
                 << setw(7) << fixed << setprecision(2) << d.Costo_consulta << " ║ "
                 << setw(6) << d.Cantidad_pAgendados << "║" << endl;

        }

    }
    
    cout << "╚═════╩═════════════════════╩══════════════════════╩══════╩══════════╩════════╝" << endl;

}


/*
 * Elimina un doctor del sistema
 */
bool eliminarDoctor(Hospital* hospital, int ID_doctor){

    Doctor* doctores = buscarDoctorPorId(hospital, ID_doctor);

    if (doctor == nullptr){

        cout << "Doctor no encontrado." << endl;

        return false;

    }
    
    // Liberar memoria de arrays dinámicos
    delete[] doctores -> Pacientes_agendados;
    delete[] doctores -> Citas_agendadas;
    
    // Marcar como no disponible
    doctores -> Disponibilidad = false;
    
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
    Paciente* pacientes = buscarPacientePorId(hospital, ID_paciente);
    Doctor* doctores = buscarDoctorPorId(hospital, ID_doctor);
    
    if (pacientes == nullptr){

        cout << "Paciente no encontrado." << endl;

        return nullptr;

    }

    if (doctores == nullptr){

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
    Cita* Nueva_cita = &hospital -> citas[hospital -> Cantidad_citas];
    
    Nueva_cita -> ID_cita = hospital -> siguienteIdCita++;
    Nueva_cita -> ID_paciente = ID_paciente;
    Nueva_cita -> ID_doctor = ID_doctor;
    strncpy(Nueva_cita -> Fecha, Fecha, 10);
    strncpy(Nueva_cita -> Hora, Hora, 5);
    strncpy(Nueva_cita -> Motivo_consulta, Motivo_consulta, 149);
    strcpy(Nueva_cita -> Estado_cita, "PENDIENTE");
    strcpy(Nueva_cita -> Observaciones_cita, "");
    Nueva_cita -> Atendida = false;
    
    hospital -> Cantidad_citas++;
    
    // Agregar cita al paciente
    if (pacientes -> Cantidad_citas >= pacientes -> Capacidad_citas){

        int Nueva_capacidad = pacientes -> Capacidad_cita * 2;
        int* nuevoArray_citas = new int[Nueva_capacidad];
        
        for (int i = 0; i < pacientes -> Cantidad_citas; i++){

            nuevoArray_citas[i] = pacientes -> Citas_agendadas[i];

        }
        
        delete[] pacientes -> Citas_agendadas;
        pacientes -> Citas_agendadas = nuevoArray_pacientes;
        pacientes -> Capacidad_citas = Nueva_capacidad;
    }

    pacientes -> Citas_agendadas[pacientes -> Cantidad_citas++] = Nueva_cita -> ID_cita;
    
    // Agregar cita al doctor
    if (doctores -> Cantidad_cAgendadas >= doctores -> Capacidad_cAgendadas){

        int Nueva_capacidad = doctores -> Capacidad_cAgendadas * 2;
        int* nuevoArray_cAgendadas = new int[Nueva_capacidad];
        
        for (int i = 0; i < doctores -> Cantidad_cAgendadas; i++){

            nuevoArray_cAgendadas[i] = doctores -> Citas_angedadas[i];

        }
        
        delete[] doctores -> Citas_agendadas;
        doctores ->Citas_agendadas = nuevoArray_cAgendadas;
        doctores -> Capacidad_cAgendadas = nuevaCapacidad_cAgendadas;
    }

    doctores -> Citas_agendadas[doctores -> Capacidad_cAgendadas++] = Nueva_cita -> ID_cita;
    
    cout << "Cita agendada exitosamente. ID: " << Nueva_cita ->ID_cita << endl;

    return Nueva_cita;

}


/*
 * Cancela una cita existente
 */

bool cancelarCita(Hospital* hospital, int ID_cita){
    Cita* citas = nullptr;
    int posicion = -1;
    
    // Buscar cita
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_cita == ID_cita){

            citas = &hospital->citas[i];
            posicion = i;

            break;

        }

    }
    
    if (citas == nullptr){

        cout << "Cita no encontrada." << endl;

        return false;

    }
    
    if (citas -> Atendida){

        cout << "No se puede cancelar una cita ya atendida." << endl;

        return false;

    }
    
    // Actualizar estado
    strcpy(citas -> Estado, "CANCELADA");
    
    cout << "Cita cancelada exitosamente." << endl;

    return true;
}


/*
 * Atiende una cita y registra en historial
 */

bool atenderCita(Hospital* hospital, int ID_cita, const char* Diagnostico,
                 const char* Tratamiento_preescrito, const char* Medicamentos_recetados){
    
    Cita* cita = nullptr;
    
    // Buscar cita
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        if (hospital -> citas[i].ID_cita == ID_cita){

            citas = &hospital -> citas[i];

            break;

        }
    }
    
    if (citas == nullptr){

        cout << "Cita no encontrada." << endl;

        return false;

    }
    
    if (citas -> Atendida){

        cout << "La cita ya fue atendida anteriormente." << endl;

        return false;

    }
    
    // Buscar paciente
    Paciente* pacientes = buscarPacientePorId(hospital, citas-> ID_paciente);

    if (pacientes == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;

    }
    
    // Crear registro de historial
    HistorialMedico Nueva_consulta;

    Nueva_consulta.ID_consulta = hospital -> siguienteIdConsulta++;
    strcpy(Nueva_consulta.Fecha, cita-> Fecha);
    strcpy(Nueva_consulta.Hora, cita-> Hora);
    strncpy(Nueva_consulta.Diagnostico, Diagnostico, 199);
    strncpy(Nueva_consulta.Tratamiento_preescrito, Tratamiento_preescrito, 199);
    strncpy(Nueva_consulta.Medicamentos_recetados, Medicamentos_recetados, 149);
    Nueva_consulta.ID_doctor = citas-> ID_doctor;
    
    // Obtener costo del doctor
    Doctor* doctores = buscarDoctorPorId(hospital, citas -> ID_doctor);
    if (doctores != nullptr){

        Nueva_consulta.Costo_consulta = doctores -> Costo_consulta;

    } else{
    
        Nueva_consulta.Costp_consulta = 0.0f;
    }
    
    // Agregar al historial
    agregarConsultaAlHistorial(paciente, Nueva_consulta);
    
    // Actualizar cita
    citas -> Atendida = true;
    strcpy(citas -> Estado_citas, "ATENDIDA");
    strncpy(citas -> Observaciones_citas, Diagnostico, 199);
    
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
    
    for (int i = 0; i < hospital -> Cantidad_citad; i++){

        if (hospital -> citas[i].ID_paciente == ID_paciente){

            resultados[index++] = &hospital->citas[i];

        }

    }
    
    return resultados;

}


/*
 * Obtiene todas las citas de un doctor
 */
Cita** obtenerCitasDeDoctor(Hospital* hospital, int ID_doctor, int* cantidad) {
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

        cout << " Fecha inválida." << endl;

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
            resultados[index++] = &hospital -> citas[i];
        }
    }
    
    return resultados;
}


/**
 * Lista citas pendientes
 */

 void listarCitasPendientes(Hospital* hospital){

    cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                            CITAS PENDIENTES                                 ║" << endl;
    cout << "╠═════╦═══════╦═══════╦══════════╦════════╦═══════════════════════════════════╣" << endl;
    cout << "║ ID  ║ PAC.  ║ DOC.  ║ FECHA    ║ HORA   ║ MOTIVO                           ║" << endl;
    cout << "╠═════╬═══════╬═══════╬══════════╬════════╬═══════════════════════════════════╣" << endl;
    
    bool Hay_citas = false;
    
    for (int i = 0; i < hospital -> Cantidad_citas; i++){
        Cita& c = hospital -> citas[i];

        if (!c.Atendida && strcmp(c.Estado_cita, "CANCELADA") != 0){
            hayCitas = true;
            
            // Obtener nombres
            Paciente* p = buscarPacientePorId(hospital, c.ID_paciente);
            Doctor* d = buscarDoctorPorId(hospital, c.ID_doctor);
            
            char Nombre_paciente[30] = "N/A";
            char Nombre_doctor[30] = "N/A";
            
            if (p != nullptr) {

                snprintf(Nombre_paciente, 30, "%s", p -> Nombre_paciente);

            }

            if (d != nullptr) {

                snprintf(Nombre_doctor, 30, "%s", d->Nombre_doctor);

            }
            
            cout << "║ " << setw(3) << c.ID_cita << " ║ "
                 << setw(5) << Nombre_paciente << " ║ "
                 << setw(5) << Nombre_doctor << " ║ "
                 << setw(8) << c.Fecha << " ║ "
                 << setw(6) << c.Hora << " ║ "
                 << setw(35) << left << c.Motivo_consulta << "║" << endl;

        }

    }
    
    if (!hayCitas){

        cout << "║                      NO HAY CITAS PENDIENTES                           ║" << endl;

    }
    
    cout << "╚═════╩═══════╩═══════╩══════════╩════════╩═══════════════════════════════════╝" << endl;

}


/*
 * Verifica disponibilidad de doctor en fecha y hora específicas
 */
bool verificarDisponibilidad(Hospital* hospital, int ID_doctor, const char* Fecha, const char* Hora){
    for (int i = 0; i < hospital -> Cantidad_citas; i++){

        Cita& c = hospital -> citas[i];
        if (c.ID_doctor == ID_doctor && strcmp(c.Fecha, Fecha) == 0 && strcmp(c.Hora, Hora) == 0 && !c.Atendida && strcmp(c.Estado, "CANCELADA") != 0){

            return false; // Ya hay una cita a esa hora

        }

    }

    return true;

}

int main(){

   

};