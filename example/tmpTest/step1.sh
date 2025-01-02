#!/bin/sh
#$ -S /bin/sh

## Compare with plink ,see the Test.word ##


InChr22VCF=ALL.chr22.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz

if [ -e $InChr22VCF ]
then
	echo wget done,check  md5sum : $InChr22VCF
	wget  -c   https://ftp.1000genomes.ebi.ac.uk/vol1/ftp/release/20130502/ALL.chr22.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz  
else
	wget  -c   https://ftp.1000genomes.ebi.ac.uk/vol1/ftp/release/20130502/ALL.chr22.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz  
fi
perl  PickBi-Allele.pl  $InChr22VCF   Bi-allele.vcf  
time  ../../bin/VCF2Dis_multi   -i  Bi-allele.vcf    -o  p_dis.mat   -InSampleGroup sample.group
