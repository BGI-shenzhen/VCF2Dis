#!/bin/sh
#$ -S /bin/sh

for i in {1..22}
do
	InChrVCF=ALL.chr${i}.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz
	if [ -e $InChrVCF ]
	then
		echo "File $InChrVCF already exists, check md5sum."
		wget -c https://ftp.1000genomes.ebi.ac.uk/vol1/ftp/release/20130502/ALL.chr${i}.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz
	else
		echo "Downloading $InChrVCF..."
		wget -c https://ftp.1000genomes.ebi.ac.uk/vol1/ftp/release/20130502/ALL.chr${i}.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz
	fi
done


