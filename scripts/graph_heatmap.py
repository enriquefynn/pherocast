#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt
from datetime import datetime

totalLine = 0
nCorrects = 0
yLocPlot = []
xLocPlot = []
xCorPlot = []
yCorPlot = []
trip = 1

for line in fileinput.input():
    totalLine+=1
    if line == '\n':
        continue
    line = line.split()
    if line[0] != '0':
        xCorPlot+= [int(line[1])]
        yCorPlot+= [int(line[2])]
    xLocPlot+= [int(line[1])]
    yLocPlot+= [int(line[2])]
    if int(line[-1]) != trip:
        trip = int(line[-1])
        plt.plot(xLocPlot[:-1], yLocPlot[:-1], 'g-')
        xLocPlot = []
        yLocPlot = []


#plt.plot(xLocPlot, yLocPlot, 'r-')
#plt.plot(xCorPlot, yCorPlot, 'bo')

from matplotlib.colors import LogNorm
import matplotlib.pyplot as plt
from numpy.random import randn

x = xCorPlot;
y = yCorPlot;

H, xedges, yedges, img = plt.hist2d(x, y, norm=LogNorm())

fig = plt.figure()
ax = fig.add_subplot(111)
im = ax.imshow(H, cmap=plt.cm.jet, norm=LogNorm(), interpolation='mitchell')
fig.colorbar(im, ax=ax)
plt.show()
