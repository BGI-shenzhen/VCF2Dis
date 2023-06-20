#!/bin/sh
#$ -S /bin/sh
#Version1.0	hewm@genomics.org.cn	2017-06-13
echo Start Time : 
date
../bin/VCF2Dis	-InPut	in.vcf.gz	-OutPut p_dis.mat
#../bin/VCF2Dis	-InPut	in.vcf.gz	-OutPut p_dis.mat  -SubPop  sample.list
echo End Time : 
date
