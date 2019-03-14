# -*- coding: utf-8 -*-
"""
Created on Sun Mar  3 23:40:07 2019

@author: Gabriel
"""

import numpy as np
import matplotlib.pyplot as plt
import numpy.fft as fft

phi = 0

f = np.arange(20,20000,19980/44100)
fe = 44100

n = np.arange(fe)

sine = np.zeros(fe)
square = np.zeros(fe)

i = 0

N = 1024
fc1 = 10000
k = np.arange(-N/2,N/2,1)
n2 = np.arange(N)

K = (2*(N*(fc1/fe)))+1
h1 = (1/N)*((np.sin(np.pi*k*(K/N)))/np.sin(np.pi*(k/N)))
h1[int(N/2)]= K/N
H1 = fft.fft(h1)


phi_ary = np.zeros(fe)
while i < fe:
    sine[i] = np.sin(2*np.pi*phi) 
    if phi <= 0.5:
        square[i] = 1
    else:
        square[i] = 0
    phi = (phi + f[i]/fe) % 1
    i = i + 1




plt.figure(0)
plt.plot(n/fe, f)
plt.xlabel("temps(s)")
plt.ylabel("fréquence")

plt.figure(1)
plt.plot(n/fe, sine)
plt.xlabel("temps(s)")
plt.ylabel("amplitude")


plt.figure(2)
plt.plot(n/fe, square)
plt.xlabel("temps(s)")
plt.ylabel("amplitude")

plt.figure(3)
plt.plot(n2*fe/N,np.abs(H1))