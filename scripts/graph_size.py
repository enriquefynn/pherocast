#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt
from datetime import datetime

totalLine = 0
nCorrects = 0
trip = 1
size = []

for line in fileinput.input():
    totalLine+=1
    size+= [float(line)/1000.]

plt.plot(range(totalLine), size, 'r-')
plt.xlabel('Period')
plt.ylabel('Size in Kb')

plt.show();
