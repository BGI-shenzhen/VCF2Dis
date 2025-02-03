
INVCF=ALL.chr1.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz
#Here is an example of how to obtain the M*.vcf files, with the actual data residing on the giga FTP. 
# You can use ‘rand’ or other samples, or different chromosomes to retrieve these VCF files
zcat $INVCF |  head -1000000 |cut -f 1-100 > ../01.TestBySite/M1.vcf 
zcat $INVCF |  head -2000000 |cut -f 1-100 > ../01.TestBySite/M2.vcf 
zcat $INVCF |  head -3000000 |cut -f 1-100 > ../01.TestBySite/M3.vcf 
zcat $INVCF |  head -4000000 |cut -f 1-100 > ../01.TestBySite/M4.vcf 
zcat $INVCF |  head -5000000 |cut -f 1-100 > ../01.TestBySite/M5.vcf 
zcat $INVCF |  head -6000000 |cut -f 1-100 > ../01.TestBySite/M6.vcf 
zcat $INVCF |  head -7000000 |cut -f 1-100 > ../01.TestBySite/M7.vcf 
zcat $INVCF |  head -8000000 |cut -f 1-100 > ../01.TestBySite/M8.vcf 
zcat $INVCF |  head -9000000 |cut -f 1-100 > ../01.TestBySite/M9.vcf 
zcat $INVCF |  head -10000000 |cut -f 1-100 > ../01.TestBySite/M10.vcf 
gzip ../01.TestBySite/M*.vcf

