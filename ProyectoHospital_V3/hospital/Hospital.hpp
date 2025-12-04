#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <cstring>
#include <ctime>

class Hospital {
private:

    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;
    
public:

    //Constructores
    Hospital();
    Hospital(const char* nombre, const char* direccion, const char* telefono);
    Hospital(const Hospital& otro);
    
    //Destructor
    ~Hospital();
    
    //Getters
    const char* getNombre() const;
    const char* getDireccion() const;
    const char* getTelefono() const;
    int getSiguienteIDPaciente() const;
    int getSiguienteIDDoctor() const;
    int getSiguienteIDCita() const;
    int getSiguienteIDConsulta() const;
    int getTotalPacientesRegistrados() const;
    int getTotalDoctoresRegistrados() const;
    int getTotalCitasAgendadas() const;
    int getTotalConsultasRealizadas() const;
    
    //Setters
    void setNombre(const char* nombre);
    void setDireccion(const char* direccion);
    void setTelefono(const char* telefono);
    void setSiguienteIDPaciente(int id);
    void setSiguienteIDDoctor(int id);
    void setSiguienteIDCita(int id);
    void setSiguienteIDConsulta(int id);
    void setTotalPacientesRegistrados(int cantidad);
    void setTotalDoctoresRegistrados(int cantidad);
    void setTotalCitasAgendadas(int cantidad);
    void setTotalConsultasRealizadas(int cantidad);
    
    //Metodos de Gestion
    int generarNuevoIDPaciente();
    int generarNuevoIDDoctor();
    int generarNuevoIDCita();
    int generarNuevoIDConsulta();
    
    void incrementarPacientesRegistrados();
    void incrementarDoctoresRegistrados();
    void incrementarCitasAgendadas();
    void incrementarConsultasRealizadas();
    
    void decrementarPacientesRegistrados();
    void decrementarDoctoresRegistrados();
    void decrementarCitasAgendadas();
    void decrementarConsultasRealizadas();
    
    //Metodos de Validacion
    bool validarDatos() const;
    
    //Metodos de Presentacion
    void mostrarInformacion() const;
    void mostrarEstadisticas() const;
    
    //Operadores
    Hospital& operator=(const Hospital& otro);
    
    //Metodo Estatico
    static size_t obtenerTamanio();
};

#endif