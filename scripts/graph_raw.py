#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt
from datetime import datetime
from math import hypot

yLocPlot = []
xLocPlot = []

for line in fileinput.input():
    line = line.split()
    x = float(line[1])#*0.00384)
    y = float(line[2])#*0.00384)
    if x != 0 and y != 0:
        xLocPlot += [x]
        yLocPlot += [y]

plt.plot(xLocPlot, yLocPlot, 'r,')
plt.savefig('rawfig.png')

plt.show();
