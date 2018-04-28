#!/bin/bash

symbol_files=("$@")
num_of_files=${#symbol_files[@]}
#for i in "${symbol_files[@]}"; do
for ((i = 0; i < ${num_of_files}; i++));
do
	readelf -s ${symbol_files[$i]} | awk '($4 ~ "FUNC")&&((/read/)||(/write/)||(/READ/)||(/WRITE/)) {print $8}' > func_names.txt
	num=$[$i+1]
	./WriteBps func_names.txt bpFile$num.gdb
	#cat bpFile.gdb
done
