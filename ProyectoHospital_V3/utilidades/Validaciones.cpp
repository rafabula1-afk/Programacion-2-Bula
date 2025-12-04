#include "Validaciones.hpp"
#include <cstring>
#include <cctype>
#include <ctime>
#include <iostream>

using namespace std;

//Validaciones Generales
bool Validaciones::validarCedula(const char* cedula) {
    if (!cedula || strlen(cedula) == 0) return false;

    if (strlen(cedula) < 5 || strlen(cedula) > 20) return false;

    for (int i = 0; cedula[i] != '\0'; i++) {
        if (!isdigit(cedula[i]) && !isalpha(cedula[i]) && cedula[i] != '-') {
            return false;
        }
    }
    
    return true;
}

bool Validaciones::validarEmail(const char* email) {
    if (!email || strlen(email) == 0) return false;
    
    bool tieneArroba = false;
    bool tienePuntoDespuesArroba = false;
    bool tieneCaracterAntesArroba = false;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            if (tieneArroba) return false;
            tieneArroba = true;
            if (i == 0) return false;
            tieneCaracterAntesArroba = (i > 0);
        } else if (email[i] == '.' && tieneArroba) {
            tienePuntoDespuesArroba = true;
        }
    }
    
    return tieneArroba && tienePuntoDespuesArroba && tieneCaracterAntesArroba;
}

bool Validaciones::validarTelefono(const char* telefono) {
    if (!telefono || strlen(telefono) == 0) return false;

    int digitos = 0;
    for (int i = 0; telefono[i] != '\0'; i++) {
        if (isdigit(telefono[i])) {
            digitos++;
        } else if (telefono[i] != ' ' && telefono[i] != '-' && 
                  telefono[i] != '(' && telefono[i] != ')') {
            return false;
        }
    }

    return digitos >= 7 && digitos <= 15;
}

bool Validaciones::validarFecha(const char* fecha) {
    if (!fecha || strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    int anio, mes, dia;
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return false;

    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (bisiesto && dia > 29) return false;
        if (!bisiesto && dia > 28) return false;
    } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return false;
    }

    time_t ahora = time(nullptr);
    struct tm* tiempoActual = localtime(&ahora);
    int anioActual = tiempoActual->tm_year + 1900;
    int mesActual = tiempoActual->tm_mon + 1;
    int diaActual = tiempoActual->tm_mday;
    
    if (anio > anioActual) return false;
    if (anio == anioActual && mes > mesActual) return false;
    if (anio == anioActual && mes == mesActual && dia > diaActual) return false;
    
    return true;
}

bool Validaciones::validarRango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

bool Validaciones::validarSoloLetras(const char* texto) {
    if (!texto) return false;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isalpha(texto[i]) && texto[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool Validaciones::validarSoloNumeros(const char* texto) {
    if (!texto) return false;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isdigit(texto[i])) {
            return false;
        }
    }
    return true;
}

bool Validaciones::validarEdad(int edad) {
    return edad >= 0 && edad <= 120;
}

bool Validaciones::validarTipoSangre(const char* tipoSangre) {
    if (!tipoSangre) return false;
    
    const char* tiposValidos[] = {"O+", "O-", "A+", "A-", "B+", "B-", "AB+", "AB-"};
    for (int i = 0; i < 8; i++) {
        if (strcasecmp(tipoSangre, tiposValidos[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool Validaciones::validarHora(const char* hora) {
    if (!hora || strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int hh, mm;
    if (sscanf(hora, "%d:%d", &hh, &mm) != 2) return false;

    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;

    if (hh < 8 || hh > 20) return false;
    
    return true;
}

//Validaciones Especificas
bool Validaciones::validarCedulaProfesional(const char* cedula) {
    if (!cedula || strlen(cedula) == 0) return false;
    
    if (strlen(cedula) < 5 || strlen(cedula) > 15) return false;

    if (!isalpha(cedula[0])) return false;

    for (int i = 1; cedula[i] != '\0'; i++) {
        if (!isdigit(cedula[i])) {
            return false;
        }
    }
    
    return true;
}

bool Validaciones::validarEspecialidad(const char* especialidad) {
    if (!especialidad || strlen(especialidad) == 0) return false;
    
    const char* especialidadesValidas[] = {
        "Cardiologia", "Pediatria", "Neurologia", "Dermatologia",
        "Oftalmologia", "Traumatologia", "Ginecologia", "Urologia",
        "Psiquiatria", "Oncologia", "Endocrinologia", "Nefrologia",
        "Medicina General", "Cirugia", "Ortopedia", "Pediatria"
    };
    
    for (int i = 0; i < 16; i++) {
        if (strcasecmp(especialidad, especialidadesValidas[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool Validaciones::validarCosto(float costo) {
    return costo >= 0.0f && costo <= 10000.0f;
}

bool Validaciones::validarAniosExperiencia(int anios) {
    return anios >= 0 && anios <= 80;
}

//Funciones de Comparacion
bool Validaciones::compararCadenasCaseInsensitive(const char* str1, const char* str2) {
    if (!str1 || !str2) return false;
    
    while (*str1 && *str2) {
        if (tolower(*str1) != tolower(*str2)) {
            return false;
        }
        str1++;
        str2++;
    }
    
    return *str1 == *str2;
}