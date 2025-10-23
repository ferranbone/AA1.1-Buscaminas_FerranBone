#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

struct Casilla {
    bool esMina = false;
    bool revelado = false;
    int minasCerca = 0;
};

void mostrarTablero(const std::vector<std::vector<Casilla>>& tablero,
    bool revelarTodo,
    char simboloOculto,
    char simboloMina,
    char simboloVacio) {
    int n = tablero.size();

    std::cout << "  ";
    for (int j = 0; j < n; j++) std::cout << j << " ";
    std::cout << std::endl;

    for (int i = 0; i < n; i++) {
        std::cout << static_cast<char>('a' + i) << " ";
        for (int j = 0; j < n; j++) {
            const Casilla& c = tablero[i][j];
            if (revelarTodo || c.revelado) {
                if (c.esMina) std::cout << simboloMina << " ";
                else if (c.minasCerca > 0) std::cout << c.minasCerca << " ";
                else std::cout << simboloVacio << " ";
            }
            else {
                std::cout << simboloOculto << " ";
            }
        }
        std::cout << std::endl;
    }
}

int contarMinas(const std::vector<std::vector<Casilla>>& tablero, int fila, int col) {
    int n = tablero.size();
    int contador = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int ni = fila + i;
            int nj = col + j;
            if (ni >= 0 && ni < n && nj >= 0 && nj < n && tablero[ni][nj].esMina)
                contador++;
        }
    }
    return contador;
}

void revelarCasilla(std::vector<std::vector<Casilla>>& tablero, int fila, int col) {
    int n = tablero.size();
    if (fila < 0 || fila >= n || col < 0 || col >= n) return;

    Casilla& c = tablero[fila][col];
    if (c.revelado || c.esMina) return;

    c.revelado = true;

    if (c.minasCerca == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0)
                    revelarCasilla(tablero, fila + i, col + j);
            }
        }
    }
}

bool haGanado(const std::vector<std::vector<Casilla>>& tablero) {
    int n = tablero.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!tablero[i][j].esMina && !tablero[i][j].revelado)
                return false;
        }
    }
    return true;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int MAX_TAM = 10;
    const char SIMBOLO_OCULTO = '#';
    const char SIMBOLO_MINA = '*';
    const char SIMBOLO_VACIO = '.';

    int tam, minas;
    std::cout << "Introduce el tamano del tablero (max 10): ";
    std::cin >> tam;
    if (tam > MAX_TAM) tam = MAX_TAM;

    std::cout << "Introduce el numero de minas: ";
    std::cin >> minas;

    std::vector<std::vector<Casilla>> tablero(tam, std::vector<Casilla>(tam));

    int colocadas = 0;
    while (colocadas < minas) {
        int i = std::rand() % tam;
        int j = std::rand() % tam;
        if (!tablero[i][j].esMina) {
            tablero[i][j].esMina = true;
            colocadas++;
        }
    }

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (!tablero[i][j].esMina)
                tablero[i][j].minasCerca = contarMinas(tablero, i, j);
        }
    }

    bool gameOver = false;

    while (!gameOver) {
        mostrarTablero(tablero, false, SIMBOLO_OCULTO, SIMBOLO_MINA, SIMBOLO_VACIO);

        if (haGanado(tablero)) {
            std::cout << "\n Has ganado! \n";
            mostrarTablero(tablero, true, SIMBOLO_OCULTO, SIMBOLO_MINA, SIMBOLO_VACIO);
            break;
        }

        char filaChar;
        int col;
        std::cout << "Selecciona casilla (fila letra, columna numero): ";
        std::cin >> filaChar >> col;

        int fila = filaChar - 'a';
        if (fila < 0 || fila >= tam || col < 0 || col >= tam) {
            std::cout << "Coordenadas invalidas.\n";
            continue;
        }

        Casilla& c = tablero[fila][col];
        if (c.revelado) {
            std::cout << "Ya has revelado esa casilla.\n";
            continue;
        }

        if (c.esMina) {
            std::cout << "\n Has pisado una mina! \n";
            mostrarTablero(tablero, true, SIMBOLO_OCULTO, SIMBOLO_MINA, SIMBOLO_VACIO);
            gameOver = true;
            break;
        }

        revelarCasilla(tablero, fila, col);
    }

    return 0;
}
