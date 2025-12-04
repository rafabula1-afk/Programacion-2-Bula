#ifndef OPERACIONESCITAS_HPP
#define OPERACIONESCITAS_HPP

#include "Cita.hpp"
#include "../hospital/Hospital.hpp"
#include "../pacientes/Paciente.hpp"
#include "../doctores/Doctor.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

using namespace std;

// ============ FUNCIONES DE GESTIÓN DE CITAS ============
void agendarCita(Hospital& hospital);
void cancelarCita();
void atenderCita();
void buscarCitasPorPaciente();
void buscarCitasPorDoctor();
void buscarCitasPorFecha();
void listarCitasPendientes();
void verificarDisponibilidad();

// ============ FUNCIONES AUXILIARES ============
void mostrarMenuCitas(Hospital& hospital);
bool pedirDatosCita(Cita& cita, Hospital& hospital, bool esModificacion = false);
void mostrarCitaEncontrada(const Cita& cita);

#endif // OPERACIONESCITAS_HPP