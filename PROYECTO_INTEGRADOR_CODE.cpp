#include <iostream>
#include <vector>
#include <limits>

const int TAMANO_TABLERO = 5;

class Barco {
public:
    // Constructor de la clase Barco
    Barco(int x, int y) : x(x), y(y), hundido(false) {}

    // Método para verificar si hay un impacto en las coordenadas dadas
    bool impacto(int x, int y) const {
        if (this->x == x && this->y == y) {
            hundido = true;
            return true; // Barco impactado
        }
        return false; // Agua
    }

    // Método para verificar si el barco está hundido
    bool estaHundido() const {
        return hundido;
    }

private:
    int x, y;
    mutable bool hundido; // Marcar como mutable para permitir modificaciones en método const
};

class Tablero {
public:
    // Constructor de la clase Tablero
    Tablero() {
        // Inicializar barcos vacíos
        for (int i = 0; i < 5; ++i) {
            barcos.push_back(Barco(0, 0));
        }

        // Inicializar el tablero como vacío
        for (int i = 0; i < TAMANO_TABLERO; ++i) {
            std::vector<char> fila(TAMANO_TABLERO, ' ');
            tablero.push_back(fila);
        }
    }

    // Método para que un jugador coloque sus barcos en el tablero
    void colocarBarcos(const std::string& nombre) {
        std::cout << nombre << ":" << std::endl;
        for (int i = 0; i < 5; ++i) {
            int x, y;
            std::cout << "Indica la posición de tu " << obtenerNombreBarco(i + 1) << " barco (x y): ";
            while (!(std::cin >> x >> y) || !coordenadasValidas(x, y) || casillaConBarco(x, y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (!coordenadasValidas(x, y)) {
                    std::cout << "Coordenadas fuera de los límites del tablero. ";
                } else if (casillaConBarco(x, y)) {
                    std::cout << "Ya hay un barco en esas coordenadas. ";
                }
                std::cout << "Por favor, ingresa coordenadas válidas." << std::endl;
                std::cout << "Indica la posición de tu " << obtenerNombreBarco(i + 1) << " barco (x y): ";
            }
            barcos[i] = Barco(x, y);
        }
    }

    // Método para realizar un disparo en el tablero
    bool disparar(int x, int y) {
        if (casillaDisparada(x, y)) {
            std::cout << "Ya has disparado en estas coordenadas. Elige otras." << std::endl;
            return false;
        }

        for (auto& barco : barcos) {
            if (barco.impacto(x, y)) {
                if (barco.estaHundido()) {
                    std::cout << "¡Impacto y hundido!" << std::endl;
                } else {
                    std::cout << "¡Impacto!" << std::endl;
                }
                marcarDisparo(x, y);
                return true;
            }
        }

        std::cout << "Agua" << std::endl;
        marcarDisparo(x, y);
        return false;
    }

    // Método para imprimir el estado actual del tablero
    void imprimirTablero() const {
        // Imprimir encabezado de las coordenadas en el eje X
        std::cout << "  ";
        for (int i = 1; i <= TAMANO_TABLERO; ++i) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // Imprimir el cuerpo del tablero
        for (int i = 0; i < TAMANO_TABLERO; ++i) {
            // Imprimir número de fila en el eje Y
            std::cout << i + 1 << " ";
            for (int j = 0; j < TAMANO_TABLERO; ++j) {
                std::cout << tablero[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Método para verificar si todos los barcos han sido hundidos
    bool todosBarcosHundidos() const {
        for (const auto& barco : barcos) {
            if (!barco.estaHundido()) {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<Barco> barcos;
    std::vector<std::vector<char>> tablero; // Matriz para representar el tablero

    // Método para verificar si unas coordenadas son válidas en el tablero
    bool coordenadasValidas(int x, int y) const {
        return x >= 1 && x <= TAMANO_TABLERO && y >= 1 && y <= TAMANO_TABLERO;
    }

    // Método para verificar si una casilla tiene un barco
    bool casillaConBarco(int x, int y) const {
        for (const auto& barco : barcos) {
            if (barco.impacto(x, y)) {
                return true;
            }
        }
        return false;
    }

    // Método para verificar si una casilla ya ha sido disparada
    bool casillaDisparada(int x, int y) const {
        return tablero[x - 1][y - 1] != ' ';
    }

    // Método para marcar el resultado de un disparo en el tablero
    void marcarDisparo(int x, int y) {
        if (coordenadasValidas(x, y)) {
            tablero[x - 1][y - 1] = casillaConBarco(x, y) ? 'X' : '*';
        }
    }

    // Método para obtener el nombre del barco según su número
    std::string obtenerNombreBarco(int numero) const {
        switch (numero) {
            case 1:
                return "primer";
            case 2:
                return "segundo";
            case 3:
                return "tercer";
            case 4:
                return "cuarto";
            case 5:
                return "quinto";
            default:
                return "";
        }
    }
};

class Jugador {
public:
    // Constructor de la clase Jugador
    Jugador(const std::string& nombre) : nombre(nombre), tablero() {
        tablero.colocarBarcos(nombre);
    }

    // Método que representa el turno de un jugador
    void turno(Jugador& oponente) {
        int x, y;
        while (true) {
            std::cout << nombre << ", ingresa coordenadas para disparar (x y): ";
            while (!(std::cin >> x >> y) || !coordenadasValidas(x, y)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entrada no válida. Por favor, ingresa coordenadas válidas." << std::endl;
                std::cout << nombre << ", ingresa coordenadas para disparar (x y): ";
            }

            if (oponente.tablero.disparar(x, y)) {
                if (oponente.tablero.todosBarcosHundidos()) {
                    std::cout << "¡Todos los barcos de " << oponente.nombre << " han sido hundidos!" << std::endl;
                }
            }

            break; // Salir del bucle si las coordenadas son válidas
        }
    }

    // Método para imprimir el tablero del jugador
    void imprimirTablero() const {
        std::cout << "Tablero de " << nombre << ":" << std::endl;
        tablero.imprimirTablero();
    }

    // Método para verificar si todos los barcos del jugador han sido hundidos
    bool todosBarcosHundidos() const {
        return tablero.todosBarcosHundidos();
    }

private:
    std::string nombre;
    Tablero tablero;

    // Método para verificar si unas coordenadas son válidas en el tablero
    bool coordenadasValidas(int x, int y) const {
        return x >= 1 && x <= TAMANO_TABLERO && y >= 1 && y <= TAMANO_TABLERO;
    }
};

// Función principal del juego
int main() {
    Jugador jugador1("Jugador 1");
    Jugador jugador2("Jugador 2");

    // Bucle principal del juego
    while (true) {
        std::cout << "Turno de " << (jugador1.todosBarcosHundidos() ? "Jugador 2" : "Jugador 1") << std::endl;
        jugador1.turno(jugador2);
        jugador2.imprimirTablero();

        if (jugador2.todosBarcosHundidos()) {
            std::cout << "¡Jugador 1 ganador!" << std::endl;
            break;
        }

        std::cout << "Turno de " << (jugador2.todosBarcosHundidos() ? "Jugador 1" : "Jugador 2") << std::endl;
        jugador2.turno(jugador1);
        jugador1.imprimirTablero();

        if (jugador1.todosBarcosHundidos()) {
            std::cout << "¡Jugador 2 ganador!" << std::endl;
            break;
        }
    }

    return 0;
}
