#!/usr/bin/env python2
import re, sys, fileinput

totalLine = 0
nCorrects = 0
nNodes = 0
for line in fileinput.input():
    totalLine+=1
    if line == '\n':
        continue
    line = line.split()
    if line[0] != '0':
        nCorrects+=1
    nNodes = int(line[-2])
potential = float(100* (totalLine-nNodes)) / totalLine
actual = 100*nCorrects/float(totalLine)

print("{0:.2f} {1:.2f}".format(actual, potential))
