#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

#include <cstring>

namespace Rutas {
    const char* const HOSPITAL = "datos/hospital.bin";
    const char* const PACIENTES = "datos/pacientes.bin";
    const char* const DOCTORES = "datos/doctores.bin";
    const char* const CITAS = "datos/citas.bin";
    const char* const HISTORIALES = "datos/historiales.bin";
}

namespace ConstantesArchivos {
    const int VERSION_ACTUAL = 1;
    const int MAX_REGISTROS = 10000;
    const int TAM_CABECERA = sizeof(int) * 4;
}

#endif