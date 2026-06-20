#!/bin/sh
#$ -S /bin/sh
echo Start Time :
date

CPPFLAGS="-O3 -march=native"
echo "Try compiling with: $CPPFLAGS"
cp  src/Ver/VCF2Dis_multi.cpp   src/VCF2Dis.cpp
g++ -g $CPPFLAGS -fopenmp   src/VCF2Dis.cpp   -lz  -L  src/zlib/   -o  bin/VCF2Dis 2>&1

if [ $? -eq 0 ]
then
	echo compile with multi-thread compile [recommand for sample number\> 1k]
	echo g++ -g $CPPFLAGS -fopenmp	src/VCF2Dis.cpp	-lz	-L	src/zlib/	-o	bin/VCF2Dis_multi
	mv  bin/VCF2Dis  bin/VCF2Dis_multi
	cp  src/Ver/VCF2Dis_single.cpp  src/VCF2Dis.cpp
	echo g++ -g $CPPFLAGS   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
	g++ -g $CPPFLAGS   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
	ln -s   VCF2Dis_single   bin/VCF2Dis
else
	echo "Compile with $CPPFLAGS failed, fallback to -O2"
	cp  src/Ver/VCF2Dis_multi.cpp   src/VCF2Dis.cpp
	g++ -g  -O2 -fopenmp   src/VCF2Dis.cpp   -lz  -L  src/zlib/   -o  bin/VCF2Dis
	if [ $? -eq 0 ]
	then
		echo compile with multi-thread compile [recommand for sample number\> 1k]
		mv  bin/VCF2Dis  bin/VCF2Dis_multi
		cp  src/Ver/VCF2Dis_single.cpp  src/VCF2Dis.cpp
		echo g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
		g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis_single
		ln -s   VCF2Dis_single   bin/VCF2Dis
	else	
		echo compile Without multi-thread
		cp  src/Ver/VCF2Dis_single.cpp src/VCF2Dis.cpp
		echo g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
		g++ -g  -O2   src/VCF2Dis.cpp -lz -L  src/zlib/   -o  bin/VCF2Dis
	fi
fi

chmod 755  bin/*
rm src/VCF2Dis.cpp
echo End Time :
date
