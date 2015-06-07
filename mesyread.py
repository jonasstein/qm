#! /usr/bin/env python2
# -*- coding: utf-8 -*-

"""
Created on Fri Apr 10 17:00:07 2015

@author: stein
"""

def string2bits(bytestring):
    MAXi = len(bytestring)    

    outbits = ''   
    for i in range(MAXi):
        outbits = outbits + byte2bits(bytestring[i])
    return(outbits)
        

def byte2bits(mybyte):
    return(bin(ord('a'))[2:])


def getevent(sixbytes):
    bitpattern = string2bits(sixbytes)
    
    ID = bitpattern[0]            #  1 bit
    ModID = bitpattern[1:4]       #  3 bits
    SlotID = bitpattern[4:9]      #  5 bits
    Amplitude = bitpattern[9:19]  # 10 bits
    Position = bitpattern[19:29]  # 10 bits
    Timestamp = bitpattern[29:48] # 19 bits

    print(ID, ModID, SlotID, Amplitude, Timestamp)
#    return(ID, ModID, SlotID, Amplitude, Position, Timestamp)

import sys
import os
import argparse
import numpy as np

HEADERLENGTH = 40

HeaderSep =  '0x00005555AAAAFFFF'
DataSep =    '0x0000FFFF5555AAAA'
ClosingSig = '0xFFFFAAAA55550000'
# "{0:016b}".format(0x5555)


LISTFILE_FILENAME = "/home/stein/my/uni/qm/listfile.mdat"
listfile = open(LISTFILE_FILENAME, 'rb')

#statinfo = os.stat(listfile)

            
LENGTH_OF_EVENT = 6 # 48 Bit = 6 Byte
LENGTH_OF_EVENT_SEPARATOR = 8 # 4 x 16 Bit 

LIST_FILE_SIZE = os.path.getsize(LISTFILE_FILENAME)
NUMBER_OF_EVENTS = 5 # (LIST_FILE_SIZE - HEADERLENGTH) / 

#record_dtype = np.dtype( [ ( 'headersep' , 'b64' ) , ( 'dblock' , 'b4') ] )
record_dtype = np.dtype( [ ( 'ID' , 'b1' ) , 
                          ( 'ModID' , 'b3'),                           
                          ( 'SlotID' , 'b5'), 
                          ( 'Amplitude' , 'b10'), 
                          ( 'Position' , 'b10'), 
                          ( 'Timestamp' , 'b10') ] )

data = np.fromfile(listfile , dtype = record_dtype , count = 8 )


print("We expect %d events"%NUMBER_OF_EVENTS)

header = listfile.read(HEADERLENGTH)

i=0

for i in range(0,NUMBER_OF_EVENTS):
    thisevent =  listfile.read(6)
    print(i)
    getevent(thisevent)
    
    #event[0:5][i] = thisevent #' '.join(format(ord(i),'b').zfill(8) for i in thisevent) 
#    i = i + 1
    

