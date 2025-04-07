#!/bin/sh
echo Start Time : 
date
##Please make sure R and related R packages are installed before running ##
/bin/time	-v	-p	Rscript	aa.r	M1.vcf	
/bin/time	-v	-p	Rscript	aa.r	M2.vcf	
/bin/time	-v	-p	Rscript	aa.r	M3.vcf	
/bin/time	-v	-p	Rscript	aa.r	M4.vcf	
/bin/time	-v	-p	Rscript	aa.r	M5.vcf	
/bin/time	-v	-p	Rscript	aa.r	M6.vcf	
/bin/time	-v	-p	Rscript	aa.r	M7.vcf	
/bin/time	-v	-p	Rscript	aa.r	M8.vcf	
/bin/time	-v	-p	Rscript	aa.r	M9.vcf	
/bin/time	-v	-p	Rscript	aa.r	M10.vcf	
echo End Time : 
date
