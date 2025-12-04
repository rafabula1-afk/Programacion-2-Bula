#ifndef OPERACIONESHISTORIAL_HPP
#define OPERACIONESHISTORIAL_HPP

#include "HistorialMedico.hpp"
#include "../hospital/Hospital.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

using namespace std;

//Funciones de Gestion de Hospital
void verHistorialPaciente();
void agregarConsultaManual();
void buscarConsultaPorID();
void listarTodasConsultas();

//Funciones Auxiliares
void mostrarMenuHistorial(Hospital& hospital);
bool pedirDatosConsulta(HistorialMedico& consulta, Hospital& hospital);

#endif