#!/usr/bin/env python2
import fileinput, sys
import matplotlib.pyplot as plt
from pylab import colorbar
from matplotlib.colors import LogNorm

def getMap(filename, cap = 1):
    l = 0
    totalLine = 0
    corPlotSet = {}
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
            hashed = line[1] + ' ' + line[2]
            if not hashed in corPlotSet:
                corPlotSet[hashed] = 0
            else:
                corPlotSet[hashed]+=1

    for elem in corPlotSet:
        splitted = elem.split()
        for i in xrange(corPlotSet[elem]/cap):
            xCorPlot+= [int(splitted[0])]
            yCorPlot+= [int(splitted[1])]

    plt.hist2d(xCorPlot, yCorPlot, bins=80, cmap='Greens', norm=LogNorm())
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

