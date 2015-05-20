#!/usr/bin/env python3
import re, sys, fileinput
from datetime import datetime

totalLine = 0
nCorrects = 0
grtr= 0.0
lst = 100.0
for line in fileinput.input():
    totalLine+=1
    if totalLine == 1:
        continue
    try:
        nArgs = 1
        line = line.split()
        percentage = float(line[1])
        if percentage > grtr:
            grtr = percentage
        if percentage < lst and percentage > 10.0:
            lst = percentage
        nCorrects += percentage
        if percentage < 10.0:
            totalLine-=1
        nArgs+=1
    except:
        print('Error in line: ', totalLine)
        exit(1)
print(nCorrects/totalLine)
print(grtr)
print(lst)
