import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('trayectoria_corazon.dat', skiprows=1)
x = data[:,1]
y = data[:,3]

# Trayectoria en el plano xy
plt.figure(figsize=(6,6))
plt.plot(x, y)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Trayectoria de la partícula')
plt.axis('equal')
plt.grid(True)
plt.savefig('trayectoria_xy.png')
plt.close()

# x(t)
t = data[:,0]
plt.figure()
plt.plot(t, x)
plt.xlabel('t')
plt.ylabel('x')
plt.title('x vs t')
plt.grid(True)
plt.savefig('x_vs_t.png')
plt.close()

# y(t)
plt.figure()
plt.plot(t, y)
plt.xlabel('t')
plt.ylabel('y')
plt.title('y vs t')
plt.grid(True)
plt.savefig('y_vs_t.png')
plt.close()

# theta(t)
theta = data[:,5]
plt.figure()
plt.plot(t, theta)
plt.xlabel('t')
plt.ylabel('theta')
plt.title('theta vs t')
plt.grid(True)
plt.savefig('theta_vs_t.png')
plt.close()

# omega(t)
omega = data[:,6]
plt.figure()
plt.plot(t, omega)
plt.xlabel('t')
plt.ylabel('omega')
plt.title('omega vs t')
plt.grid(True)
plt.savefig('omega_vs_t.png')
plt.close()