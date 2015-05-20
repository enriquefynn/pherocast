#!/usr/bin/env python2
import re, sys, fileinput
import matplotlib.pyplot as plt

lines = []
accuracy = []

for line in fileinput.input():
    line = line.split()
    try:
        lines += [int(line[0])]
        accuracy += [float(line[1])]
    except:
        break

plt.plot(lines, accuracy, 'r.')
plt.xscale('log')
plt.xlabel('Entries')
plt.ylabel('Accuracy')

plt.show();
