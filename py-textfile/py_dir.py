#!/usr/bin/python

import sys, os

if os.path.exists(sys.argv[1]):
    dirpath = sys.argv[1]
    os.chdir(dirpath)
    if os.path.isdir(dirpath):
        subpathlist = os.listdir(dirpath)

mode = sys.argv[2]

def sizeofdir(dir):
    size = 0
    for root, dirs, files in os.walk(dir):
        size += sum([os.path.getsize(os.path.join(root, name)) for name in files])
    return size

def sorter (mode):
    if mode == '-l':
        for path in subpathlist:
            if os.path.isdir(path):
                flag = 'd'
            else:
                flag = 'f'
            print str(sizeofdir(os.path.abspath(path))) + '\t' + flag + '\t' + path

sorter(mode)
