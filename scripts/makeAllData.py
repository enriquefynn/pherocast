#!/usr/bin/env python3
import re, sys, fileinput
from datetime import datetime

def writeAll(filename, filenames):
    allFile = open(filename, 'w')
    files = []
    for elem in filenames:
        files += [open(elem, 'r')]
    line = 0
    while len(files) > 0:
        print(line, end='\r')
        line+=1
        try:
            avg = 0.
            filesToRemove = []
            if line == 1:
                for f in files:
                    f.readline()
            for f in files:
                data = f.readline()
                parsed = data.split()
                if data == '':
                    filesToRemove += [f]
                    if len(files) == 0:
                        break
                else:
                    allFile.write(data)
            for elem in filesToRemove:
                files.remove(elem)
            if len(files) == 0:
                break
        except Exception as ex:
            print(ex)
            print(f)
            print(line)
            break

filenames = []
for line in fileinput.input():
    filenames += [line[:-1]]

writeAll('allFiles.txt', filenames)

