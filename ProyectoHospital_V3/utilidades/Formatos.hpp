#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <cstring>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

class Formatos {
public:

    //Formatos de Fecha y Hora
    static string formatearFecha(time_t tiempo);
    static string formatearHora(time_t tiempo);
    static string formatearFechaHora(time_t tiempo);
    static string formatearFechaCorta(time_t tiempo);
    
    //Formatos de Texto
    static void convertirAMayusculas(char* texto);
    static void convertirAMinusculas(char* texto);
    static string capitalizarPalabras(const char* texto);
    static string formatearTelefono(const char* telefono);
    static string formatearDinero(float cantidad);
    
    //Funciones de Entrada/Salida
    static void limpiarBuffer();
    static void pausar();
    static void mostrarSeparador(char caracter = '=', int longitud = 50);
    static void mostrarEncabezado(const char* titulo);
    
    //Funciones de Validación de Entrada
    static bool leerEntero(int& valor, const char* mensaje, int min = INT_MIN, int max = INT_MAX);
    static bool leerFloat(float& valor, const char* mensaje, float min = -FLT_MAX, float max = FLT_MAX);
    static bool leerCadena(char* buffer, int tamanio, const char* mensaje);
    static bool leerFecha(char* fecha, const char* mensaje);
    static bool leerHora(char* hora, const char* mensaje);
};

#endif