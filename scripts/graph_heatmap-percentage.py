#!/usr/bin/env python2
import fileinput, sys
import matplotlib.pyplot as plt
from pylab import colorbar
from matplotlib.colors import LogNorm, LinearSegmentedColormap, BoundaryNorm

def getMap(filename, cap = 1):
    xmin = 100000000
    xmax = 0
    ymin = 100000000
    ymax = 0
    l = 0
    totalLine = 0
    corPlotSet = {}
    ncorPlotSet = {}
    xCorPlot = []
    yCorPlot = []
    filein = open(filename, 'r')

    for line in filein:
        l+=1
        if l %100000 == 0:
            print(l)
        totalLine+=1
        if line == '\n':
            continue
        line = line.split()
        x = int(line[1])
        y = int(line[2])
        xmax = max(xmax, x)
        xmin = min(xmin, x)
        ymax = max(ymax, y)
        ymin = min(ymin, y)
        hashed = line[1] + ' ' + line[2]
        if line[0] != '0':
            if not hashed in corPlotSet:
                corPlotSet[hashed] = 1
            else:
                corPlotSet[hashed]+= 1
        else:
            if not hashed in ncorPlotSet:
                ncorPlotSet[hashed] = 1
            else:
                ncorPlotSet[hashed]+= 1

    for elem in corPlotSet:
        splitted = elem.split()
        x = int(splitted[0])
        y = int(splitted[1])
        right = corPlotSet[elem]
        n = float(right)
        if elem in ncorPlotSet:
            n+= ncorPlotSet[elem]
        for i in xrange(int((right/n)*100)):
            xCorPlot+= [x]
            yCorPlot+= [y]

    cmap = LinearSegmentedColormap('custom', {
        'red': ((0.0, 1.0, 1.0),
            (0.0, 1.0, 1.0),
            (1.0, 0.0, 0.0)),
        'green': ((0.0, 1.0, 1.0),
            (0.0, 0.0, 0.0),
            (1.0, 1.0, 1.0)),
        'blue': ((0.0, 1.0, 1.0),
            (0.0, 0.0, 0.0),
            (1.0, 0.0, 0.0))}, 256)

    print(len(xCorPlot))
    plt.hist2d([1,2,3,4,4,4,4,4], [1,2,3,4,4,4,4,4], bins=4, cmap=cmap)
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')

    #ax = plt.gca()
    plt.title("Heatmap")
    plt.colorbar()
    plt.show()

if __name__ == '__main__':
    if len(sys.argv) == 2:
        getMap(sys.argv[1])
    elif len(sys.argv) == 3:
        getMap(sys.argv[1], int(sys.argv[2]))

