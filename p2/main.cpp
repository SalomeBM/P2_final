#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <ctime>

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

    void agregarEstacion(Estacion* estacion, int posicion = -1, int tiempoAnterior = 0, int tiempoSiguiente = 0);

    bool eliminarEstacion(Estacion* estacion);

    bool tieneTransferencia() const {
        return tieneEstacionTransferencia;
    }

    void marcarTransferencia() {
        tieneEstacionTransferencia = true;
    }

    int tiempoDeViaje(Estacion* origen, Estacion* destino) const;

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
    bool esTransferencia;
    int tiempoSiguiente;  // Tiempo de viaje a la siguiente estación

    // Métodos privados para el manejo de memoria
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
    Estacion(string _nombre) : nombre(_nombre), lineas(nullptr), capacidadLineas(5), numLineas(0), anterior(nullptr), siguiente(nullptr), esTransferencia(false), tiempoSiguiente(0) {
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

    void setSiguiente(Estacion* estacion, int tiempo = 0) {
        siguiente = estacion;
        tiempoSiguiente = tiempo;
    }

    Estacion* getSiguiente() const {
        return siguiente;
    }

    void setTransferencia(bool transferencia) {
        esTransferencia = transferencia;
    }

    bool getTransferencia() const {
        return esTransferencia;
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
        return esTransferencia;
    }

    bool eliminarEstacion(Estacion* estacion) {
        // Implementa la logica para eliminar una estacion
        return false;
    }

    int tiempoDeViaje(Estacion* origen, Estacion* destino) const {
        return 0;
    }

    int getTiempoSiguiente() const {
        return tiempoSiguiente;
    }

    Linea** getLineas() const {
        return lineas;
    }

    int getNumeroLineas() const {
        return numLineas;
    }
};

// Definicion del metodo agregarEstacion de la clase Linea
void Linea::agregarEstacion(Estacion* estacion, int posicion, int tiempoAnterior, int tiempoSiguiente) {
    if (posicion == -1 || posicion >= numEstaciones) { // Añadir al final
        if (!primeraEstacion) {
            primeraEstacion = estacion;
            ultimaEstacion = estacion;
        } else {
            ultimaEstacion->setSiguiente(estacion, tiempoSiguiente);
            estacion->setAnterior(ultimaEstacion);
            ultimaEstacion = estacion;
        }
    } else if (posicion == 0) { // Añadir al principio
        estacion->setSiguiente(primeraEstacion, tiempoSiguiente);
        primeraEstacion->setAnterior(estacion);
        primeraEstacion = estacion;
    } else { // Añadir en una posición intermedia
        Estacion* actual = primeraEstacion;
        for (int i = 0; i < posicion - 1 && actual->getSiguiente(); ++i) {
            actual = actual->getSiguiente();
        }
        Estacion* siguienteEstacion = actual->getSiguiente();

        estacion->setSiguiente(siguienteEstacion, tiempoSiguiente);
        estacion->setAnterior(actual);

        if (siguienteEstacion) {
            siguienteEstacion->setAnterior(estacion);
            // Actualizar el tiempo de viaje de la estación anterior a la estación actual
            siguienteEstacion->setSiguiente(siguienteEstacion->getSiguiente(), tiempoSiguiente);
        }

        actual->setSiguiente(estacion, tiempoAnterior);
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

int Linea::tiempoDeViaje(Estacion* origen, Estacion* destino) const {
    int tiempoTotal = 0;
    Estacion* actual = primeraEstacion;
    bool contando = false;

    while (actual) {
        if (actual == origen) {
            contando = true;
        }
        if (contando) {
            tiempoTotal += actual->getTiempoSiguiente();
        }
        if (actual == destino) {
            break;
        }
        actual = actual->getSiguiente();
    }

    return (contando && actual == destino) ? tiempoTotal : -1; // Retorna -1 si las estaciones no están en la misma línea o en el orden correcto
}

class Red {
private:
    Linea* primeraLinea;
    Linea* ultimaLinea;
    int numLineas;

public:
    Red() : primeraLinea(nullptr), ultimaLinea(nullptr), numLineas(0) {}

    Estacion* buscarEstacionPorNombre(const string& nombreEstacion) const {
        Linea* tempLinea = primeraLinea;
        while (tempLinea) {
            Estacion* tempEstacion = tempLinea->obtenerPrimeraEstacion();
            while (tempEstacion) {
                if (tempEstacion->getNombre() == nombreEstacion) {
                    return tempEstacion;
                }
                tempEstacion = tempEstacion->getSiguiente();
            }
            tempLinea = tempLinea->getSiguiente();
        }
        return nullptr;
    }


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
                cout << "- Estacion: " << tempEstacion->getNombre();
                if (tempEstacion->getTransferencia()) {
                    cout << " - estacion con transferencia a la estacion: ";
                    // Obtener las líneas conectadas a esta estación de transferencia
                    Linea** lineas = tempEstacion->getLineas();
                    for (int i = 0; i < tempEstacion->getNumeroLineas(); ++i) {
                        cout << " " << lineas[i]->getNombre();
                    }
                }
                if (tempEstacion->getSiguiente()) {
                    cout << " (" << tempEstacion->getTiempoSiguiente() << " minutos)";
                }
                cout << endl;
                tempEstacion = tempEstacion->getSiguiente();
            }
            tempLinea = tempLinea->getSiguiente();
        }
    }

    Linea* getPrimeraLinea() const {
        return primeraLinea;
    }

    Linea* buscarLineaPorNombre(const string& nombreLinea) const {
        Linea* tempLinea = primeraLinea;
        while (tempLinea) {
            if (tempLinea->getNombre() == nombreLinea) {
                return tempLinea;
            }
            tempLinea = tempLinea->getSiguiente();
        }
        return nullptr;
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
        cout << "1. Crear linea" << endl;
        cout << "2. Eliminar linea" << endl;
        cout << "3. Crear estacion" << endl;
        cout << "4. Eliminar estacion" << endl;
        cout << "5. Calcular tiempo de desplazamiento entre estaciones" << endl;
        cout << "6. Salir" << endl;
        cout << "7. Mostrar lineas" << endl;
        cout << "Seleccione una opcion: ";
    }

    // Metodo para crear una linea en la red
    void crearLinea() {
        string nombreLinea;
        bool nombreValido = false;

        while (!nombreValido) {
            cout << "Ingrese el nombre de la linea: ";
            cin >> nombreLinea;

            // Validar si la línea ya existe
            if (buscarLineaPorNombre(nombreLinea) != nullptr) {
                cout << "Error: Una linea con ese nombre ya existe en la red. Por favor, ingrese un nombre diferente." << endl;
            } else {
                nombreValido = true;
            }
        }

        Linea* nuevaLinea = new Linea(nombreLinea);
        agregarLinea(nuevaLinea);

        cout << endl << "Linea creada exitosamente." << endl << endl;
    }


    void crearEstacion() {
        string nombreEstacion, nombreLinea;
        char esTransferencia;

        cout << "Ingrese el nombre de la estacion: ";
        cin >> nombreEstacion;

        // Validar si la estación ya existe
        if (buscarEstacionPorNombre(nombreEstacion) != nullptr) {
            cout << endl << "Error: Ya existe una estacion con ese nombre. Por favor, intentelo de nuevo." << endl;
            return;
        }

        cout << "Ingrese el nombre de la linea a la que desea agregar la estacion: ";
        cin >> nombreLinea;

        // Validación de la entrada para la transferencia
        do {
            cout << "Es una estacion de transferencia? (s/n): ";
            cin >> esTransferencia;
            if (esTransferencia != 's' && esTransferencia != 'S' && esTransferencia != 'n' && esTransferencia != 'N') {
                cout << "Entrada no valida. Por favor ingrese 's', 'S', 'n' o 'N'." << endl;
            }
        } while (esTransferencia != 's' && esTransferencia != 'S' && esTransferencia != 'n' && esTransferencia != 'N');

        Linea* linea = primeraLinea;
        while (linea) {
            if (linea->getNombre() == nombreLinea) {
                Estacion* nuevaEstacion = new Estacion(nombreEstacion);
                if (esTransferencia == 's' || esTransferencia == 'S') {
                    nuevaEstacion->setTransferencia(true);

                    // Mostrar las líneas existentes para seleccionar la transferencia
                    cout << "Lineas existentes en la red:" << endl;
                    Linea* tempLinea = primeraLinea;
                    while (tempLinea) {
                        cout << "- " << tempLinea->getNombre() << endl;
                        tempLinea = tempLinea->getSiguiente();
                    }

                    // Pedir al usuario que seleccione las líneas para la transferencia
                    string nombreOtraLinea;
                    do {
                        cout << "Ingrese el nombre de otra linea para la transferencia (diferente de " << nombreLinea << "): ";
                        cin >> nombreOtraLinea;

                        if (nombreOtraLinea == nombreLinea) {
                            cout << "No puede seleccionar la misma linea para la transferencia." << endl;
                        }
                    } while (nombreOtraLinea == nombreLinea);

                    // Validar si la línea seleccionada existe
                    Linea* otraLinea = primeraLinea;
                    while (otraLinea) {
                        if (otraLinea->getNombre() == nombreOtraLinea) {
                            nuevaEstacion->agregarLinea(otraLinea);
                            otraLinea->marcarTransferencia();  // Marcar la otra línea como que tiene estación de transferencia

                            // Crear la estación correspondiente en la otra línea
                            Estacion* estacionTransferencia = new Estacion(nombreEstacion);
                            estacionTransferencia->setTransferencia(true);
                            estacionTransferencia->agregarLinea(linea);  // Conectar esta estación con la línea original

                            // Mostrar las estaciones existentes y pedir la posición en la otra línea
                            Estacion* tempEstacionOtra = otraLinea->obtenerPrimeraEstacion();
                            cout << "Estaciones existentes en la linea " << nombreOtraLinea << ":" << endl;
                            int posicionOtra = 0;
                            while (tempEstacionOtra) {
                                cout << posicionOtra << ". " << tempEstacionOtra->getNombre() << endl;
                                tempEstacionOtra = tempEstacionOtra->getSiguiente();
                                posicionOtra++;
                            }
                            cout << posicionOtra << ". (Agregar al final)" << endl;

                            int opcionPosicionOtra;
                            cout << "Ingrese el numero de posicion donde desea agregar la nueva estacion en la linea " << nombreOtraLinea << ": ";
                            cin >> opcionPosicionOtra;

                            // Solicitar tiempos de viaje entre estaciones en la otra línea
                            int tiempoAnteriorOtra = 0, tiempoSiguienteOtra = 0;
                            if (opcionPosicionOtra > 0 && opcionPosicionOtra < posicionOtra) {
                                cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion anterior en la linea " << nombreOtraLinea << ": ";
                                cin >> tiempoAnteriorOtra;
                                cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion siguiente en la linea " << nombreOtraLinea << ": ";
                                cin >> tiempoSiguienteOtra;
                            } else if (opcionPosicionOtra == 0) {
                                cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion siguiente en la linea " << nombreOtraLinea << ": ";
                                cin >> tiempoSiguienteOtra;
                            } else if (opcionPosicionOtra == posicionOtra) {
                                cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion anterior en la linea " << nombreOtraLinea << ": ";
                                cin >> tiempoAnteriorOtra;
                            }

                            otraLinea->agregarEstacion(estacionTransferencia, opcionPosicionOtra, tiempoAnteriorOtra, tiempoSiguienteOtra);
                            break;
                        }
                        otraLinea = otraLinea->getSiguiente();
                    }

                    if (!otraLinea) {
                        cout << endl << "La linea especificada no existe en la red." << endl;
                        delete nuevaEstacion; // Liberar memoria si la línea no existe
                        return;
                    }
                }
                // Mostrar las estaciones existentes y pedir la posición
                Estacion* tempEstacion = linea->obtenerPrimeraEstacion();
                cout << "Estaciones existentes en la linea " << nombreLinea << ":" << endl;
                int posicion = 0;
                while (tempEstacion) {
                    cout << posicion << ". " << tempEstacion->getNombre() << endl;
                    tempEstacion = tempEstacion->getSiguiente();
                    posicion++;
                }
                cout << posicion << ". (Agregar al final)" << endl;

                int opcionPosicion;
                cout << "Ingrese el numero de posicion donde desea agregar la nueva estacion en la linea " << nombreLinea << ": ";
                cin >> opcionPosicion;

                // Solicitar tiempos de viaje entre estaciones en la línea principal
                int tiempoAnterior = 0, tiempoSiguiente = 0;
                if (opcionPosicion > 0 && opcionPosicion < posicion) {
                    cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion anterior en la linea " << nombreLinea << ": ";
                    cin >> tiempoAnterior;
                    cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion siguiente en la linea " << nombreLinea << ": ";
                    cin >> tiempoSiguiente;
                } else if (opcionPosicion == 0) {
                    cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion siguiente en la linea " << nombreLinea << ": ";
                    cin >> tiempoSiguiente;
                } else if (opcionPosicion == posicion) {
                    cout << "Ingrese el tiempo de viaje entre la nueva estacion y la estacion anterior en la linea " << nombreLinea << ": ";
                    cin >> tiempoAnterior;
                }

                // Agregar la nueva estación a la línea principal
                linea->agregarEstacion(nuevaEstacion, opcionPosicion, tiempoAnterior, tiempoSiguiente);
                cout << endl << "Estacion creada exitosamente." << endl;
                return;
            }
            linea = linea->getSiguiente();
        }

        cout << endl << "La linea especificada no existe en la red." << endl;
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
                        // Verificar si la estación es una estación de transferencia
                        if (estacion->getTransferencia()) {
                            cout << endl << "No se puede eliminar una estacion de transferencia." << endl;
                            return;
                        }
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

        cout << endl << "La linea especificada no existe en la red." << endl;
    }

    void calcularTiempoDeDesplazamiento() {
        string nombreLinea, nombreEstacionOrigen, nombreEstacionDestino;

        cout << "Ingrese el nombre de la linea: ";
        cin >> nombreLinea;
        cout << "Ingrese el nombre de la estacion de origen: ";
        cin >> nombreEstacionOrigen;
        cout << "Ingrese el nombre de la estacion de destino: ";
        cin >> nombreEstacionDestino;

        Linea* linea = buscarLineaPorNombre(nombreLinea);
        if (!linea) {
            cout << "La linea especificada no existe en la red." << endl;
            return;
        }

        Estacion* estacionOrigen = buscarEstacionPorNombre(nombreEstacionOrigen);
        Estacion* estacionDestino = buscarEstacionPorNombre(nombreEstacionDestino);

        if (!estacionOrigen || !estacionDestino) {
            cout << "Una o ambas estaciones especificadas no existen." << endl;
            return;
        }

        int tiempo = linea->tiempoDeViaje(estacionOrigen, estacionDestino);
        if (tiempo == -1) {
            cout << "Las estaciones no están en la misma línea o no se pudo calcular el tiempo de viaje." << endl;
        } else {
            cout << "El tiempo de viaje entre " << nombreEstacionOrigen << " y " << nombreEstacionDestino << " es de " << tiempo << " minutos." << endl;
        }
    }

};

// Función para verificar que la entrada no sea un número
int entradaSoloNumeros() {
    string input;
    int opcion;
    bool valid = false;
    while (!valid) {
        cin >> input;
        valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            opcion = stoi(input);
        } else {
            cout << "Opción invalida, intente de nuevo, ingrese solamente numeros: ";
        }
    }
    return opcion;
}


int main() {
    Red red;
    int opcion;
    cout << "Bienvenido al sistema de gestion de la red de metro." << endl;
    do {
        red.mostrarMenu();
        opcion = entradaSoloNumeros();
        cout << endl;

        switch (opcion) {
        case 1:
            red.crearLinea();
            break;
        case 2: {
            string nombreLinea;
            cout << "Ingrese el nombre de la linea que desea eliminar: ";
            cin >> nombreLinea;

            Linea* lineaEliminar = red.buscarLineaPorNombre(nombreLinea);

            if (lineaEliminar) {
                red.eliminarLinea(lineaEliminar);
            } else {
                cout << endl << "La linea especificada no existe en la red." << endl;
            }
            break;
        }
        case 3:
            red.crearEstacion();
            break;
        case 4: {
            red.eliminarEstacion();
            break;
        }
        case 5:
            red.calcularTiempoDeDesplazamiento();
            break;
        case 6:
            cout << "¡Hasta luego!." << endl;
            break;
        case 7:
            red.mostrarEstructura();
            break;
        default:
            cout << "Opción no valida. Intente de nuevo." << endl;
            break;
        }
    } while (opcion != 6);

    return 0;
}
