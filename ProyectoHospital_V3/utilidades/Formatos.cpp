#include "Formatos.hpp"
#include <cstring>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <climits>
#include <algorithm>

using namespace std;

//Formatos de Fecha y Hora
string Formatos::formatearFecha(time_t tiempo) {
    struct tm* tiempoStruct = localtime(&tiempo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tiempoStruct);
    return string(buffer);
}

string Formatos::formatearHora(time_t tiempo) {
    struct tm* tiempoStruct = localtime(&tiempo);
    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tiempoStruct);
    return string(buffer);
}

string Formatos::formatearFechaHora(time_t tiempo) {
    struct tm* tiempoStruct = localtime(&tiempo);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tiempoStruct);
    return string(buffer);
}

string Formatos::formatearFechaCorta(time_t tiempo) {
    struct tm* tiempoStruct = localtime(&tiempo);
    char buffer[12];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tiempoStruct);
    return string(buffer);
}

//Formatos de Texto
void Formatos::convertirAMayusculas(char* texto) {
    if (!texto) return;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper(texto[i]);
    }
}

void Formatos::convertirAMinusculas(char* texto) {
    if (!texto) return;
    
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = tolower(texto[i]);
    }
}

string Formatos::capitalizarPalabras(const char* texto) {
    if (!texto) return "";
    
    string resultado(texto);
    bool nuevaPalabra = true;
    
    for (size_t i = 0; i < resultado.length(); i++) {
        if (nuevaPalabra && isalpha(resultado[i])) {
            resultado[i] = toupper(resultado[i]);
            nuevaPalabra = false;
        } else if (isspace(resultado[i])) {
            nuevaPalabra = true;
        } else {
            resultado[i] = tolower(resultado[i]);
        }
    }
    
    return resultado;
}

string Formatos::formatearTelefono(const char* telefono) {
    if (!telefono) return "";
    
    string telefonoStr(telefono);
    string resultado;

    for (char c : telefonoStr) {
        if (isdigit(c)) {
            resultado += c;
        }
    }

    if (resultado.length() == 10) {
        return "(" + resultado.substr(0, 3) + ") " + 
               resultado.substr(3, 3) + "-" + 
               resultado.substr(6, 4);
    } else if (resultado.length() == 7) {
        return resultado.substr(0, 3) + "-" + 
               resultado.substr(3, 4);
    }
    
    return telefonoStr;
}

string Formatos::formatearDinero(float cantidad) {
    stringstream ss;
    ss << fixed << setprecision(2) << "$" << cantidad;
    return ss.str();
}

//Funciones de Entrada/Salida
void Formatos::limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

void Formatos::pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

void Formatos::mostrarSeparador(char caracter, int longitud) {
    cout << string(longitud, caracter) << endl;
}

void Formatos::mostrarEncabezado(const char* titulo) {
    int longitud = strlen(titulo) + 4;
    cout << "\n";
    mostrarSeparador('=', longitud);
    cout << "  " << titulo << "  " << endl;
    mostrarSeparador('=', longitud);
}

//Funciones de Validacion de Entrada
bool Formatos::leerEntero(int& valor, const char* mensaje, int min, int max) {
    while (true) {
        cout << mensaje;
        string entrada;
        getline(cin, entrada);
        
        // Verificar si es número
        bool esNumero = true;
        for (char c : entrada) {
            if (!isdigit(c) && c != '-') {
                esNumero = false;
                break;
            }
        }
        
        if (!esNumero || entrada.empty()) {
            cout << "Error: Debe ingresar un numero entero valido." << endl;
            continue;
        }
        
        try {
            valor = stoi(entrada);
            
            if (valor < min || valor > max) {
                cout << "Error: El valor debe estar entre " << min << " y " << max << "." << endl;
                continue;
            }
            
            return true;
        } catch (const invalid_argument& e) {
            cout << "Error: Entrada invalida." << endl;
        } catch (const out_of_range& e) {
            cout << "Error: Numero fuera de rango." << endl;
        }
    }
}

bool Formatos::leerFloat(float& valor, const char* mensaje, float min, float max) {
    while (true) {
        cout << mensaje;
        string entrada;
        getline(cin, entrada);
        
        if (entrada.empty()) {
            cout << "Error: No puede estar vacio." << endl;
            continue;
        }

        bool tienePunto = false;
        bool esNumero = true;
        
        for (size_t i = 0; i < entrada.length(); i++) {
            char c = entrada[i];
            if (c == '.') {
                if (tienePunto) {
                    esNumero = false;
                    break;
                }
                tienePunto = true;
            } else if (!isdigit(c) && !(i == 0 && c == '-')) {
                esNumero = false;
                break;
            }
        }
        
        if (!esNumero) {
            cout << "Error: Formato invalido. Use numeros con punto decimal." << endl;
            continue;
        }
        
        try {
            valor = stof(entrada);
            
            if (valor < min || valor > max) {
                cout << "Error: El valor debe estar entre " << min << " y " << max << "." << endl;
                continue;
            }
            
            return true;
        } catch (const invalid_argument& e) {
            cout << "Error: Entrada invalida." << endl;
        } catch (const out_of_range& e) {
            cout << "Error: Numero fuera de rango." << endl;
        }
    }
}

bool Formatos::leerCadena(char* buffer, int tamanio, const char* mensaje) {
    while (true) {
        cout << mensaje;
        string entrada;
        getline(cin, entrada);
        
        if (entrada.empty()) {
            cout << "Error: No puede estar vacio." << endl;
            continue;
        }
        
        if (entrada.length() >= tamanio) {
            cout << "Error: Texto demasiado largo. Maximo " << (tamanio - 1) << " caracteres." << endl;
            continue;
        }
        
        strncpy(buffer, entrada.c_str(), tamanio - 1);
        buffer[tamanio - 1] = '\0';
        return true;
    }
}

bool Formatos::leerFecha(char* fecha, const char* mensaje) {
    while (true) {
        cout << mensaje << " (YYYY-MM-DD): ";
        string entrada;
        getline(cin, entrada);

        if (entrada.length() != 10 || entrada[4] != '-' || entrada[7] != '-') {
            cout << "Error: Formato invalido. Use YYYY-MM-DD." << endl;
            continue;
        }

        for (int i = 0; i < 10; i++) {
            if (i == 4 || i == 7) continue;
            if (!isdigit(entrada[i])) {
                cout << "Error: Solo se permiten digitos." << endl;
                continue;
            }
        }

        int anio = stoi(entrada.substr(0, 4));
        int mes = stoi(entrada.substr(5, 2));
        int dia = stoi(entrada.substr(8, 2));

        if (anio < 2000 || anio > 2100) {
            cout << "Error: Año invalido. Use entre 2000 y 2100." << endl;
            continue;
        }
        
        if (mes < 1 || mes > 12) {
            cout << "Error: Mes invalido." << endl;
            continue;
        }
        
        if (dia < 1 || dia > 31) {
            cout << "Error: Dia invalido." << endl;
            continue;
        }
        
        if (mes == 2) {
            bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
            if (bisiesto && dia > 29) {
                cout << "Error: Febrero solo tiene 29 dias en años bisiestos." << endl;
                continue;
            }
            if (!bisiesto && dia > 28) {
                cout << "Error: Febrero solo tiene 28 dias." << endl;
                continue;
            }
        } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
            cout << "Error: Este mes solo tiene 30 dias." << endl;
            continue;
        }

        time_t ahora = time(nullptr);
        struct tm* tiempoActual = localtime(&ahora);
        int anioActual = tiempoActual->tm_year + 1900;
        int mesActual = tiempoActual->tm_mon + 1;
        int diaActual = tiempoActual->tm_mday;
        
        if (anio > anioActual) {
            cout << "Advertencia: Fecha futura." << endl;
        } else if (anio == anioActual && mes > mesActual) {
            cout << "Advertencia: Fecha futura." << endl;
        } else if (anio == anioActual && mes == mesActual && dia > diaActual) {
            cout << "Advertencia: Fecha futura." << endl;
        }
        
        strcpy(fecha, entrada.c_str());
        return true;
    }
}

bool Formatos::leerHora(char* hora, const char* mensaje) {
    while (true) {
        cout << mensaje << " (HH:MM): ";
        string entrada;
        getline(cin, entrada);
        
        if (entrada.length() != 5 || entrada[2] != ':') {
            cout << "Error: Formato invalido. Use HH:MM." << endl;
            continue;
        }
        
        for (int i = 0; i < 5; i++) {
            if (i == 2) continue;
            if (!isdigit(entrada[i])) {
                cout << "Error: Solo se permiten digitos." << endl;
                continue;
            }
        }

        int hh = stoi(entrada.substr(0, 2));
        int mm = stoi(entrada.substr(3, 2));
        
        if (hh < 0 || hh > 23) {
            cout << "Error: Hora invalida. Use 00-23." << endl;
            continue;
        }
        
        if (mm < 0 || mm > 59) {
            cout << "Error: Minutos invalidos. Use 00-59." << endl;
            continue;
        }

        if (hh < 8 || hh > 20) {
            cout << "Advertencia: Horario fuera del horario laboral normal (8:00-20:00)." << endl;
        }
        
        strcpy(hora, entrada.c_str());
        return true;
    }
}