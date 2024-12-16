#!/bin/sh
#Version1.0	hewm2008@gmail.com	2024-05-16
echo Start Time : 
date
##    Simple usage   ##
#../../bin/VCF2Dis	-InPut	Khuman.vcf.gz	-OutPut	p_dis.mat	

##   For Input population for tree plot ##
../../bin/VCF2Dis   -InPut  Khuman.vcf.gz   -OutPut p_dis.mat  -InSampleGroup pop.info

##   For  sub sample example ##
#../../bin/VCF2Dis  -InPut  Khuman.vcf.gz   -OutPut p_dis.mat  -SubPop subample.list   


##  to re-Run tree pdf ##
#   Rscript  vistreecode.r  p_dis.mat 2> /dev/null 

echo End Time : 
date
