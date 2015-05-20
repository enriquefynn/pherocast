#!/usr/bin/env python2
import re, sys
import matplotlib.pyplot as plt

def getGraph(filename):
    totalLine = 0
    time = []
    stdDev = []
    cars = []
    filen = open(filename, 'r')
    for line in filen:
        splitted = line.split()
        try:
            totalLine+=1
            time+= [float(splitted[0])/1000.]
            stdDev+= [float(splitted[1])/1000.]
            cars += [float(splitted[2])]
        except Exception as ex:
            print(ex)
            print(line)
            break

    fig, plt1 = plt.subplots()
    plt1.plot(range(totalLine), time, 'r.', label='Average Time')
    plt1.plot(range(totalLine), stdDev, 'g.', label='Standard deviation')
    plt1.set_xlabel('Period')
    plt1.set_ylabel('Time in ms', color='r')
    for tl in plt1.get_yticklabels():
        tl.set_color('r')
    plt2 = plt.twinx()
    plt2.plot(range(totalLine), cars, 'b--', label='Number of cars')
    plt2.set_ylabel('cars', color='b')
    for tl in plt2.get_yticklabels():
        tl.set_color('b')
    plt1.legend(loc=2)
    plt2.legend()
    plt.savefig('timeAvg.png')
    plt.show();

if __name__ == '__main__':
    getGraph(sys.argv[1])
