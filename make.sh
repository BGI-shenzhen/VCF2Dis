#!/bin/sh
#$ -S /bin/sh
echo Start Time :
date

cp  src/Ver/VCF2Dis_1.50.cpp   src/VCF2Dis.cpp
g++ -g  -O2 -fopenmp   src/VCF2Dis.cpp   -lz  -L  src/zlib/   -o  bin/VCF2Dis >/dev/null 2>&1

if [ $? -eq 0 ]
then
	echo compile with multi-thread compile
	echo g++ -g  -O2 -fopenmp	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis
#	g++ -g  -O2 -fopenmp	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis
else	
	echo compile Without multi-thread
	echo Warning: Since the single-thread of V1.50 is slower than the single thread of V1.47, it is recommended to get the V1.47 to re-compile VCF2Dis for SingelThread version
	echo cp  src/Ver/VCF2Dis_1.47.cpp src/VCF2Dis.cpp
	cp  src/Ver/VCF2Dis_1.47.cpp src/VCF2Dis.cpp
	echo g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
	g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
fi

chmod 755  bin/*
rm src/VCF2Dis.cpp
echo End Time :
date
