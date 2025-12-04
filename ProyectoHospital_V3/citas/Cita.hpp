#ifndef CITA_HPP
#define CITA_HPP

#include <cstring>
#include <ctime>

class Cita {

private:

    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
    
    int consultaID;
    
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
    
public:

    //Constructores
    Cita();
    Cita(int id, int pacienteID, int doctorID, const char* fecha, const char* hora, const char* motivo);
    Cita(const Cita& otro);
    
    //Destructor
    ~Cita();
    
    //Getters
    int getId() const;
    int getPacienteID() const;
    int getDoctorID() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getMotivo() const;
    const char* getEstado() const;
    const char* getObservaciones() const;
    bool isAtendida() const;
    int getConsultaID() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;
    
    //Setters
    void setId(int id);
    void setPacienteID(int pacienteID);
    void setDoctorID(int doctorID);
    void setFecha(const char* fecha);
    void setHora(const char* hora);
    void setMotivo(const char* motivo);
    void setEstado(const char* estado);
    void setObservaciones(const char* observaciones);
    void setAtendida(bool atendida);
    void setConsultaID(int consultaID);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModificacion(time_t fecha);
    
    //Metodos de Validacion
    bool validarDatos() const;
    bool fechaHoraValidas() const;
    bool pacienteYDoctorExisten() const;
    
    //Metodos de Gestion
    void marcarComoAtendida();
    void cancelar();
    void reprogramar(const char* nuevaFecha, const char* nuevaHora);
    bool estaPendiente() const;
    bool estaCancelada() const;
    bool estaAtendida() const;
    
    //Metodos de Presentacion
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    //Operadores
    Cita& operator=(const Cita& otro);
    
    //Metodo Estatico
    static size_t obtenerTamanio();
};

#endif