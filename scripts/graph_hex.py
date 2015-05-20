#!/usr/bin/env python2
import re, sys, fileinput
from matplotlib import pyplot as PLT
from matplotlib import cm as CM
from matplotlib import mlab as ML
import numpy as NP

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

x = y = NP.linspace(-5, 5, 100)
X, Y = NP.meshgrid(xCorPlot[:1000], yCorPlot[:1000])
Z1 = ML.bivariate_normal(X, Y, 2, 2, 0, 0)
Z2 = ML.bivariate_normal(X, Y, 4, 1, 1, 1)
ZD = Z2 - Z1
x = X.ravel()
y = Y.ravel()
z = ZD.ravel()
gridsize=30
PLT.subplot(111)

# if 'bins=None', then color of each hexagon corresponds directly to its count
# 'C' is optional--it maps values to x-y coordinates; if 'C' is None (default) then 
# the result is a pure 2D histogram 

PLT.hexbin(X, Y, C=z, cmap=CM.jet, bins=None)

cb = PLT.colorbar()
cb.set_label('valor')
PLT.show()
