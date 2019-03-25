# -*- coding: utf-8 -*-
"""
Created on Mon Mar 25 15:04:35 2019

@author: Gabriel
"""

import numpy as np
import matplotlib.pyplot as plt

capMin = 100
capMax = 950



fs = 20000
cap = np.arange(capMin, capMax, 1)

freq = (18.44*np.exp(0.0057*cap))/fs

with open('LUT_phase.h', 'w') as fd:
    fd.write('float LUT_phase[850] = {\n')
    for x in freq:
        fd.write(f'{x},\n')
    fd.write('}')
plt.figure(0)
plt.plot(cap, freq)