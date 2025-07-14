// DataManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "Vector.h"
#include "Matrix.h"
#include "OdeSolution.h"

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>

class DataManager
{
public:

    template <class T>
    static bool exportVector(const std::vector<T>& data, const std::string& filename, int precision = 4);

    template <class T>
    static bool exportCustomVector(const Vector<T>& vec, const std::string& filename, int precision = 4);

    template <class T>
    static bool exportMatrix(const Matrix<T>& matrix, const std::string& filename, int precision = 4);

    static bool exportOdeSolution(const OdeSolution& result, const std::string& filename, int precision = 4);
};

#include "DataManager.tpp"

#endif