#!/bin/bash

./convert.awk SVM_DATA/train_sample{_label,}.txt > train.txt &&
./convert.awk SVM_DATA/test_sample{_label,}.txt > test.txt
