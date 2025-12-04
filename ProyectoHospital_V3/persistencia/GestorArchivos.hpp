#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include "../hospital/Hospital.hpp"
#include "../pacientes/Paciente.hpp"
#include "../doctores/Doctor.hpp"
#include "../citas/Cita.hpp"
#include "../historial/HistorialMedico.hpp"
#include "Constantes.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

//Estructura de header
struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
    
    ArchivoHeader() : cantidadRegistros(0), proximoID(1), registrosActivos(0), version(ConstantesArchivos::VERSION_ACTUAL) {}
};

class GestorArchivos {
private:

//Metodos Privados
    static long calcularPosicion(int indice, size_t tamanioRegistro) {
        return sizeof(ArchivoHeader) + (indice * tamanioRegistro);
    }
    
    static bool crearDirectorioSiNoExiste(const char* ruta) {
        struct stat info;
        if (stat(ruta, &info) != 0) {
            #ifdef _WIN32
                return mkdir(ruta) == 0;
            #else
                return mkdir(ruta, 0777) == 0;
            #endif
        } else if (info.st_mode & S_IFDIR) {
            return true;
        }
        return false;
    }
    
public:

    //Metodos de Inicializacion y Verificacion
    static bool inicializarSistemaArchivos() {
        if (!crearDirectorioSiNoExiste("datos")) {
            cerr << "Error: No se pudo crear el directorio 'datos'" << endl;
            return false;
        }
        
        const char* archivos[] = {
            Rutas::HOSPITAL,
            Rutas::PACIENTES,
            Rutas::DOCTORES,
            Rutas::CITAS,
            Rutas::HISTORIALES
        };
        
        for (int i = 0; i < 5; i++) {
            if (!inicializarArchivo(archivos[i])) {
                cerr << "Error: No se pudo inicializar " << archivos[i] << endl;
                return false;
            }
        }
        
        cout << "Sistema de archivos inicializado correctamente." << endl;
        return true;
    }
    
    static bool verificarIntegridadArchivos() {
        const char* archivos[] = {
            Rutas::HOSPITAL,
            Rutas::PACIENTES,
            Rutas::DOCTORES,
            Rutas::CITAS,
            Rutas::HISTORIALES
        };
        
        bool todosOK = true;
        for (int i = 0; i < 5; i++) {
            ifstream archivo(archivos[i], ios::binary);
            if (!archivo.is_open()) {
                cerr << archivos[i] << " - NO EXISTE O NO SE PUEDE ACCEDER" << endl;
                todosOK = false;
                continue;
            }
            
            ArchivoHeader header;
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
            archivo.close();
            
            if (header.version != ConstantesArchivos::VERSION_ACTUAL) {
                cerr << archivos[i] << " - VERSION INCORRECTA" << endl;
                todosOK = false;
            } else {
                cout << archivos[i] << " - OK (" << header.registrosActivos << " registros)" << endl;
            }
        }
        
        return todosOK;
    }
    
    //Metodos para Header
    static ArchivoHeader leerHeader(const char* nombreArchivo) {
        ArchivoHeader header;
        ifstream archivo(nombreArchivo, ios::binary);
        
        if (archivo.is_open() && archivo.good()) {
            archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        } else {
            header = ArchivoHeader();
        }
        
        if (archivo.is_open()) archivo.close();
        return header;
    }
    
    static bool escribirHeader(const char* nombreArchivo, ArchivoHeader header) {
        fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            return false;
        }
        
        archivo.seekp(0);
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        
        return !archivo.fail();
    }
    
    //Metodos Genericos
    template<typename T>
    static bool guardarRegistro(const T& registro, const char* archivo) {
        ofstream archivoBinario(archivo, ios::binary | ios::app);
        if (!archivoBinario.is_open()) {
            cerr << "Error: No se pudo abrir " << archivo << " para escritura" << endl;
            return false;
        }

        const char* bytes = reinterpret_cast<const char*>(&registro);
        archivoBinario.write(bytes, sizeof(T));
        
        bool exito = !archivoBinario.fail();
        archivoBinario.close();
        
        if (exito) {
            ArchivoHeader header = leerHeader(archivo);
            header.cantidadRegistros++;
            header.registrosActivos++;
            if (registro.getId() >= header.proximoID) {
                header.proximoID = registro.getId() + 1;
            }
            escribirHeader(archivo, header);
        }
        
        return exito;
    }
    
    template<typename T>
    static bool leerRegistroPorIndice(int indice, T& registro, const char* archivo) {
        ifstream archivoBinario(archivo, ios::binary);
        if (!archivoBinario.is_open()) {
            return false;
        }
        
        ArchivoHeader header = leerHeader(archivo);
        if (indice < 0 || indice >= header.cantidadRegistros) {
            archivoBinario.close();
            return false;
        }
        
        long posicion = calcularPosicion(indice, sizeof(T));
        archivoBinario.seekg(posicion);
        
        char* bytes = reinterpret_cast<char*>(&registro);
        archivoBinario.read(bytes, sizeof(T));
        
        bool exito = !archivoBinario.fail();
        archivoBinario.close();
        return exito;
    }
    
    template<typename T>
    static bool buscarRegistroPorID(int id, T& registro, const char* archivo) {
        ifstream archivoBinario(archivo, ios::binary);
        if (!archivoBinario.is_open()) {
            return false;
        }
        
        ArchivoHeader header = leerHeader(archivo);
        archivoBinario.seekg(sizeof(ArchivoHeader));
        
        bool encontrado = false;
        T temp;
        
        for (int i = 0; i < header.cantidadRegistros; i++) {
            char* bytes = reinterpret_cast<char*>(&temp);
            archivoBinario.read(bytes, sizeof(T));
            
            if (archivoBinario.fail()) break;
            
            if (temp.getId() == id && !temp.isEliminado()) {
                registro = temp;
                encontrado = true;
                break;
            }
        }
        
        archivoBinario.close();
        return encontrado;
    }
    
    template<typename T>
    static bool actualizarRegistroPorIndice(int indice, const T& registro, const char* archivo) {
        fstream archivoBinario(archivo, ios::binary | ios::in | ios::out);
        if (!archivoBinario.is_open()) {
            return false;
        }
        
        ArchivoHeader header = leerHeader(archivo);
        if (indice < 0 || indice >= header.cantidadRegistros) {
            archivoBinario.close();
            return false;
        }
        
        long posicion = calcularPosicion(indice, sizeof(T));
        archivoBinario.seekp(posicion);
        
        const char* bytes = reinterpret_cast<const char*>(&registro);
        archivoBinario.write(bytes, sizeof(T));
        
        bool exito = !archivoBinario.fail();
        archivoBinario.close();
        return exito;
    }
    
    template<typename T>
    static T* listarRegistrosActivos(int& cantidad, const char* archivo) {
        ifstream archivoBinario(archivo, ios::binary);
        if (!archivoBinario.is_open()) {
            cantidad = 0;
            return nullptr;
        }
        
        ArchivoHeader header = leerHeader(archivo);
        archivoBinario.seekg(sizeof(ArchivoHeader));

        cantidad = 0;
        T temp;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            char* bytes = reinterpret_cast<char*>(&temp);
            archivoBinario.read(bytes, sizeof(T));
            
            if (archivoBinario.fail()) break;
            
            if (!temp.isEliminado()) {
                cantidad++;
            }
        }
        
        if (cantidad == 0) {
            archivoBinario.close();
            return nullptr;
        }

        T* resultados = new T[cantidad];

        archivoBinario.clear();
        archivoBinario.seekg(sizeof(ArchivoHeader));
        
        int index = 0;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            char* bytes = reinterpret_cast<char*>(&temp);
            archivoBinario.read(bytes, sizeof(T));
            
            if (archivoBinario.fail()) break;
            
            if (!temp.isEliminado()) {
                resultados[index] = temp;
                index++;
            }
        }
        
        archivoBinario.close();
        return resultados;
    }
    
    template<typename T>
    static bool marcarRegistroComoEliminado(int id, const char* archivo) {
        T registro;
        if (!buscarRegistroPorID(id, registro, archivo)) {
            return false;
        }

        registro.setEliminado(true);

        ifstream archivoBinario(archivo, ios::binary);
        if (!archivoBinario.is_open()) {
            return false;
        }
        
        ArchivoHeader header = leerHeader(archivo);
        archivoBinario.seekg(sizeof(ArchivoHeader));
        
        int indice = -1;
        T temp;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            char* bytes = reinterpret_cast<char*>(&temp);
            archivoBinario.read(bytes, sizeof(T));
            
            if (archivoBinario.fail()) break;
            
            if (temp.getId() == id) {
                indice = i;
                break;
            }
        }
        archivoBinario.close();
        
        if (indice == -1) return false;

        fstream archivoActualizar(archivo, ios::binary | ios::in | ios::out);
        if (!archivoActualizar.is_open()) {
            return false;
        }
        
        long posicion = calcularPosicion(indice, sizeof(T));
        archivoActualizar.seekp(posicion);
        
        const char* bytes = reinterpret_cast<const char*>(&registro);
        archivoActualizar.write(bytes, sizeof(T));
        
        bool exito = !archivoActualizar.fail();
        archivoActualizar.close();
        
        if (exito) {
            header.registrosActivos--;
            escribirHeader(archivo, header);
        }
        
        return exito;
    }
    
    template<typename T>
    static bool compactarArchivo(const char* archivo) {
        ifstream archivoEntrada(archivo, ios::binary);
        if (!archivoEntrada.is_open()) {
            return false;
        }
        
        ArchivoHeader header = leerHeader(archivo);

        string archivoTemp = string(archivo) + ".tmp";
        ofstream archivoSalida(archivoTemp, ios::binary);
        if (!archivoSalida.is_open()) {
            archivoEntrada.close();
            return false;
        }

        ArchivoHeader nuevoHeader;
        nuevoHeader.version = header.version;
        nuevoHeader.cantidadRegistros = 0;
        nuevoHeader.registrosActivos = 0;
        nuevoHeader.proximoID = header.proximoID;
        
        archivoSalida.write(reinterpret_cast<const char*>(&nuevoHeader), sizeof(ArchivoHeader));

        archivoEntrada.seekg(sizeof(ArchivoHeader));
        T temp;
        int nuevoID = 1;
        
        for (int i = 0; i < header.cantidadRegistros; i++) {
            char* bytes = reinterpret_cast<char*>(&temp);
            archivoEntrada.read(bytes, sizeof(T));
            
            if (archivoEntrada.fail()) break;
            
            if (!temp.isEliminado()) {
                temp.setId(nuevoID++);

                const char* bytesEscritura = reinterpret_cast<const char*>(&temp);
                archivoSalida.write(bytesEscritura, sizeof(T));
                
                nuevoHeader.cantidadRegistros++;
                nuevoHeader.registrosActivos++;
            }
        }
        
        archivoEntrada.close();
        archivoSalida.close();

        nuevoHeader.proximoID = nuevoID;

        fstream archivoTempRW(archivoTemp, ios::binary | ios::in | ios::out);
        if (!archivoTempRW.is_open()) {
            remove(archivoTemp.c_str());
            return false;
        }
        
        archivoTempRW.seekp(0);
        archivoTempRW.write(reinterpret_cast<const char*>(&nuevoHeader), sizeof(ArchivoHeader));
        archivoTempRW.close();

        remove(archivo);
        rename(archivoTemp.c_str(), archivo);
        
        return true;
    }
    
    //Metodos Auxiliares
    static bool inicializarArchivo(const char* nombreArchivo) {
        ifstream archivoExistente(nombreArchivo, ios::binary);
        if (archivoExistente.is_open()) {
            archivoExistente.close();
            return true;
        }

        ofstream archivoNuevo(nombreArchivo, ios::binary);
        if (!archivoNuevo.is_open()) {
            return false;
        }
        
        ArchivoHeader header;
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = ConstantesArchivos::VERSION_ACTUAL;
        
        archivoNuevo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        archivoNuevo.close();
        
        return true;
    }
    
    static int contarRegistrosActivos(const char* nombreArchivo, size_t tamanoRegistro) {
        ArchivoHeader header = leerHeader(nombreArchivo);
        return header.registrosActivos;
    }
};

#endif