#include "hospital/Hospital.hpp"
#include "pacientes/operacionesPacientes.hpp"
#include "doctores/operacionesDoctores.hpp"
#include "citas/operacionesCitas.hpp"
#include "historial/operacionesHistorial.hpp"
#include "persistencia/GestorArchivos.hpp"
#include "utilidades/Formatos.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    cout << "\n";
    Formatos::mostrarSeparador('=', 50);
    cout << "   SISTEMA DE GESTION HOSPITALARIA V3" << endl;
    Formatos::mostrarSeparador('=', 50);
    
    cout << "\nInicializando sistema de archivos..." << endl;
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cerr << "\nError critico: No se pudo inicializar el sistema de archivos." << endl;
        cerr << "   Verifique los permisos de escritura en el directorio." << endl;
        return 1;
    }

    Hospital hospital;

    if (!GestorArchivos::cargarHospital(hospital)) {
        cout << "No se pudo cargar el hospital. Creando uno nuevo..." << endl;
        hospital = Hospital("Hospital General", "Calle Principal #123", "555-1234");
        if (!GestorArchivos::guardarHospital(hospital)) {
            cerr << "Error critico al guardar hospital." << endl;
            return 1;
        }
    }
    
    cout << "\nSistema inicializado correctamente." << endl;
    cout << "  Hospital: " << hospital.getNombre() << endl;
    
    int opcion;
    do {
        cout << "\n";
        Formatos::mostrarSeparador('=', 50);
        cout << "          MENU PRINCIPAL" << endl;
        Formatos::mostrarSeparador('=', 50);
        cout << "1. Gestion de Pacientes" << endl;
        cout << "2. Gestion de Doctores" << endl;
        cout << "3. Gestion de Citas" << endl;
        cout << "4. Historial Medico" << endl;
        cout << "5. Mantenimiento del Sistema" << endl;
        cout << "6. Guardar y Salir" << endl;
        Formatos::mostrarSeparador('-', 30);
        cout << "Opcion: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch(opcion) {
            case 1:
                mostrarMenuPacientes(hospital);
                break;
            case 2:
                mostrarMenuDoctores(hospital);
                break;
            case 3:
                mostrarMenuCitas(hospital);
                break;
            case 4:
                mostrarMenuHistorial(hospital);
                break;
            case 5:
                menuMantenimiento(hospital);
                break;
            case 6:
                cout << "\nGuardando datos y saliendo..." << endl;
                GestorArchivos::guardarHospital(hospital);
                break;
            default:
                cout << "Opcion invalida." << endl;
        }
    } while(opcion != 6);
    
    cout << "\nSistema cerrado correctamente." << endl;
    return 0;
}

void menuMantenimiento(Hospital& hospital) {
    int opcion;
    
    do {
        Formatos::mostrarEncabezado("MANTENIMIENTO DEL SISTEMA");
        
        cout << "1. Verificar integridad de archivos" << endl;
        cout << "2. Compactar archivos de pacientes" << endl;
        cout << "3. Compactar archivos de doctores" << endl;
        cout << "4. Compactar archivos de citas" << endl;
        cout << "5. Mostrar estadisticas" << endl;
        cout << "6. Volver al menu principal" << endl;
        cout << "\nOpción: ";
        
        cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: {
                cout << "\nVerificando integridad de archivos..." << endl;
                if (GestorArchivos::verificarIntegridadArchivos()) {
                    cout << "\nTodos los archivos están correctos." << endl;
                } else {
                    cout << "\nSe encontraron problemas en algunos archivos." << endl;
                }
                break;
            }
            case 2: {
                cout << "\nCompactando archivo de pacientes..." << endl;
                if (GestorArchivos::compactarArchivo<Paciente>(Rutas::PACIENTES)) {
                    cout << "Archivo de pacientes compactado correctamente." << endl;
                } else {
                    cout << "Error al compactar archivo de pacientes." << endl;
                }
                break;
            }
            case 3: {
                cout << "\nCompactando archivo de doctores..." << endl;
                if (GestorArchivos::compactarArchivo<Doctor>(Rutas::DOCTORES)) {
                    cout << "Archivo de doctores compactado correctamente." << endl;
                } else {
                    cout << "Error al compactar archivo de doctores." << endl;
                }
                break;
            }
            case 4: {
                cout << "\nCompactando archivo de citas..." << endl;
                if (GestorArchivos::compactarArchivo<Cita>(Rutas::CITAS)) {
                    cout << "Archivo de citas compactado correctamente." << endl;
                } else {
                    cout << "Error al compactar archivo de citas." << endl;
                }
                break;
            }
            case 5: {
                cout << "\n=== ESTADÍSTICAS DEL SISTEMA ===" << endl;
                hospital.mostrarEstadisticas();
                
                int pacientesActivos = GestorArchivos::contarRegistrosActivos(Rutas::PACIENTES, Paciente::obtenerTamano());
                int doctoresActivos = GestorArchivos::contarRegistrosActivos(Rutas::DOCTORES, Doctor::obtenerTamano());
                int citasActivas = GestorArchivos::contarRegistrosActivos(Rutas::CITAS, Cita::obtenerTamano());
                int consultasActivas = GestorArchivos::contarRegistrosActivos(Rutas::HISTORIALES, HistorialMedico::obtenerTamano());
                
                cout << "\n=== REGISTROS ACTIVOS EN ARCHIVOS ===" << endl;
                cout << "Pacientes: " << pacientesActivos << endl;
                cout << "Doctores: " << doctoresActivos << endl;
                cout << "Citas: " << citasActivas << endl;
                cout << "Consultas: " << consultasActivas << endl;
                break;
            }
            case 6:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opción invalida." << endl;
        }
        
        if (opcion != 6) {
            Formatos::pausar();
        }
        
    } while (opcion != 6);
}