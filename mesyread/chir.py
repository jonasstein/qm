# python chir.py <file1> <file2> <filesum>

import numpy as np

import argparse

parser = argparse.ArgumentParser()
parser.add_argument("sourcefilename1")
parser.add_argument("sourcefilename2")
parser.add_argument("destfilename")
args=parser.parse_args()
sourcefilename1 =  args.sourcefilename1
sourcefilename2 =  args.sourcefilename2
destfilename = args.destfilename

def read_file(filename):
    left = []
    mid = []
    right = []
    counts = []
    error = []
    myfile = open(filename, 'r')
    for line in myfile:
        if not (line[0] == '#'):
            words = line.split(', ')
            left.append(float(words[0]))
            mid.append(float(words[1]))
            right.append(float(words[2]))
            counts.append(int(words[3]))
            error.append(float(words[4]))
    myfile.close()
    data = {'left': left, \
            'mid': mid, \
            'right': right, \
            'counts': counts, \
            'error': error}
    return data

data1 = read_file(sourcefilename1)
data2 = read_file(sourcefilename2)

data = {'left': [], 'mid': [], 'right': [], 'counts': [], 'error': []}

for i in range(len(data1['left'])):
    data['left'].append((data1['left'][i] + data2['left'][i]) / 2.0)
    data['mid'].append((data1['mid'][i] + data2['mid'][i]) / 2.0)
    data['right'].append((data1['right'][i] + data2['right'][i]) / 2.0)
    data['counts'].append( (data1['counts'][i] - data2['counts'][i]) / \
                            (data1['counts'][i] + data2['counts'][i]) )
    data['error'].append(np.sqrt( data1['error'][i]**2 * 4 * data2['counts'][i]**2 / (data1['counts'][i] + data2['counts'][i])**4 / \
                                + data2['error'][i]**2 * 4 * data1['counts'][i]**2 / (data1['counts'][i] + data2['counts'][i])**4) )


myfile = open(destfilename, 'w')
myfile.write("# Sum of the histos in the two files: \n")
myfile.write("# " + sourcefilename1 + '\n')
myfile.write("# " + sourcefilename2 + '\n')
for i in range(len(data['left'])):
    line = str(data['left'][i]) + " " + str(data['mid'][i]) \
         + " " + str(data['right'][i]) \
         + " " + str(data['counts'][i]) \
         + " " + str(data['error'][i])
    myfile.write(line + '\n')
    
myfile.close();
