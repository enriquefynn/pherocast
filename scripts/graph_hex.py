#!/usr/bin/env python2
import re, sys, fileinput
from matplotlib import pyplot as plt
import seaborn as sns
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
        #plt.plot(xLocPlot[:-1], yLocPlot[:-1], 'r-')
        xLocPlot = []
        yLocPlot = []

import numpy as np
cmap = plt.cm.gist_heat_r
hexmap = sns.jointplot(np.array(xCorPlot), np.array(yCorPlot), stat_func=None, kind='hex', space=0, bins=40)

ax = hexmap.ax_joint

ax.set_xlim(min(xCorPlot), max(xCorPlot))
ax.set_ylim(min(yCorPlot), max(yCorPlot))

#ax.tick_params(labelbottom='off', labelleft='off')

plt.show()
