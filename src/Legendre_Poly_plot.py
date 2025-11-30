import numpy as np
import matplotlib.pyplot as plt

# load the data
data = np.loadtxt("legendre_ad.txt")

x = data[:, 0]

# unpack P_k and P_k' (for k = 0..5)
P0, dP0 = data[:, 1], data[:, 2]
P1, dP1 = data[:, 3], data[:, 4]
P2, dP2 = data[:, 5], data[:, 6]
P3, dP3 = data[:, 7], data[:, 8]
P4, dP4 = data[:, 9], data[:, 10]
P5, dP5 = data[:, 11], data[:, 12]

# ---- plot the polynomials P_k ----
plt.figure()
plt.plot(x, P0, label="P0")
plt.plot(x, P1, label="P1")
plt.plot(x, P2, label="P2")
plt.plot(x, P3, label="P3")
plt.plot(x, P4, label="P4")
plt.plot(x, P5, label="P5")
plt.xlabel("x")
plt.ylabel("P_k(x)")
plt.title("Legendre polynomials P0..P5")
plt.grid()
plt.legend()

# ---- plot the derivatives P_k' ----
plt.figure()
plt.plot(x, dP0, label="P0'")
plt.plot(x, dP1, label="P1'")
plt.plot(x, dP2, label="P2'")
plt.plot(x, dP3, label="P3'")
plt.plot(x, dP4, label="P4'")
plt.plot(x, dP5, label="P5'")
plt.xlabel("x")
plt.ylabel("P_k'(x)")
plt.title("Derivatives of Legendre polynomials P0..P5")
plt.grid()
plt.legend()

plt.show()
