#!/bin/sh
#$ -S /bin/sh
echo Start Time :
date

cp  src/Ver/VCF2Dis_multi.cpp   src/VCF2Dis.cpp
g++ -g  -O2 -fopenmp   src/VCF2Dis.cpp   -lz  -L  src/zlib/   -o  bin/VCF2Dis >/dev/null 2>&1

if [ $? -eq 0 ]
then
	echo compile with multi-thread compile [recommand for sample number\> 1k]
	echo g++ -g  -O2 -fopenmp	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis_multi
	mv  bin/VCF2Dis  bin/VCF2Dis_multi
	cp  src/Ver/VCF2Dis_single.cpp  src/VCF2Dis.cpp
	echo g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
	g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
	ln -s   VCF2Dis_single   bin/VCF2Dis
#	g++ -g  -O2 -fopenmp	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis
else	
	echo compile Without multi-thread
	#echo cp  src/Ver/VCF2Dis_single.cpp src/VCF2Dis.cpp
	cp  src/Ver/VCF2Dis_single.cpp src/VCF2Dis.cpp
	echo g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
	g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
fi

chmod 755  bin/*
rm src/VCF2Dis.cpp
echo End Time :
date
