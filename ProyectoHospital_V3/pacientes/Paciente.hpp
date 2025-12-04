#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <cstring>
#include <ctime>

class Paciente {
private:

    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    bool activo;
    
    int cantidadConsultas;
    int primerConsultaID;
    
    int cantidadCitas;
    int citasIDs[20];
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
    
public:

    //Constructores
    Paciente();
    Paciente(int id, const char* nombre, const char* apellido, const char* cedula);
    Paciente(const Paciente& otro);
    
    //Destructor
    ~Paciente();
    
    //Getters
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedula() const;
    int getEdad() const;
    char getSexo() const;
    const char* getTipoSangre() const;
    const char* getTelefono() const;
    const char* getDireccion() const;
    const char* getEmail() const;
    const char* getAlergias() const;
    const char* getObservaciones() const;
    bool isActivo() const;
    int getCantidadConsultas() const;
    int getPrimerConsultaID() const;
    int getCantidadCitas() const;
    const int* getCitasIDs() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;
    
    //Setters
    void setId(int id);
    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setCedula(const char* cedula);
    void setEdad(int edad);
    void setSexo(char sexo);
    void setTipoSangre(const char* tipoSangre);
    void setTelefono(const char* telefono);
    void setDireccion(const char* direccion);
    void setEmail(const char* email);
    void setAlergias(const char* alergias);
    void setObservaciones(const char* observaciones);
    void setActivo(bool activo);
    void setCantidadConsultas(int cantidad);
    void setPrimerConsultaID(int id);
    void setCantidadCitas(int cantidad);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModificacion(time_t fecha);
    
    //Metodos de Validacion
    bool validarDatos() const;
    bool esMayorDeEdad() const;
    bool cedulaEsValida() const;
    bool tieneContactoEmergencia() const;
    
    //Metodos de Gestion de Citas
    bool agregarCitaID(int citaID);
    bool eliminarCitaID(int citaID);
    bool tieneCitas() const;
    
    //Metodos de Presentacion
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    //Operadores
    Paciente& operator=(const Paciente& otro);
    
    //Metodo Estatico
    static size_t obtenerTamanio();
};

#endif