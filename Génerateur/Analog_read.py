import numpy as np
import serial as sr
import matplotlib.pyplot as plt
import time

#timeout = time.time() + 20*1
#data = np.genfromtxt('capteur1.txt', dtype='int16')

data = sr.Serial('COM3',9600)
frequence = []
amplitude = []

while True:
    # Initialisation du compteur pour le break
    """
    test = 0
    if test == 1:
        break
    test -= 1
    """
    x = len(frequence)
    if (x == 35):
        break

    line = data.read(9) #nombre de caracteres a lire
    #print(line)
    #print ("Variable type of line is", type(line))

    # Conversion de line (bytes) en string
    str = bytes.decode(line)
    #print(str)

    # Séparation de la string pour avoir les valeurs de chaques capteurs indépendament
    cap1_str, cap2_str = str.split(';')
    #print("Variable type of cap1_str is", type(cap1_str))
    #print(cap1_str,cap2_str)

    # Conversion des capteurs (string) en int
    cap1_int = int(cap1_str)
    #print("Variable type of cap1 is", type(cap1_int))
    cap2_int = int(cap2_str)
    #print("Variable type of cap2 is", type(cap2_int))

    # Création d'arrays des valeurs des capteurs
    frequence.append(cap1_int)
    #print("Fréquence :", frequence)
    amplitude.append(cap2_int)
    #print("Volume :", amplitude)

    # Graphique en temps réel
    #plt.plot(frequence)
    #plt.title("Fréquence")
    #plt.xlabel("Temps (s)")
    #plt.ylabel("Fréquence (Hz)")
    #plt.show()

plt.plot(frequence)
plt.title("Capteur contrôlant la fréquence")
plt.xlabel("Temps (s)")
plt.ylabel("Fréquence (Hz)")
plt.show()
freq = np.asarray(frequence)
print(type(freq))
#freq = bytes.decode(frequence)

phi = 0

#f = 1
fe = 44100

n = np.arange(fe)

sine = np.zeros(fe)
i = 0
while i < fe:
    sine[i] = np.sin(2 * np.pi * phi)
    phi = ((phi + freq) / fe)
    phi = np.remainder(phi, 1)
    i = i + 1
    #    if i < fe / 2:
    if (i % 1000) == 0:
        freq = freq + 1
#    else:
#        if (i % 250) == 0 :
#            f = f - 1

plt.figure(1)
plt.plot(n / fe, sine)

#print(data)

