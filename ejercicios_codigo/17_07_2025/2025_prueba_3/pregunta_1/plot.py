import numpy as np
import matplotlib.pyplot as plt

# script para graficar la solución del sistema de lorenz
# lee los datos generados por el programa c++ y genera dos gráficos: variables vs tiempo y órbita 3d

# leer los datos del archivo generado por el programa c++
t, x, y, z = np.loadtxt('lorenz.dat', skiprows=2, unpack=True)

plt.figure(figsize=(10, 6))
# graficar x(t), y(t), z(t) en función del tiempo
plt.plot(t, x, label='x(t)')
plt.plot(t, y, label='y(t)')
plt.plot(t, z, label='z(t)')
plt.xlabel('t')
plt.ylabel('variables')
plt.title('variables del sistema de Lorenz vs tiempo')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('lorenz_vs_t.png', dpi=300)
# guardar el gráfico como png
plt.show()

# graficar la trayectoria en el espacio de fases (x, y, z)
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')
ax.plot(x, y, z, lw=1)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title('órbita en el espacio de fases (Lorenz)')
plt.tight_layout()
plt.savefig('lorenz_orbita_3d.png', dpi=300)
# guardar el gráfico 3d como png
plt.show()