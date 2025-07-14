#include <iostream>

// Definición de la clase Vector2DInt
class Vector2DInt {
public:
    int x; // Componente x del vector
    int y; // Componente y del vector

    // Constructor por defecto
    Vector2DInt() : x(0), y(0) {}

    // Constructor con parámetros
    Vector2DInt(int _x, int _y) : x(_x), y(_y) {}

    // Función para imprimir el vector
    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    // Operador de suma de vectores
    Vector2DInt operator+(const Vector2DInt& other) const {
        return Vector2DInt(x + other.x, y + other.y);
    }
};

int main() {
    // Creación de objetos (instancias) de la clase Vector2DInt
    Vector2DInt v1(3, 4);
    Vector2DInt v2(1, 2);

    std::cout << "v1: ";
    v1.print();
    std::cout << "v2: ";
    v2.print();

    // Suma de vectores
    Vector2DInt v3 = v1 + v2;
    std::cout << "v1 + v2 = ";
    v3.print();

    return 0;
}
