// CommonHeaders.h
#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H

// Includes de tus clases personalizadas
#include "Vector.h"
#include "Matrix.h"
#include "OdeSolver.h"
#include "OdeSolution.h" // es una estructura
#include "DataManager.h"

// Includes de la biblioteca estándar que usas comúnmente
#include <iostream>    // Para std::cout, std::cerr, std::endl
#include <string>      // Para std::string
#include <vector>      // Para std::vector
#include <cmath>       // Para M_PI (necesita _USE_MATH_DEFINES en Windows para M_PI), std::abs, etc.
#include <functional>  // Para std::function (usado en ODESolver)
#include <stdexcept>   // Para std::runtime_error, std::invalid_argument (usado en Matrix, ODESolver)
#include <iomanip>     // Para std::scientific, std::setprecision (usado en DataManager)
#include <typeinfo>    // Para typeid (usado en DataManager)
#include <cassert>     // Para assert (usado en Vector)
#include <limits>      // Para std::numeric_limits (usado en Vector, Matrix)
#include <type_traits> // Para std::is_floating_point_v (usado en Vector, Matrix)

#endif