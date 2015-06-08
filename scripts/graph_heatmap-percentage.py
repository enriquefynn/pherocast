#!/usr/bin/env python2
import fileinput, sys
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from pylab import colorbar
from matplotlib.colors import LogNorm, LinearSegmentedColormap, BoundaryNorm
def getMap(filename):
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
        if line[0] != '0':
            xCorPlot += [int(line[1])]
            yCorPlot += [int(line[2])]

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
    plt.hist2d(xCorPlot, yCorPlot, bins=100, cmap=cmap, norm=LogNorm())
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')

    #ax = plt.gca()
    plt.title("Heatmap")
    plt.colorbar()
    plt.savefig('correctNumberHeatmap.pdf')
    plt.show()

if __name__ == '__main__':
    getMap(sys.argv[1])

