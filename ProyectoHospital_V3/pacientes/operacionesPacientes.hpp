#ifndef OPERACIONESPACIENTES_HPP
#define OPERACIONESPACIENTES_HPP

#include "Paciente.hpp"
#include "../hospital/Hospital.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

using namespace std;

//Funciones de Gestion de Pacientes
void registrarPaciente(Hospital& hospital);
void buscarPacientePorID();
void buscarPacientePorCedula();
void buscarPacientesPorNombre();
void modificarPaciente();
void eliminarPaciente();
void listarTodosPacientes();
void verHistorialPaciente();

//Funciones Auxiliares
void mostrarMenuPacientes(Hospital& hospital);
bool pedirDatosPaciente(Paciente& paciente, Hospital& hospital, bool esModificacion = false);
void mostrarPacienteEncontrado(const Paciente& paciente);

#endif