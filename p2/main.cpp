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

    void agregarEstacion(Estacion* estacion, int posicion = -1);

    bool eliminarEstacion(Estacion* estacion);

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
void Linea::agregarEstacion(Estacion* estacion, int posicion) {
    if (posicion == -1 || posicion >= numEstaciones) { // Añadir al final
        if (!primeraEstacion) {
            primeraEstacion = estacion;
            ultimaEstacion = estacion;
        } else {
            ultimaEstacion->setSiguiente(estacion);
            estacion->setAnterior(ultimaEstacion);
            ultimaEstacion = estacion;
        }
    } else if (posicion == 0) { // Añadir al principio
        estacion->setSiguiente(primeraEstacion);
        primeraEstacion->setAnterior(estacion);
        primeraEstacion = estacion;
    } else { // Añadir en una posición intermedia
        Estacion* actual = primeraEstacion;
        for (int i = 0; i < posicion - 1 && actual->getSiguiente(); ++i) {
            actual = actual->getSiguiente();
        }
        estacion->setSiguiente(actual->getSiguiente());
        estacion->setAnterior(actual);
        if (actual->getSiguiente()) {
            actual->getSiguiente()->setAnterior(estacion);
        }
        actual->setSiguiente(estacion);
    }

    numEstaciones++;

    // Si la estacion agregada es de transferencia, marcar la linea
    if (estacion->tieneTransferencia()) {
        marcarTransferencia();
    }
}

// Implementacion del metodo eliminarEstacion de la clase Linea
bool Linea::eliminarEstacion(Estacion* estacion) {
    if (primeraEstacion == nullptr) {
        return false;
    }

    Estacion* actual = primeraEstacion;
    while (actual) {
        if (actual == estacion) {
            if (actual == primeraEstacion && actual == ultimaEstacion) {
                primeraEstacion = ultimaEstacion = nullptr;
            } else if (actual == primeraEstacion) {
                primeraEstacion = actual->getSiguiente();
                primeraEstacion->setAnterior(nullptr);
            } else if (actual == ultimaEstacion) {
                ultimaEstacion = actual->getAnterior();
                ultimaEstacion->setSiguiente(nullptr);
            } else {
                actual->getAnterior()->setSiguiente(actual->getSiguiente());
                actual->getSiguiente()->setAnterior(actual->getAnterior());
            }
            numEstaciones--;
            delete actual;
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
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
            Estacion* tempEstacion = tempLinea->obtenerPrimeraEstacion();
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
        cout << endl << "Menu de opciones:" << endl;
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

        cout << endl<< "Linea creada exitosamente." << endl << endl;
    }

    void crearEstacion() {
        string nombreEstacion, nombreLinea;
        char esTransferencia;

        cout << "Ingrese el nombre de la estacion: ";
        cin >> nombreEstacion;

        cout << "Ingrese el nombre de la linea a la que desea agregar la estacion: ";
        cin >> nombreLinea;

        cout << "Es una estacion de transferencia? (s/n): ";
        cin >> esTransferencia;

        Linea* linea = primeraLinea;
        while (linea) {
            if (linea->getNombre() == nombreLinea) {
                Estacion* nuevaEstacion = new Estacion(nombreEstacion);
                if (esTransferencia == 's' || esTransferencia == 'S') {
                    nuevaEstacion->agregarLinea(linea);
                    nuevaEstacion->tieneTransferencia();
                }

                // Mostrar las estaciones existentes y pedir la posición
                Estacion* tempEstacion = linea->obtenerPrimeraEstacion();
                cout << "Estaciones existentes en la linea:" << endl;
                int posicion = 0;
                while (tempEstacion) {
                    cout << posicion << ". " << tempEstacion->getNombre() << endl;
                    tempEstacion = tempEstacion->getSiguiente();
                    posicion++;
                }
                cout << posicion << ". (Agregar al final)" << endl;

                int opcionPosicion;
                cout << "Ingrese el numero de posicion donde desea agregar la nueva estacion: ";
                cin >> opcionPosicion;

                linea->agregarEstacion(nuevaEstacion, opcionPosicion);
                cout << "Estacion creada exitosamente." << endl;
                return;
            }
            linea = linea->getSiguiente();
        }

        cout << "La linea especificada no existe en la red." << endl;
    }

    void eliminarEstacion() {
        string nombreEstacion, nombreLinea;

        cout << "Ingrese el nombre de la estacion que desea eliminar: ";
        cin >> nombreEstacion;

        cout << "Ingrese el nombre de la linea a la que pertenece la estacion: ";
        cin >> nombreLinea;

        Linea* linea = primeraLinea;
        while (linea) {
            if (linea->getNombre() == nombreLinea) {
                Estacion* estacion = linea->obtenerPrimeraEstacion();
                while (estacion) {
                    if (estacion->getNombre() == nombreEstacion) {
                        if (linea->eliminarEstacion(estacion)) {
                            cout << "Estacion eliminada exitosamente." << endl;
                        } else {
                            cout << "No se pudo eliminar la estacion." << endl;
                        }
                        return;
                    }
                    estacion = estacion->getSiguiente();
                }
                cout << "La estacion especificada no existe en la linea." << endl;
                return;
            }
            linea = linea->getSiguiente();
        }

        cout << "La linea especificada no existe en la red." << endl;
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
        cout<<endl;

        switch (opcion) {
        case 1:
            red.crearEstacion();
            break;
        case 2:
            red.eliminarEstacion();
            break;
        case 3:
            red.crearLinea();
            break;
        case 4:{
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
            // Implementar la opcion de calcular tiempo de desplazamiento
            break;
        case 6:
            cout << "¡Hasta luego!." << endl;
            break;
        case 7:
            red.mostrarEstructura();
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
            break;
        }
    } while (opcion != 6);

    return 0;
}
