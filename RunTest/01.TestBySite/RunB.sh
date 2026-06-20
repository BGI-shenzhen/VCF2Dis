#!/bin/sh
echo Start Time : 
date
# Please make sure R and related R packages are installed before running
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M1.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M2.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M3.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M4.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M5.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M6.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M7.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M8.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M9.vcf	-OutPut	p_dis.mat	-NoOUTtree
/bin/time	-v	-p	../VCF2Dis-1.53/bin/VCF2Dis	-InPut	M10.vcf	-OutPut	p_dis.mat	-NoOUTtree
echo End Time : 
date
