import pandas as pd
import matplotlib.pyplot as plt

# --- Gráfica de la trayectoria y energía ---
df_traj = pd.read_csv('trajectory.csv')

plt.style.use('seaborn-v0_8-whitegrid')
fig, axs = plt.subplots(2, 1, figsize=(12, 12))

# Trayectoria
axs[0].plot(df_traj['x'], df_traj['y'], lw=1, color='dodgerblue')
axs[0].scatter(df_traj['x'].iloc[0], df_traj['y'].iloc[0], color='red', zorder=5, label='Inicio')
axs[0].set_title('Trayectoria de la Partícula (Calculada en C++)')
axs[0].set_xlabel('Posición x')
axs[0].set_ylabel('Posición y')
axs[0].axis('equal')
axs[0].legend()

# Energía Cinética
axs[1].plot(df_traj['t'], df_traj['kinetic_energy'], lw=1.5, color='darkorange')
axs[1].set_title('Energía Cinética vs. Tiempo')
axs[1].set_xlabel('Tiempo (t)')
axs[1].set_ylabel('Energía Cinética ($E_k$)')
axs[1].set_yscale('log')

plt.tight_layout()
plt.show()


# --- Gráfica del experimento de interpolación ---
df_interp = pd.read_csv('interpolation_results.csv')

# Para la gráfica, necesitamos los puntos de los nodos también.
# Podemos leerlos del archivo original de trayectoria.
n_slice = int(len(df_traj) * 0.01)
x_data = df_traj['x'].iloc[:n_slice:2]
y_data = df_traj['y'].iloc[:n_slice:2]

plt.figure(figsize=(14, 8))
plt.plot(x_data, y_data, 'o', color='blue', markersize=8, label='Nodos de Interpolación ($x_{2k}$)')
plt.plot(df_interp['x_real'], df_interp['y_real'], 'x', color='red', markersize=8, mew=2, label='Puntos Reales Eliminados ($x_{2k+1}$)')
plt.plot(df_interp['x_restored'], df_interp['y_restored'], '+', color='limegreen', markersize=10, mew=2, label='Puntos Restaurados por Spline')

plt.title('Restauración de Trayectoria con Spline Cúbico (C++)')
plt.xlabel('Posición x')
plt.ylabel('Posición y')
plt.legend()
plt.axis('equal')
plt.grid(True)
plt.show()