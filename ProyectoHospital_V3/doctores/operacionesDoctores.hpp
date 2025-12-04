#ifndef OPERACIONESDOCTORES_HPP
#define OPERACIONESDOCTORES_HPP

#include "Doctor.hpp"
#include "../hospital/Hospital.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

using namespace std;

// ============ FUNCIONES DE GESTIÓN DE DOCTORES ============
void registrarDoctor(Hospital& hospital);
void buscarDoctorPorID();
void listarTodosDoctores();
void listarDoctoresPorEspecialidad();
void modificarDoctor();
void eliminarDoctor();
void asignarPacienteADoctor();
void listarPacientesDeDoctor();

// ============ FUNCIONES AUXILIARES ============
void mostrarMenuDoctores(Hospital& hospital);
bool pedirDatosDoctor(Doctor& doctor, Hospital& hospital, bool esModificacion = false);
void mostrarDoctorEncontrado(const Doctor& doctor);

#endif // OPERACIONESDOCTORES_HPP