#include "matrix.h"

using namespace std;

// Variable global para el conteo de intercambios de filas en Gauss para el determinante.
int contador = 0;

/* --- Implementación de la clase matrix --- */

// Constructor por defecto
matrix::matrix(){}

// Destructor
matrix::~matrix(){}

// Constructor con dimensiones (filas, columnas)
matrix::matrix(int rows, int cols)
{
    data.resize(rows);
    for (int i = 0; i < rows; ++i) {
        data[i].resize(cols, 0.0);
    }
}

// Constructor de copia
matrix::matrix(const matrix & other)
{
    data = other.data;
}

// Constructor a partir de un vector de vectores
matrix::matrix(vector<vector<double>> elements)
{
    data = elements;
}

// Constructor para matriz identidad
matrix::matrix(int n)
{
    data.resize(n);
    for (int i = 0; i < n; ++i)
    {
        data[i].resize(n, 0.0);
        data[i][i] = 1.0;
    }
}

// Operador de asignación
matrix matrix::operator = (const matrix & x)
{
    if (this != & x)
    {
        data = x.data;
    }
    return *this;
}

// Acceso a elementos (solo lectura)
double matrix::operator()(int r, int c) const
{
    return data[r][c];
}

// Acceso a elementos (lectura/escritura)
double & matrix::operator()(int r, int c)
{
    return data[r][c];
}

// Retorna todos los elementos de la matriz
vector<vector<double>> matrix::get()
{
    return data;
}

// Retorna el número de filas
int matrix::nrow() const
{
    return data.size();
}

// Retorna el número de columnas
int matrix::ncol() const
{
    if (data.empty()) {
        return 0;
    }
    return data[0].size();
}

// Retorna una fila específica
vector<double> matrix::row(int r) {
    return data[r];
}

// Retorna una columna específica
vector<double> matrix::col(int c) {
    vector<double> res(data.size());
    for(int i = 0; i < data.size(); i++) {
        res[i] = data[i][c];
    }
    return res;
}

// Guarda el contenido de la matriz en un archivo
void matrix::save(string filePath)
{
    ofstream myfile;
    myfile.open(filePath);
    if (!myfile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo '" << filePath << "' para guardar la matriz." << endl;
        return;
    }
    for(int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {
            myfile << fixed << setw(11) << setprecision(6) << data[i][j];
            if(j < data[i].size() - 1)
            {
                myfile << ",";
            }
        }
        myfile << endl;
    }
    myfile.close();
}


/* --- Implementación de funciones globales (operadores y utilidades) --- */

// Sobrecarga del operador << para matrix
ostream & operator << (ostream & os, const matrix & a)
{
    const double EPSILON = 1e-9;

    const auto& x = a.data;

    os << "[";
    for (int i = 0; i < x.size(); ++i)
    {
        if (i > 0) {
            os << "," << endl << " ";
        }
        os << "[";
        for (int j = 0; j < x[i].size(); ++j)
        {
            if (abs(x[i][j]) < EPSILON) {
                os << 0.0;
            } else {
                os << x[i][j];
            }
            if (j < x[i].size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";
    }
    os << "]";
    return os;
}

// Sobrecarga del operador << para vector<double>
ostream & operator << (ostream & os, const vector<double> & v) {
    os << "[";
    for(int i = 0; i < v.size(); ++i) {
        os << v[i];
        if(i + 1 < v.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

// Suma de dos matrices
matrix operator + (const matrix & a1, const matrix & b1)
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    matrix suma(a1.nrow(), a1.ncol());
    for (int i = 0; i < a1.nrow(); ++i)
    {
        for (int j = 0; j < a1.ncol(); ++j)
        {
            suma(i,j) = a1(i,j) + b1(i,j);
        }
    }
    return suma;
}

// Resta de dos matrices
matrix operator - (const matrix & a1, const matrix & b1)
{
    assert(a1.nrow() == b1.nrow() && a1.ncol() == b1.ncol());
    matrix resta(a1.nrow(), a1.ncol());
    for (int i = 0; i < a1.nrow(); ++i)
    {
        for (int j = 0; j < a1.ncol(); ++j)
        {
            resta(i,j) = a1(i,j) - b1(i,j);
        }
    }
    return resta;
}

// Producto de escalar por matriz
matrix operator * (double a1, const matrix & b1)
{
    matrix producto(b1.nrow(), b1.ncol());
    for (int i = 0; i < b1.nrow(); ++i)
    {
        for (int j = 0; j < b1.ncol(); ++j)
        {
            producto(i,j) = a1 * b1(i,j);
        }
    }
    return producto;
}

// Producto matriz por vector (matriz * vector columna)
vector<double> operator * (const matrix & a, const vector<double> &x)
{
    assert(a.ncol() == x.size());

    vector<double> resultado(a.nrow(), 0.0);

    for(int i = 0; i < a.nrow(); ++i) {
        for(int j = 0; j < a.ncol(); ++j) {
            resultado[i] += a(i, j) * x[j];
        }
    }
    return resultado;
}

// Producto vector por matriz (vector fila * matriz)
vector<double> operator * (const vector<double> & v, const matrix & m)
{
    // Para que el producto v * A sea válido, el número de elementos del vector
    // (interpretado como vector fila) debe ser igual al número de filas de la matriz.
    assert(v.size() == m.nrow());

    // El vector resultado tendrá un número de elementos igual al número de columnas de la matriz.
    vector<double> resultado(m.ncol(), 0.0);

    // Iteramos sobre las columnas del resultado
    for (int j = 0; j < m.ncol(); ++j)
    {
        // Para cada elemento del resultado, sumamos los productos del vector v por la columna j de la matriz m
        for (int k = 0; k < v.size(); ++k) // O equivalentemente, k < m.nrow()
        {
            resultado[j] += v[k] * m(k, j);
        }
    }
    return resultado;
}


// Producto entre dos matrices
matrix operator * (const matrix & a1, const matrix & b1)
{
    assert(a1.ncol() == b1.nrow());
    matrix producto(a1.nrow(), b1.ncol());
    for (int i = 0; i < a1.nrow(); ++i)
    {
        for (int j = 0; j < b1.ncol(); ++j)
        {
            for (int k = 0; k < a1.ncol(); ++k)
            {
                producto(i,j) = producto(i,j) +  a1(i,k)*b1(k,j);
            }
        }
    }
    return producto;
}

// Suma de dos vectores
vector<double> operator + (const vector<double> & v1, const vector<double> & v2) {
    assert(v1.size() == v2.size());
    vector<double> res(v1.size());
    for(int i = 0; i < v1.size(); ++i) {
        res[i] = v1[i] + v2[i];
    }
    return res;
}

// Resta de dos vectores
vector<double> operator - (const vector<double> & v1, const vector<double> & v2) {
    assert(v1.size() == v2.size());
    vector<double> res(v1.size());
    for(int i = 0; i < v1.size(); ++i) {
        res[i] = v1[i] - v2[i];
    }
    return res;
}

// Norma euclidiana de un vector
double norm2(const vector<double> & v) {
    double res = 0.0;
    for(int i = 0; i < v.size(); ++i) { res += (v[i] * v[i]); }
    return sqrt(res);
}


/* --- Implementación de Funciones Utilitarias de Álgebra Lineal --- */

// Calcula la matriz transpuesta
matrix transpuesta(const matrix & M)
{
    int filas = M.nrow();
    int columnas = M.ncol();

    matrix m_transpuesta(columnas, filas);

    for (int i = 0; i < filas; ++i)
    {
        for (int j = 0; j < columnas; ++j)
        {
            m_transpuesta(j,i) = M(i,j);
        }
    }
    return m_transpuesta;
}

// Realiza la eliminación de Gauss-Jordan
matrix gauss(const matrix & x)
{
    matrix m_gauss = x;

    contador = 0; // Reinicio de la variable global

    for (int i = 0; i < m_gauss.nrow(); ++i)
    {
        // Búsqueda del pivote más grande en la columna
        double pivote_val = m_gauss(i, i);
        int fila_pivote_maximo = i;
        for (int j = i + 1; j < m_gauss.nrow(); ++j)
        {
            if (abs(m_gauss(j, i)) > abs(pivote_val))
            {
                pivote_val = m_gauss(j, i);
                fila_pivote_maximo = j;
            }
        }

        // Intercambio de filas si es necesario
        if (fila_pivote_maximo != i)
        {
            for (int j = 0; j < m_gauss.ncol(); ++j)
            {
                double temp = m_gauss(i, j);
                m_gauss(i, j) = m_gauss(fila_pivote_maximo, j);
                m_gauss(fila_pivote_maximo, j) = temp;
            }
            contador += 1;
        }

        // Advertencia si el pivote es cero
        if (m_gauss(i, i) == 0.0) {
            cerr << "Error: Pivote cero o muy cercano a cero durante la eliminación gaussiana. La matriz puede ser singular." << endl;
        }

        // Eliminación hacia abajo (reducción de elementos debajo del pivote)
        for (int j = 0; j < m_gauss.nrow(); ++j)
        {
            if (i != j)
            {
                double factor = m_gauss(j, i) / m_gauss(i, i);
                for (int k = 0; k < m_gauss.ncol(); ++k)
                {
                    m_gauss(j, k) = m_gauss(j, k) - factor * m_gauss(i, k);
                }
            }
        }
    }
    return m_gauss;
}

// Calcula el determinante de una matriz cuadrada
double determinante(const matrix & x)
{
    assert(x.nrow() == x.ncol());

    matrix m_reducida = gauss(x);
    double det_val = 1.0;

    det_val = det_val * pow(-1, contador);

    for (int i = 0; i < x.nrow(); ++i)
    {
        det_val = det_val * m_reducida(i, i);
    }
    return det_val;
}

// Calcula la matriz inversa
matrix inversa(const matrix & x)
{
    assert(x.nrow() == x.ncol());

    matrix m_extendida(x.nrow(), x.ncol() * 2);

    // Copia la matriz original
    for (int i = 0; i < x.nrow(); ++i)
    {
        for (int j = 0; j < x.ncol(); ++j)
        {
            m_extendida(i, j) = x(i, j);
        }
    }

    // Rellena la parte derecha con la matriz identidad
    for (int i = 0; i < x.nrow(); ++i)
    {
        m_extendida(i, i + x.ncol()) = 1.0;
    }

    // Aplica Gauss-Jordan
    m_extendida = gauss(m_extendida);

    // Normaliza cada fila para que los pivotes sean 1
    for (int i = 0; i < x.nrow(); ++i)
    {
        double factor = m_extendida(i, i);
        if (factor == 0.0) {
            cerr << "Error: Matriz singular, no se puede calcular la inversa." << endl;
            return matrix(0,0);
        }

        for (int j = 0; j < m_extendida.ncol(); ++j)
        {
            m_extendida(i, j) = m_extendida(i, j) / factor;
        }
    }

    // Extrae la matriz inversa de la parte derecha
    matrix m_inversa(x.nrow(), x.ncol());
    for (int i = 0; i < x.nrow(); ++i)
    {
        for (int j = 0; j < x.ncol(); ++j)
        {
            m_inversa(i, j) = m_extendida(i, j + x.ncol());
        }
    }
    return m_inversa;
}
