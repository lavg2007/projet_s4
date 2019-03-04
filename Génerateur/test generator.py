# -*- coding: utf-8 -*-
"""
Created on Sun Mar  3 23:40:07 2019

@author: Gabriel
"""

import numpy as np
import matplotlib.pyplot as plt

phi = 0

f = 1
fe = 44100

n = np.arange(fe)

sine = np.zeros(fe)
i = 0
while i < fe:
    sine[i] = np.sin(2*np.pi*phi)    
    phi = (phi + f/fe) % 1
    i = i + 1
#    if i < fe / 2:        
    if (i % 1000) == 0 :
        f = f + 1
#    else:
#        if (i % 250) == 0 :
#            f = f - 1


plt.figure(1)
plt.plot(n/fe, sine)
