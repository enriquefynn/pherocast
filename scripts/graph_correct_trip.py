#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt
from datetime import datetime
from math import hypot

totalLine = 0
nCorrects = 0
trip = 1
xmax = ymax = 0 
xmin = ymin = float('inf')
yLocPlot = []
xLocPlot = []
xCorPlot = []
yCorPlot = []

for line in fileinput.input():
    totalLine+=1
    if line == '\n':
        plt.plot(xCorPlot, yCorPlot, 'b.')
        plt.plot(xLocPlot, yLocPlot, 'r-')
        yLocPlot = []
        xLocPlot = []
        xCorPlot = []
        yCorPlot = []
        continue
    line = line.split()
    if line[0] != '0':
        xCorPlot+= [int(line[1])]
        yCorPlot+= [int(line[2])]
    if line[1] != '0' or line[2] != '0':
        xmax = max(xmax, int(line[1]))
        ymax = max(ymax, int(line[2]))
        xmin = min(xmin, int(line[1]))
        ymin = min(ymin, int(line[2]))
        xLocPlot+= [int(line[1])]
        yLocPlot+= [int(line[2])]

print xmin, xmax, ymin, ymax
plt.savefig('correctPredictions.png')
plt.show()

