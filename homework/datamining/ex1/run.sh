#!/bin/bash

./data_gen.awk $1 > data.py && python apriori.py
