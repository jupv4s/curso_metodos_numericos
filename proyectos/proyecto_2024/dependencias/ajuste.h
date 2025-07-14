#ifndef Ajuste_H
#define Ajuste_H

#include "math_matriz.h"

// Declaracion de la clase

template <class T>
class Ajuste
{
  private:
    vector<vector<double>> v_dataset;

  public:
    Ajuste();
    ~Ajuste();
    Ajuste(vector<vector<double>>);

    // Ajustes
    vector<double> lineal();
    vector<double> cuadratica();
};

// Implementacion de la clase

template <class T>
Ajuste<T>::Ajuste(){}

template <class T>
Ajuste<T>::~Ajuste(){}

template <class T>
Ajuste<T>::Ajuste(vector<vector<double>> v_dataset)
{
    this -> v_dataset = v_dataset;
}

// Ajuste lineal multivariable
template <class T>
vector<double> Ajuste<T>::lineal()
{
    Matriz<double> m_dataset(v_dataset);
    int orden = m_dataset.ncol()-1;

    Matriz<double> X(m_dataset.nrow(), orden+1);
    
    for (int i = 0; i < m_dataset.nrow(); i+=1)
    {
        X(i,0) = 1;
        for (int j = 1; j < orden+1; j+=1)
        {
            X(i,j) = m_dataset(i, j-1);
        }
    }

    Matriz<double> y(m_dataset.nrow(), 1);
    
    for (int i = 0; i < m_dataset.nrow(); i+=1)
    {
        y(i,0) = m_dataset(i, orden);
    }

    Matriz<double> constantes_matriz = (inversa(transpuesta(X)*X)*transpuesta(X))*y;
    vector<double> constantes;
    
    for (int i = 0; i < orden+1; i+=1)
    {
        constantes.push_back(constantes_matriz.get()[i][0]);
    }
    return constantes;
}

#endif