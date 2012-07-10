#!/usr/bin/python

import sys

length = 0
with open(sys.argv[1], 'r') as file:
    for line in file:
        length += len(line) #word count of line
        #add other operator here
        
print length
        
