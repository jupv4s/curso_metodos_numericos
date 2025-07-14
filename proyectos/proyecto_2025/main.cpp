#include <iostream>

using namespace std;

bool esPrimo(int num)
{
    if (num < 2) return false;
    for (int i = 2; i * i <= num; ++i)
    {
        if (num % i == 0) return false;
    }
    return true;
}

int main()
{
    int cantidad = 10;
    int encontrados = 0;
    cout << "Primeros 10 números primos (usando for con rango fijo): ";
    // Suponemos que los primeros 10 primos están antes del 30
    for (int numero = 2; encontrados < cantidad && numero < 30; ++numero)
    {
        if (esPrimo(numero))
        {
            cout << numero;
            ++encontrados;
            if (encontrados < cantidad) cout << ", ";
        }
    }
    cout << endl;
    return 0;
}