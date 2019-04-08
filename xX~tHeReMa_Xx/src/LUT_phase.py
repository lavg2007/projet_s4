# -*- coding: utf-8 -*-
"""
Created on Mon Mar 25 15:04:35 2019

@author: Gabriel
"""

import numpy as np
import matplotlib.pyplot as plt

capMin = 100
capMax = 1000
range = 2**11


fs = 20000
cap = np.arange(capMin, capMax, 1)

#freq = np.round((18.44*np.exp(0.0057*cap))/fs*(2**15))
freq = 19.073*np.exp(0.0054*cap)/fs
Qfreq = np.round(freq *range)

nsin = np.arange(range)
sin = np.round(np.sin(2*np.pi*nsin/range) * range)

with open('LUT_phase.h', 'w') as fd:
    fd.write(f'int LUT_phase[{len(cap)}] = {{\n')
    for x in Qfreq:
        fd.write(f'{x},\n')
    fd.write('};\n')
    fd.write(f'int LUT_sin[{len(nsin)}] = {{\n')
    for x in sin:
        fd.write(f'{x},\n')
    fd.write('};')

plt.figure(0)
plt.plot(cap, Qfreq)
# plt.show()


plt.figure(1)
plt.plot(nsin, sin)
# plt.show()

plt.figure(2)
plt.plot(cap, freq*fs)
plt.show()
