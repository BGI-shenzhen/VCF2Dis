#!/bin/sh
echo Start Time : 
date
#cp  ../../example/Example1/Khuman.vcf.gz  ./
##  NA19006 From JPT to CEU ##  
Rscript	aa.r	Khuman.vcf.gz	
echo End Time : 
date
