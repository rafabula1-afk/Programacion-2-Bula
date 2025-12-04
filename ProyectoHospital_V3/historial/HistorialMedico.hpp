#ifndef HISTORIALMEDICO_HPP
#define HISTORIALMEDICO_HPP

#include <cstring>
#include <ctime>

class HistorialMedico {
private:
    int id;
    int pacienteID;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    
    int siguienteConsultaID;
    
    bool eliminado;
    time_t fechaRegistro;
    
public:
    //Constructores
    HistorialMedico();
    HistorialMedico(int id, int pacienteID, int doctorID, const char* fecha, const char* hora, const char* diagnostico);
    HistorialMedico(const HistorialMedico& otro);
    
    //Destructor
    ~HistorialMedico();
    
    //Getters
    int getId() const;
    int getPacienteID() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getDiagnostico() const;
    const char* getTratamiento() const;
    const char* getMedicamentos() const;
    int getDoctorID() const;
    float getCosto() const;
    int getSiguienteConsultaID() const;
    bool isEliminado() const;
    time_t getFechaRegistro() const;
    
    //Setters
    void setId(int id);
    void setPacienteID(int pacienteID);
    void setFecha(const char* fecha);
    void setHora(const char* hora);
    void setDiagnostico(const char* diagnostico);
    void setTratamiento(const char* tratamiento);
    void setMedicamentos(const char* medicamentos);
    void setDoctorID(int doctorID);
    void setCosto(float costo);
    void setSiguienteConsultaID(int siguienteConsultaID);
    void setEliminado(bool eliminado);
    void setFechaRegistro(time_t fecha);
    
    //Metodos de Validacion
    bool validarDatos() const;
    bool fechaHoraValidas() const;
    bool costoValido() const;
    
    //Metodos de Navegacion
    bool tieneSiguienteConsulta() const;
    void enlazarSiguienteConsulta(int siguienteID);
    void desenlazarSiguienteConsulta();
    
    //Metodos de Presentacion
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    //Operadores
    HistorialMedico& operator=(const HistorialMedico& otro);
    
    //Metodo Estatico
    static size_t obtenerTamanio();
};

#endif