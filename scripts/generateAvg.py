#!/usr/bin/env python2
import fileinput
import numpy as np
from datetime import datetime
from math import sqrt


def getAvg(extension, filename, filenames):
    avgFile = open(filename, 'w')
    files = []
    for elem in filenames:
        f = elem + '.' + extension
        files += [open(f, 'r')]
    line = 0
    while len(files) > 0:
        line+=1
        try:
            avg = 0.
            means = []
            filesToRemove = []
            for f in files:
                data = f.readline()
                if data == '':
                    filesToRemove += [f]
                    if len(files) == 0:
                        break
                else:
                    means+= [float(data)]
            for elem in filesToRemove:
                files.remove(elem)
            if len(files) == 0:
                break
            means.sort()
            percentage = int(len(means)*0.1)
            if len(means) - percentage > 0 and percentage > 0:
                means = means[percentage:len(means)-percentage]
            avgFile.write('{} {} {}'.format(np.average(means), np.std(means), len(means)))
            avgFile.write('\n')
        except Exception as ex:
            print(ex)
            print(f)
            print(line)
            break

filenames = []
for line in fileinput.input():
    filenames += [line[:-1]]

getAvg('time', 'avgTime.txt', filenames)
getAvg('size', 'avgSize.txt', filenames)
