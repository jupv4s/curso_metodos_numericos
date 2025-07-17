import matplotlib.pyplot as plt
import numpy as np

def main():
    """Genera y muestra solo el gráfico obligatorio de energía cinética."""
    try:
        t, energia = np.loadtxt('energia_cinetica.dat', skiprows=1, unpack=True)
        plt.figure(figsize=(12, 6))
        plt.plot(t, energia, label='Energía Cinética')
        plt.xlabel('Tiempo (t)')
        plt.ylabel('Energía Cinética (E)')
        plt.title('Energía Cinética vs. Tiempo')
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig('energia_cinetica.png')
        print("Gráfico 'energia_cinetica.png' generado.")
        plt.show()
    except Exception as e:
        print(f"Error al graficar: {e}")

if __name__ == '__main__':
    main()