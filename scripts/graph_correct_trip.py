#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt
from datetime import datetime
from math import hypot

totalLine = 0
nCorrects = 0
trip = 1
yLocPlot = []
xLocPlot = []
xCorPlot = []
yCorPlot = []

for line in fileinput.input():
    totalLine+=1
    if line == '\n':
        plt.plot(xCorPlot, yCorPlot, 'bo')
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
        xLocPlot+= [int(line[1])]
        yLocPlot+= [int(line[2])]

plt.axis('equal')
plt.show();
