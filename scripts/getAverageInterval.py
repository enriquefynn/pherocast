#!/usr/bin/env python2

import re, sys, fileinput

total = 0
PS = 0
avg=0
for line in fileinput.input():
    line = line.split()
    TS = int(line[0])
    if TS - PS < 10000:
        avg+= (TS-PS)
        total+=1
    PS = TS
print(avg)
print(total)
avg/= total
print(avg)
