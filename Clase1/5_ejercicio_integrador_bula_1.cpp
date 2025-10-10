/*
 * EJERCICIO CLASE 1 - PUNTEROS Y MEMORIA DINÁMICA
 * ================================================
 * 
 * ENUNCIADO SIMPLIFICADO:
 * Implementar un programa que permita:
 * 1. Crear un arreglo dinámico de números enteros
 * 2. Llenar el arreglo con valores ingresados por el usuario
 * 3. Mostrar todos los números del arreglo
 * 4. Encontrar el número mayor en el arreglo
 * 5. Calcular el promedio de todos los números
 * 6. Liberar la memoria correctamente
 * 
 * REQUISITOS OBLIGATORIOS:
 * - Usar punteros y memoria dinámica (new/delete)
 * - Usar paso por referencia donde sea apropiado
 * - Validar entrada del usuario (tamaño del arreglo)
 * - Liberar toda la memoria asignada
 * - Asignar nullptr después de delete
 * 
 * FUNCIONES QUE DEBE IMPLEMENTAR:
 * 1. int* crearArreglo(int tamanio)
 * 2. void llenarArreglo(int* arreglo, int tamanio)
 * 3. void mostrarArreglo(int* arreglo, int tamanio)
 * 4. int encontrarMayor(int* arreglo, int tamanio)
 * 5. float calcularPromedio(int* arreglo, int tamanio)
 * 6. void liberarArreglo(int*& arreglo)
 * 
 * MENÚ SIMPLE:
 * 1. Crear y llenar arreglo
 * 2. Mostrar arreglo
 * 3. Encontrar número mayor
 * 4. Calcular promedio
 * 5. Salir
 * 
 * NOTAS IMPORTANTES:
 * - Validar que el tamaño del arreglo sea positivo
 * - Verificar que new no retorne nullptr
 * - Usar buenas prácticas de programación
 * - Comentar el código apropiadamente
 * 
 * CRITERIOS DE EVALUACIÓN:
 * - Correcta implementación de punteros (50%)
 * - Manejo correcto de memoria (30%)
 * - Funcionalidad del programa (20%)
 * 
 * TIEMPO ESTIMADO: 1-2 horas
 * DIFICULTAD: Básica-Medio
 */

#include <iostream>
#include <locale>
#include <iomanip>
using namespace std;

// Funcion para crear un arreglo dinamico
int* crearArreglo(int tamanio) {
    // TODO: Implementar esta funcion 
    // - Validar que tamanio sea positivo
    // - Crear arreglo dinomico con new
    // - Verificar que new no retorne nullptr
    // - Retornar puntero al arreglo
    
    
    int* arregloDinamico = new int[tamanio];

}

// Funcion para llenar el arreglo con valores del usuario
void llenarArreglo(int* arreglo, int tamanio) {
    // TODO: Implementar esta funcion
    // - Verificar que arreglo no sea nullptr
    // - Pedir valores al usuario
    // - Llenar el arreglo con los valores ingresados
    
    cout << "\n";
    cout << "Ingrese " << tamanio << " numeros:" << endl;
    for (int i = 0; i < tamanio; i++) {
        cout << "Elemento " << i << ": ";
        cin >> arreglo[i];
        }
}

// Funcion para mostrar todos los elementos del arreglo
void mostrarArreglo(int* arreglo, int tamanio) {
    // TODO: Implementar esta funcion
    // - Verificar que arreglo no sea nullptr
    // - Mostrar todos los elementos del arreglo
    // - Manejar caso de arreglo vacio
    
    cout << "\n";
    cout << "Arreglo dinamico: |";
    for (int i = 0; i < tamanio; i++) {
		cout << arreglo[i] << "|";
    }
    cout << endl;
}

// Funcion para encontrar el numero mayor
int encontrarMayor(int* arreglo, int tamanio, int mayor) {
    // TODO: Implementar esta funcion
    // - Verificar que arreglo no sea nullptr y tamanio > 0
    // - Buscar el numero mayor en el arreglo
    // - Retornar el numero mayor
    
    for (int i = 0; i < tamanio; i++) {
        
        mayor = arreglo[0];
        
        if(arreglo[i]>mayor){
        	
        	mayor = arreglo[i];
		}
        
    }
    
    return mayor; // Placeholder
}

// Funcion para calcular el promedio
float calcularPromedio(int* arreglo, int tamanio, float promedio) {
    // TODO: Implementar esta funcion
    // - Verificar que arreglo no sea nullptr y tamanio > 0
    // - Sumar todos los elementos
    // - Dividir por el tamanio
    // - Retornar el promedio
    
    
    for (int i = 0; i < tamanio; i++) {
        
        promedio = promedio + arreglo[i];
        
    }
    
    promedio = promedio / tamanio;
    
    return promedio; // Placeholder
}

// Funcion para liberar la memoria del arreglo
void liberarArreglo(int*& arreglo) {
    // TODO: Implementar esta funcion
    // - Verificar que arreglo no sea nullptr
    // - Liberar memoria con delete[]
    // - Asignar nullptr al puntero
    
    delete[] arreglo;
    arreglo = nullptr;
    
    cout << "Memoria liberada correctamente" << endl;
    cout << "\n";
}

// Funcion para mostrar el menú
void mostrarMenu() {
    cout << "\n=== GESTION DE ARREGLOS DINAMICOS ===" << endl;
    cout << "1. Crear y llenar arreglo" << endl;
    cout << "2. Mostrar arreglo" << endl;
    cout << "3. Encontrar numero mayor" << endl;
    cout << "4. Calcular promedio" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    setlocale(LC_ALL, "spanish");
    
    // Variables principales
    int* arreglo = nullptr;
    int tamanio = 0;
    
    cout << "=== GESTION DE ARREGLOS DINAMICOS ===" << endl;
    cout << "Implemente las funciones marcadas con TODO para completar el ejercicio." << endl;
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                // Crear y llenar arreglo
                cout << "\n";
				cout << "Ingrese el tamanio del arreglo: ";
                cin >> tamanio;
                
                arreglo = crearArreglo(tamanio);
                if (arreglo != nullptr) {
                    llenarArreglo(arreglo, tamanio);
                    cout << "\n";
                    cout << "Arreglo creado y llenado correctamente." << endl;
                } else {
                	cout << "\n";
                    cout << "Error al crear el arreglo." << endl;
                }
                break;
            }
            
            case 2: {
                // Mostrar arreglo
                if (arreglo != nullptr) {
                    mostrarArreglo(arreglo, tamanio);
                } else {
                	cout << "\n";
                    cout << "No hay arreglo creado. Use la opcion 1 primero." << endl;
                }
                break;
            }
            
            case 3: {
                // Encontrar numero mayor
                if (arreglo != nullptr && tamanio > 0) {
                    int mayor = encontrarMayor(arreglo, tamanio, mayor);
                    cout << "\n";
                    cout << "El numero mayor es: " << mayor << endl;
                } else {
                	cout << "\n";
                    cout << "No hay arreglo creado o esta vacio." << endl;
                }
                break;
            }
            
            case 4: {
                // Calcular promedio
                if (arreglo != nullptr && tamanio > 0) {
                    float promedio = calcularPromedio(arreglo, tamanio, promedio);
                    cout << "\n";
                    cout << "El promedio es: " << fixed << setprecision(2) << promedio << endl;
                } else {
                	cout << "\n";
                    cout << "No hay arreglo creado o esta vacio." << endl;
                }
                break;
            }
            
            case 5: {
            	cout << "\n";
                cout << "Saliendo del programa..." << endl;
                break;
            }
            
            default: {
                cout << "Opcion invalida. Intente nuevamente." << endl;
                break;
            }
        }
    } while (opcion != 5);
    
    // Liberar memoria antes de salir
    liberarArreglo(arreglo);
    
    cout << "\n";
    cout << "Programa finalizado. Memoria liberada." << endl;
    return 0;
}

/*
 * PISTAS PARA LA IMPLEMENTACIÓN:
 * 
 * 1. crearArreglo(int tamanio):
 *    - Verificar que tamanio > 0
 *    - Crear arreglo con: int* nuevoArreglo = new int[tamanio];
 *    - Verificar que new no retorne nullptr
 *    - Retornar puntero al arreglo
 * 
 * 2. llenarArreglo(int* arreglo, int tamanio):
 *    - Verificar que arreglo != nullptr
 *    - Usar un ciclo for para pedir valores
 *    - Usar cin para leer cada valor
 *    - Asignar a arreglo[i]
 * 
 * 3. mostrarArreglo(int* arreglo, int tamanio):
 *    - Verificar que arreglo != nullptr
 *    - Usar un ciclo for para mostrar valores
 *    - Mostrar cada elemento con cout
 * 
 * 4. encontrarMayor(int* arreglo, int tamanio):
 *    - Verificar que arreglo != nullptr y tamanio > 0
 *    - Inicializar variable mayor con arreglo[0]
 *    - Usar ciclo for para comparar con el resto
 *    - Actualizar mayor si encuentra uno más grande
 *    - Retornar mayor
 * 
 * 5. calcularPromedio(int* arreglo, int tamanio):
 *    - Verificar que arreglo != nullptr y tamanio > 0
 *    - Inicializar variable suma = 0
 *    - Usar ciclo for para sumar todos los elementos
 *    - Dividir suma entre tamanio
 *    - Retornar promedio como float
 * 
 * 6. liberarArreglo(int*& arreglo):
 *    - Verificar que arreglo != nullptr
 *    - Usar: delete[] arreglo;
 *    - Asignar: arreglo = nullptr;
 * 
 * CONCEPTOS CLAVE:
 * - new int[tamanio] crea un arreglo dinámico
 * - delete[] libera memoria de arreglos
 * - nullptr indica que un puntero no apunta a nada
 * - Paso por referencia (&) permite modificar el puntero
 * - Siempre verificar punteros antes de usar
 */
