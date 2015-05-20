#!/usr/bin/env python2
import re, sys, fileinput
import numpy as np
import numpy.random
import matplotlib.pyplot as plt
from pylab import colorbar
from matplotlib.colors import LogNorm

totalLine = 0
nCorrects = 0
yLocPlot = []
xLocPlot = []
xCorPlot = []
yCorPlot = []
trip = 1

for line in fileinput.input():
    totalLine+=1
    if totalLine == 1:
        continue
    line = line.split()
    if line[0] != '0':
        xCorPlot+= [int(line[1])]
        yCorPlot+= [int(line[2])]
    xLocPlot+= [int(line[1])]
    yLocPlot+= [int(line[2])]
    if int(line[-1]) != trip:
        trip = int(line[-1])
        xLocPlot = []
        yLocPlot = []


plt.hist2d(xCorPlot, yCorPlot, bins=80, cmap='Greens', norm=LogNorm())
ax = plt.gca()
plt.title("Heatmap")
plt.colorbar()
plt.show()
