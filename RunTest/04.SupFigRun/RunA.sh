#!/bin/sh
#Version1.0	hewm2008@gmail.com	2024-05-16
echo Start Time : 
date

##  Khuman.vcf.gz is a small dataset from VCF2Dis Example 1  ##
cp  ../../example/Example1/Khuman.vcf.gz  ./
##  Randomly extracted from chr22 and consistent with the data used in VCF2PCACluster ##
../VCF2Dis-1.53/bin/VCF2Dis_single   -InPut  Khuman.vcf.gz   -OutPut p_dis.mat  -InSampleGroup pop.info

echo End Time : 
date
