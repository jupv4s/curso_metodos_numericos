import matplotlib.pyplot as plt
import numpy as np

# Datos experimentales
data = np.loadtxt('dependencias/datos_del_experimento/datos_periodo2_vs_largo.txt')
L = data[:, 0]
T_squared = data[:, 1]

# Ajuste lineal
def linear_fit(x, m, c):
    return m * x + c

# Parámetros del ajuste lineal (según la ecuación T^2 = 4.24L - 0.48)
m = 4.24
c = -0.48

# Valores predichos para el ajuste lineal
T_squared_fit = linear_fit(L, m, c)

# Graficar
plt.figure()
plt.scatter(L, T_squared, label='Datos')
plt.plot(L, T_squared_fit, label='Ajuste Lineal', color='red')
plt.title('$T^2$ en función de largo del Péndulo $L$')
plt.xlabel('$L$ [m]')
plt.ylabel('$T^2$ [s$^2$]')
plt.legend()
# plt.grid(True, which='both', axis='x')
plt.grid(False)

plt.savefig('grafica_pendulo_simple.pdf')
# plt.show()