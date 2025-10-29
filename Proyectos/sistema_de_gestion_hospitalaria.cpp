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
    char Fecha_consulta[11];
    char Hora_consulta[6];
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

    int* Pacientes_asignados;
    int Cantidad_pAsignados;
    int Capacidad_totalpAG;

    int* Citas_agendadas;
    int Cantidad_citAgendadas;
    int Capacidad_totalcitAG;

    bool Disponibilidad;

};

struct Cita
{
    int ID_cita;
    Paciente* pacientes;
    Doctor* doctores;
    char Fecha_cita[11];
    char Hora_cita[6];
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

bool validarFecha(const char* fecha);
bool validarHora(const char* hora);
int compararFechas(const char* fecha1, const char* fecha2);
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

void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta);
HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad);
void mostrarHistorialMedico(Paciente* paciente);
HistorialMedico* obtenerUltimaConsulta(Paciente* paciente);


// =============================================================================
// IMPLEMENTACIÓN DE GESTIÓN DE MEMORIA
// =============================================================================


/*
 * Inicializa la estructura Hospital con capacidad inicial
 */

Hospital* inicializarHospital(const char* Nombre_hospital, int capacidadIncial){
    
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

bool validarFecha(const char* fecha){

    // Valida el formato de fecha YYYY-MM-DD
    if(strlen(fecha) != 10) return false;
    if(fecha[4] != '-' || fecha[7] != '-') return false;

    // Extraer año, mes y dia
    int anio, mes, dia;
    if(sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) !=3) return false;

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

 bool validarHora(const char* hora){

    // Validar el formato de hora HH:MM
    if(strlen(hora) != 5) return false;
    if(hora[2] != ':') return false;

    // Extraer horas y minutos
    int hh, mm;
    if(sscanf(hora, "%d:%d", &hh, &mm) != 2) return false;

    // Validar rangos basicos
    return (hh >= 0 && hh <= 23) && (mm >= 0 && mm <= 59);

 }


/*
 * Valida la hora establecida
 */

int compararFechas(const char* fecha1, const char* fecha2){

    return strcmp(fecha1, fecha2);  // Retorna -1 si fecha1 < fecha 2
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
    Paciente* Nuevo_paciente = &hospital -> pacientes[hospital->cantidadPacientes];

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

    for (int i = 0; i < hospita l-> Cantidad_pacientes; i++) {

        if (hospital -> pacientes[i].ID_paciente == ID_paciente && hospital->pacientes[i].activo) {

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
    cout << "Paciente: " << paciente -> Nombre_paciente << " " << paciente->Nombre_paciente << endl;
    
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

    if (strlen(input) > 0) {

        strncpy(paciente -> Tipo_sangre, input, 4);

    }
    
    cout << "Datos del paciente actualizados exitosamente." << endl;

    return true;
    
}


/*
 * Elimina un paciente del sistema
 */

bool eliminarPaciente(Hospital* hospital, int ID_paciente){

    // Buscar paciente por ID
    Paciente* paciente = buscarPacientePorId(hospital, ID_paciente);

    if (paciente == nullptr){

        cout << "Paciente no encontrado." << endl;

        return false;

    }

    // Liberar memoria de arrays dinámicos
    delete[] paciente -> historialmedicos;
    delete[] paciente -> citas;


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
    for (int i = 0; i < hospital->cantidadPacientes; i++) {

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

void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta){

    // Redimensionar si es necesario
    if(paciente -> Cantidad_consultas > paciente -> Capacidad_consultas){

        int nuevaCapacidad_consultas = paciente -> Capacidad_consultas * 2;

        HistorialMedico* nuevoArray_consultas = new HistorialMedico[nuevaCapacidad_consultas];

    // Copiar consultas existentes
    for (int i = 0; i < paciente -> Cantidad_consultas; i++) {

            nuevoArray_consultas[i] = paciente -> historialesmedicos[i];

    }

    // Liberar array antiguo
    delete[] paciente -> historialesmedicos;
        
    // Actualizar puntero y capacidad
    paciente -> historialesmedicos = nuevoArray_consultas;

    paciente -> Capacidad_consultas = nuevaCapacidad_consultas;

    cout << "Historial médico redimensionado a capacidad: " << nuevaCapacidad_consultas << endl;

    }

    // Agregar nueva consulta
    paciente -> historialesmedicos[paciente->cantidadConsultas] = consulta;
    paciente -> cantidadConsultas++;

}


/*
 * Obtiene el historial completo del paciente
 */

HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad){

    *cantidad = paciente->cantidad_consultas;

    return paciente->historialesmedicos;

}


/*
 * Muestra el historial médico del paciente
 */

void mostrarHistorialMedico(Paciente* paciente){

    // Verificar si hay consultas registradas
    if (paciente -> Cantidad_consultas == 0) {

        cout << "El paciente no tiene consultas registradas." << endl;

        return;
    }

     cout << "\n╔══════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                         HISTORIAL MÉDICO - " << paciente -> Nombre_paciente << " " << paciente->Apellido_paciente << setw(25) << "║" << endl;
    cout << "╠════════╦════════════╦════════╦══════════════════════════════════════════════════╣" << endl;
    cout << "║ FECHA  ║   HORA     ║ DOCTOR ║ DIAGNÓSTICO                                      ║" << endl;
    cout << "╠════════╬════════════╬════════╬══════════════════════════════════════════════════╣" << endl;

    //Rellenar los campos
    for (int i = 0; i < paciente->Cantidad_consultas; i++) {

        HistorialMedico& consulta = paciente->historialesmedicos[i];

        cout << "║ " << setw(6) << consulta.Fecha_consulta << " ║ "
             << setw(8) << consulta.Hora_consulta << " ║ "
             << setw(6) << consulta.ID_doctor << " ║ "
             << setw(50) << left << consulta.Diagnostico << "║" << endl;
    }
    
    cout << "╚════════╩════════════╩════════╩══════════════════════════════════════════════════╝" << endl;

}


/**
 * Obtiene la última consulta del paciente
 */

HistorialMedico* obtenerUltimaConsulta(Paciente* paciente) {

    if (paciente->Cantidad_consultas == 0) {

        return nullptr;
    }

    return &paciente->historialesmedicos[paciente->Cantidad_consultas - 1];
}





    

int main(){

   

};