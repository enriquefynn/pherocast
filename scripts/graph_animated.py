#!/usr/bin/env python2
import fileinput
import select, sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from math import hypot

totalLine = 0
nCorrects = 0
start = []
yLocPlot = []
xLocPlot = []
xCorPlot = []
yCorPlot = []
trips = []
corrects = []

xmax = -10000000000
xmin = 10000000000
ymax = -10000000000
ymin = 10000000000

for line in fileinput.input():
    totalLine+=1
    if line == '\n':
        continue
    line = line.split()
    if line[0] != '0':
        corrects+= [1]
        xCorPlot+= [int(line[1])]
        yCorPlot+= [int(line[2])]
    else:
        corrects+= [0]
    trip = line[-1]
    if line[1] != '0' and line[2] != '0':
        xmax = max(xmax, int(line[1]))
        xmin = min(xmin, int(line[1]))
        ymax = max(ymax, int(line[2]))
        ymin = min(ymin, int(line[2]))
    xLocPlot+= [int(line[1])]
    yLocPlot+= [int(line[2])]
    if totalLine > 2:
        dist = hypot(xLocPlot[-2] - xLocPlot[-1], yLocPlot[-2] - yLocPlot[-1])
        if dist > 50:
            xLocPlot.pop()
            yLocPlot.pop()
    trips+= [int(line[-1])]


def heardEnter():
    i,o,e = select.select([sys.stdin],[],[],0.0001)
    for s in i:
        if s == sys.stdin:
            input = sys.stdin.readline()
            return True
    return False

def simData():
    paused = False
    percentageCorrect = 0.
    cummulativePercentage = 0
    l = 0
    x = []
    y = []
    while l < totalLine: 
        if paused:
            paused = raw_input()
        if heardEnter():
            paused = not paused
        l+=1
        x += [xLocPlot[l]]
        y += [yLocPlot[l]]
        cummulativePercentage+= corrects[l]
        percentageCorrect = float(cummulativePercentage)/l
        yield x, y, trips[l], corrects[l], percentageCorrect

def simPoints(simData):
    x, y = simData[0], simData[1]
    lines[0][0].set_data(x, y)
    lines[1][0].set_data(x[-1], y[-1])
    tripText.set_text('Trip: ' + str(simData[2]))
    correctText.set_text('Correct: ' + str('%.3f' % (100*simData[4])) + '%')
    if (simData[3] == 1):
        lines[1][0].set_color('blue')
    else:
        lines[1][0].set_color('red')
    return lines

fig = plt.figure()
ax = fig.add_subplot(111)

ax.set_ylim(ymin, ymax)
ax.set_xlim(xmin, xmax)
tripText = ax.text(0.05, 0.9, '', transform=ax.transAxes)
correctText = ax.text(0.05, 0.85, '', transform=ax.transAxes)

lines = [ax.plot([], [], 'g-', ms=10), ax.plot([], [], 'bo', ms=10)]

lineAni = animation.FuncAnimation(fig, simPoints, simData, interval=1, blit=False)

plt.show()
