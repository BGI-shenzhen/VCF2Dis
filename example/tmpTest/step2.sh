#!/bin/sh
#Version1.0	heweiming@qq.com	2024-06-20
echo Start Time : 
date
inVCF=Bi-allele.vcf
time	/home/heweiming/01.Soft/plink/plink1.9	--vcf	$inVCF	--distance-matrix	--out	Distance.out
perl  ChangFormat.pl   Distance.out.mdist.id  Distance.out.mdist   new.mat
time   Rscript    vistreecode.r   new.mat
echo End Time : 
date
