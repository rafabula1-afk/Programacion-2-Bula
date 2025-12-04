#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <cstring>
#include <cctype>
#include <ctime>
#include <iostream>

using namespace std;

class Validaciones {
public:

    //Validaciones Generales
    static bool validarCedula(const char* cedula);
    static bool validarEmail(const char* email);
    static bool validarTelefono(const char* telefono);
    static bool validarFecha(const char* fecha);
    static bool validarRango(int valor, int min, int max);
    static bool validarSoloLetras(const char* texto);
    static bool validarSoloNumeros(const char* texto);
    static bool validarEdad(int edad);
    static bool validarTipoSangre(const char* tipoSangre);
    static bool validarHora(const char* hora);
    
    //Validaciones Especificas
    static bool validarCedulaProfesional(const char* cedula);
    static bool validarEspecialidad(const char* especialidad);
    static bool validarCosto(float costo);
    static bool validarAniosExperiencia(int anios);
    
    //Funciones de Comparacion
    static bool compararCadenasCaseInsensitive(const char* str1, const char* str2);
};

#endif