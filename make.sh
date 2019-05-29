#!/bin/sh
#$ -S /bin/sh
echo Start Time : 
date
echo g++ -g  -O2	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis
g++ -g  -O2	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis
echo End Time :
date
