INVCF=ALL.chr1.phase3_shapeit2_mvncall_integrated_v5b.20130502.genotypes.vcf.gz

#Here is an example of how to obtain the S*.vcf files, with the actual data residing on the giga FTP.
# You can use ‘rand’ or other samples, or different chromosomes to retrieve these VCF files

perl GetSampleVCF.pl biallelic.snp.gz  $INVCF 2500 > ../02.TestBySample/02.S2500.vcf
INVCF=../02.TestBySample/02.S2500.vcf

perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 2000 > ../02.TestBySample/02.S2000.vcf
INVCF=../02.TestBySample/02.S2000.vcf

perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 1500 > ../02.TestBySample/02.S1500.vcf
INVCF=../02.TestBySample/02.S1500.vcf

perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 1000 > ../02.TestBySample/02.S1000.vcf
INVCF=../02.TestBySample/02.S1000.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 900 > ../02.TestBySample/02.S900.vcf
INVCF=../02.TestBySample/02.S900.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 800 > ../02.TestBySample/02.S800.vcf
INVCF=../02.TestBySample/02.S800.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 700 > ../02.TestBySample/02.S700.vcf
INVCF=../02.TestBySample/02.S700.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 600 > ../02.TestBySample/02.S600.vcf
INVCF=../02.TestBySample/02.S600.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 500 > ../02.TestBySample/02.S500.vcf
INVCF=../02.TestBySample/02.S500.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 400 > ../02.TestBySample/02.S400.vcf
INVCF=../02.TestBySample/02.S400.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 300 > ../02.TestBySample/02.S300.vcf
INVCF=../02.TestBySample/02.S300.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 200 > ../02.TestBySample/02.S200.vcf
INVCF=../02.TestBySample/02.S200.vcf


perl  GetSampleVCF.pl biallelic.snp.gz $INVCF 100 > ../02.TestBySample/02.S100.vcf
INVCF=../02.TestBySample/02.S100.vcf

gzip  ../02.TestBySample/02.S*.vcf

