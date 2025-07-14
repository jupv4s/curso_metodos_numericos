#include <iostream>

// Definición de la plantilla de clase Vector2D
template <typename T> // 'T' es un parámetro de tipo genérico
class Vector2D {
public:
    T x; // Componente x, de tipo T
    T y; // Componente y, de tipo T

    // Constructor por defecto
    Vector2D() : x(T()), y(T()) {} // Inicializa con el valor por defecto de T

    // Constructor con parámetros
    Vector2D(T _x, T _y) : x(_x), y(_y) {}

    // Función para imprimir el vector
    void print() const {
        std::cout << "Vector2D<" << typeid(T).name() << ">(" << x << ", " << y << ")" << std::endl;
    }

    // Operador de suma de vectores
    Vector2D<T> operator+(const Vector2D<T>& other) const {
        return Vector2D<T>(x + other.x, y + other.y);
    }
};

int main() {
    // Uso de la plantilla de clase con diferentes tipos

    // Vector 2D con enteros (el compilador instancia Vector2D<int>)
    Vector2D<int> v_int1(3, 4);
    Vector2D<int> v_int2(1, 2);
    std::cout << "v_int1: ";
    v_int1.print();
    std::cout << "v_int2: ";
    v_int2.print();
    Vector2D<int> v_int_sum = v_int1 + v_int2;
    std::cout << "v_int1 + v_int2 = ";
    v_int_sum.print();
    std::cout << std::endl;

    // Vector 2D con números de punto flotante (el compilador instancia Vector2D<float>)
    Vector2D<float> v_float1(3.5f, 4.2f);
    Vector2D<float> v_float2(1.1f, 2.3f);
    std::cout << "v_float1: ";
    v_float1.print();
    std::cout << "v_float2: ";
    v_float2.print();
    Vector2D<float> v_float_sum = v_float1 + v_float2;
    std::cout << "v_float1 + v_float2 = ";
    v_float_sum.print();
    std::cout << std::endl;

    // Vector 2D con números de doble precisión (el compilador instancia Vector2D<double>)
    Vector2D<double> v_double1(10.123, 20.456);
    Vector2D<double> v_double2(5.0, 7.0);
    std::cout << "v_double1: ";
    v_double1.print();
    std::cout << "v_double2: ";
    v_double2.print();
    Vector2D<double> v_double_sum = v_double1 + v_double2;
    std::cout << "v_double1 + v_double2 = ";
    v_double_sum.print();

    return 0;
}
