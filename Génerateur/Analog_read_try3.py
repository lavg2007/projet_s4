import numpy as np
import serial as sr
import matplotlib.pyplot as plt
import datetime

#timeout = time.time() + 20*1
#data = np.genfromtxt('capteur1.txt', dtype='int16')
start_time = datetime.datetime.now()
print(start_time)

data = sr.Serial('COM4',9600)
frequence = []
amplitude = []
t = []
temps = []

while True:
    # Initialisation du compteur pour le break
    """
    test = 0
    if test == 1:
        break
    test -= 1
    """
    """
    x = len(qqchose)
    if (x == 10):
        break
    """

    time_delta = datetime.datetime.now() - start_time
    print(time_delta)
    if time_delta.total_seconds() >= 20:
        break

    line = data.read(9) #nombre de caracteres a lire
    print(line)
    #print ("Variable type of line is", type(line))

    # Conversion de line (bytes) en string
    str = bytes.decode(line)
    print(str)

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

    #Création d'un array de temps pour le graphique
    t.append(time_delta)
    #print(type(temps))

for i in range(0,len(t)):
    temps.append((t[i].microseconds*(10**-6))+(t[i].seconds))
#print(temps)

plt.figure(1)
plt.plot(temps,frequence)
#plt.gcf().autofmt_tempsdate()
plt.title("Capteur contrôlant la fréquence")
plt.xlabel("Temps (s)")
plt.ylabel("Fréquence (Hz)")
plt.show()
freq = np.asarray(frequence)
#freq = [15, 48, 79, 110, 157, 121, 185, 222, 231, 227, 288, 369, 429, 501, 579, 640, 704, 792, 861, 950, 542, 103, 51]
#print(type(freq))
#freq = bytes.decode(frequence)

phi = 0

#f = 1
fe = 44100

n = np.arange(fe)

sine = np.zeros(fe)

i = 0
while i < fe:
    sine = np.sin(2*np.pi*phi)
    phi = (phi + freq/fe) % 1
    i = i + 1
#    if i < fe / 2:
    if (i % 1000) == 0 :
        freq = freq + 1
#    else:
#        if (i % 250) == 0 :
#            f = f - 1

xscale = n/fe
plt.figure(2)
plt.plot(xscale[0:len(sine)], sine)


