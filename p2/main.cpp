#include <iostream>
#include <string>

using namespace std;

// Definicion adelantada de la clase Estacion
class Estacion;

// Definicion completa de la clase Linea
class Linea {
private:
    string nombre;
    Estacion* primeraEstacion;
    Estacion* ultimaEstacion;
    Linea* siguiente;
    int numEstaciones;
    bool tieneEstacionTransferencia;

public:
    Linea(string _nombre) : nombre(_nombre), primeraEstacion(nullptr), ultimaEstacion(nullptr), siguiente(nullptr), numEstaciones(0), tieneEstacionTransferencia(false) {}

    string getNombre() const {
        return nombre;
    }

    int numeroEstaciones() const {
        return numEstaciones;
    }

    void agregarEstacion(Estacion* estacion);

    bool eliminarEstacion(Estacion* estacion) {
        // Implementa la logica para eliminar una estacion de la linea
        return false;
    }

    bool tieneTransferencia() const {
        return tieneEstacionTransferencia;
    }

    void marcarTransferencia() {
        tieneEstacionTransferencia = true;
    }

    int tiempoDeViaje(Estacion* origen, Estacion* destino) const {
        // Implementa la logica para calcular el tiempo de viaje entre dos estaciones en la linea
        return 0;
    }

    Linea* getSiguiente() const {
        return siguiente;
    }

    void setSiguiente(Linea* _siguiente) {
        siguiente = _siguiente;
    }

    Estacion* obtenerPrimeraEstacion() const {
        return primeraEstacion;
    }
};

// Definicion completa de la clase Estacion
class Estacion {
private:
    string nombre;
    Linea** lineas;
    int capacidadLineas;
    int numLineas;
    Estacion* anterior;
    Estacion* siguiente;

    // Metodos privados para el manejo de memoria
    void expandirCapacidad() {
        int nuevaCapacidad = capacidadLineas * 2;
        Linea** temp = new Linea*[nuevaCapacidad];
        for (int i = 0; i < numLineas; ++i) {
            temp[i] = lineas[i];
        }
        delete[] lineas;
        lineas = temp;
        capacidadLineas = nuevaCapacidad;
    }

public:
    Estacion(string _nombre) : nombre(_nombre), lineas(nullptr), capacidadLineas(5), numLineas(0), anterior(nullptr), siguiente(nullptr) {
        lineas = new Linea*[capacidadLineas];
    }

    ~Estacion() {
        delete[] lineas;
    }

    string getNombre() const {
        return nombre;
    }

    void setAnterior(Estacion* estacion) {
        anterior = estacion;
    }

    Estacion* getAnterior() const {
        return anterior;
    }

    void setSiguiente(Estacion* estacion) {
        siguiente = estacion;
    }

    Estacion* getSiguiente() const {
        return siguiente;
    }

    void agregarLinea(Linea* linea) {
        if (numLineas == capacidadLineas) {
            expandirCapacidad();
        }
        lineas[numLineas++] = linea;
    }

    bool tieneLinea(Linea* linea) const {
        for (int i = 0; i < numLineas; ++i) {
            if (lineas[i] == linea) {
                return true;
            }
        }
        return false;
    }

    bool tieneTransferencia() const {
        for (int i = 0; i < numLineas; ++i) {
            if (lineas[i]->tieneTransferencia()) {
                return true;
            }
        }
        return false;
    }

    bool eliminarEstacion(Estacion* estacion) {
        // Implementa la logica para eliminar una estacion
        return false;
    }

    int tiempoDeViaje(Estacion* origen, Estacion* destino) const {
        // Implementa la logica para calcular el tiempo de viaje entre dos estaciones
        return 0;
    }
};

// Definicion del metodo agregarEstacion de la clase Linea
void Linea::agregarEstacion(Estacion* estacion) {
    if (!primeraEstacion) {
        primeraEstacion = estacion;
        ultimaEstacion = estacion;
    } else {
        ultimaEstacion->setSiguiente(estacion);
        ultimaEstacion = estacion;
    }
    numEstaciones++;

    // Si la estacion agregada es de transferencia, marcar la linea
    if (estacion->tieneTransferencia()) {
        marcarTransferencia();
    }
}

class Red {
private:
    Linea* primeraLinea;
    Linea* ultimaLinea;
    int numLineas;

public:
    Red() : primeraLinea(nullptr), ultimaLinea(nullptr), numLineas(0) {}

    void agregarLinea(Linea* linea) {
        if (!primeraLinea) {
            primeraLinea = linea;
            ultimaLinea = linea;
        } else {
            ultimaLinea->setSiguiente(linea);
            ultimaLinea = linea;
        }
        numLineas++;
    }

    void mostrarEstructura() const {
        Linea* tempLinea = primeraLinea;
        while (tempLinea) {
            cout << "Linea: " << tempLinea->getNombre();
            if (tempLinea->tieneTransferencia()) {
                cout << " (con estacion de transferencia)";
            }
            cout << endl;
            Estacion* tempEstacion = tempLinea->obtenerPrimeraEstacion(); // Usar el nuevo metodo
            while (tempEstacion) {
                cout << "- Estacion: " << tempEstacion->getNombre() << endl;
                tempEstacion = tempEstacion->getSiguiente();
            }
            tempLinea = tempLinea->getSiguiente();
        }
    }

    Linea* getPrimeraLinea() const {
        return primeraLinea;
    }

    void eliminarLinea(Linea* linea) {
        // Verificar si la linea tiene estaciones de transferencia
        if (linea->tieneTransferencia()) {
            cout << "No se puede eliminar la linea '" << linea->getNombre() << "' porque tiene una estacion de transferencia." << endl;
            return;
        }

        // Implementa la logica para eliminar una linea de la red
        Linea* tempLinea = primeraLinea;
        Linea* prevLinea = nullptr;
        while (tempLinea && tempLinea != linea) {
            prevLinea = tempLinea;
            tempLinea = tempLinea->getSiguiente();
        }

        if (!prevLinea) {
            primeraLinea = tempLinea->getSiguiente();
        } else {
            prevLinea->setSiguiente(tempLinea->getSiguiente());
        }

        delete tempLinea;
        numLineas--;

        cout << "Linea eliminada exitosamente." << endl;
    }

    int contarLineas() const {
        return numLineas;
    }

    void mostrarMenu() {
        cout << "Menu de opciones:" << endl;
        cout << "1. Crear estacion" << endl;
        cout << "2. Eliminar estacion" << endl;
        cout << "3. Crear linea" << endl;
        cout << "4. Eliminar linea" << endl;
        cout << "5. Calcular tiempo de desplazamiento entre estaciones" << endl;
        cout << "6. Salir" << endl;
        cout << "7. Mostrar lineas" << endl;
        cout << "Seleccione una opcion: ";
    }

    // Metodo para crear una linea en la red
    void crearLinea() {
        string nombreLinea;
        cout << "Ingrese el nombre de la linea: ";
        cin >> nombreLinea;

        Linea* nuevaLinea = new Linea(nombreLinea);
        agregarLinea(nuevaLinea);

        cout << "Linea creada exitosamente." << endl;
    }
};

int calcularTiempoDeLlegada(Estacion* origen, Estacion* destino) {
    // Implementa la logica para calcular el tiempo de llegada
    return 0;
}

int main() {
    Red red;
    int opcion;
    cout << "Bienvenido al sistema de gestion de la red de metro." << endl;
    do {
        red.mostrarMenu();
        cin >> opcion;
        cout << endl;

        switch (opcion) {
        case 1:
            // Implementa la logica para crear una estacion
            break;
        case 2:
            // Implementa la logica para eliminar una estacion
            break;
        case 3:
            red.crearLinea(); // Crear linea en la red
            break;
        case 4: {
            string nombreLinea;
            cout << "Ingrese el nombre de la linea que desea eliminar: ";
            cin >> nombreLinea;

            Linea* lineaEliminar = nullptr;
            Linea* tempLinea = red.getPrimeraLinea();
            while (tempLinea) {
                if (tempLinea->getNombre() == nombreLinea) {
                    lineaEliminar = tempLinea;
                    break;
                }
                tempLinea = tempLinea->getSiguiente();
            }

            if (lineaEliminar) {
                red.eliminarLinea(lineaEliminar);
            } else {
                cout << "La linea especificada no existe en la red." << endl;
            }
            break;
        }
        case 5:
            // Implementa la logica para calcular el tiempo de desplazamiento entre estaciones
            break;
        case 6:
            cout << "¡Hasta luego!" << endl;
            break;
        case 7:
            red.mostrarEstructura();
            cout << endl;
            break;
        default:
            cout << "Opcion no valida. Por favor, seleccione una opcion valida." << endl;
            break;
        }
    } while (opcion != 6);

    return 0;
}
