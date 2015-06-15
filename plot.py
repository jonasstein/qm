# python plot.py <file>

import numpy as np
import pylab as pl
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("sourcefilename")
args=parser.parse_args()
sourcefilename =  args.sourcefilename

def read_file(filename):
    left = []
    mid = []
    right = []
    counts = []
    myfile = open(filename, 'r')
    for line in myfile:
        if not (line[0] == '#'):
            words = line.split(', ')
            left.append(float(words[0]))
            mid.append(float(words[1]))
            right.append(float(words[2]))
            counts.append(int(words[3]))
    myfile.close()
    data = {'left': left, \
            'mid': mid, \
            'right': right, \
            'counts': counts}
    return data

data = read_file(sourcefilename)

pl.plot(data['mid'], data['counts'])
pl.show()
