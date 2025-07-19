import numpy as np
import matplotlib.pyplot as plt

# Cargar los datos de las trayectorias
try:
    data1 = np.loadtxt('trayectoria1.txt')
    data2 = np.loadtxt('trayectoria2.txt')
except FileNotFoundError:
    print("Error: No se encontraron los archivos de datos. Asegúrate de compilar y ejecutar el código C++ primero.")
    exit()

# Extraer coordenadas
t1, x1, y1 = data1[:, 0], data1[:, 1], data1[:, 2]
t2, x2, y2 = data2[:, 0], data2[:, 1], data2[:, 2]

# --- Graficar las trayectorias ---
plt.style.use('seaborn-v0_8-darkgrid')
fig, ax = plt.subplots(figsize=(10, 10))

# --- Dibujar el potencial como fondo ---
r0 = 10.0
sigma = 10.0
x_grid = np.linspace(-15, 15, 400)
y_grid = np.linspace(-15, 15, 400)
X, Y = np.meshgrid(x_grid, y_grid)
R = np.sqrt(X**2 + Y**2)
U = np.exp(-sigma**2 * (R - r0)**2)

# Usar contourf para un fondo de color suave
contour = ax.contourf(X, Y, U, levels=20, cmap='viridis', alpha=0.6)
fig.colorbar(contour, ax=ax, label='Potencial $U(x,y)$')

# Dibujar el anillo del máximo potencial
ring = plt.Circle((0, 0), r0, color='red', fill=False, linestyle='--', linewidth=2, label=f'Anillo ($r_0={r0}$)')
ax.add_artist(ring)


# --- Graficar las trayectorias ---
ax.plot(x1, y1, label='Partícula 1', color='cyan', linewidth=1.5)
ax.plot(x2, y2, label='Partícula 2', color='magenta', linewidth=1.5)

# Marcar puntos iniciales y finales
ax.plot(x1[0], y1[0], 'o', color='blue', markersize=8, label='Inicio P1')
ax.plot(x1[-1], y1[-1], 'x', color='blue', markersize=10, mew=2, label='Fin P1')
ax.plot(x2[0], y2[0], 'o', color='purple', markersize=8, label='Inicio P2')
ax.plot(x2[-1], y2[-1], 'x', color='purple', markersize=10, mew=2, label='Fin P2')


# --- Configuración del gráfico ---
ax.set_title('Trayectorias de dos partículas en un potencial anular (Método PEFRL)', fontsize=16)
ax.set_xlabel('$x$', fontsize=12)
ax.set_ylabel('$y$', fontsize=12)
ax.set_aspect('equal', adjustable='box')
ax.legend()
ax.grid(True)

plt.tight_layout()
plt.savefig('trayectorias.png', dpi=300)
plt.show()