#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <cstring>
#include <ctime>

class Doctor {
private:

    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    
    int cantidadPacientes;
    int pacientesIDs[50];
    
    int cantidadCitas;
    int citasIDs[30];
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
    
public:

    //Constructores
    Doctor();
    Doctor(int id, const char* nombre, const char* apellido, const char* cedulaProfesional, const char* especialidad);
    Doctor(const Doctor& otro);
    
    //Destructor
    ~Doctor();
    
    //Getters
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedulaProfesional() const;
    const char* getEspecialidad() const;
    int getAniosExperiencia() const;
    float getCostoConsulta() const;
    const char* getHorarioAtencion() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    bool isDisponible() const;
    int getCantidadPacientes() const;
    const int* getPacientesIDs() const;
    int getCantidadCitas() const;
    const int* getCitasIDs() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;
    
    //Setters
    void setId(int id);
    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setCedulaProfesional(const char* cedula);
    void setEspecialidad(const char* especialidad);
    void setAniosExperiencia(int anios);
    void setCostoConsulta(float costo);
    void setHorarioAtencion(const char* horario);
    void setTelefono(const char* telefono);
    void setEmail(const char* email);
    void setDisponible(bool disponible);
    void setCantidadPacientes(int cantidad);
    void setCantidadCitas(int cantidad);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModificacion(time_t fecha);
    
    //Metodos de Validacion
    bool validarDatos() const;
    bool especialidadEsValida() const;
    bool cedulaProfesionalValida() const;
    bool costoConsultaValido() const;
    
    //Metodos de Gestion
    bool agregarPacienteID(int pacienteID);
    bool eliminarPacienteID(int pacienteID);
    bool agregarCitaID(int citaID);
    bool eliminarCitaID(int citaID);
    bool estaDisponible() const;
    void setDisponible(bool disponible);
    bool tienePacientes() const;
    bool tieneCitas() const;
    
    //Metodos de Presentacion
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    //Operadores
    Doctor& operator=(const Doctor& otro);
    
    //Metodo Estatico
    static size_t obtenerTamanio();
};

#endif