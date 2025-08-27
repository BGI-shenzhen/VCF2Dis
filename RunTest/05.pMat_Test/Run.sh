cp  ../../example/Example1/Khuman.vcf.gz  ./
#cp  ../VCF2Dis-1.53/example/Example1/Khuman.vcf.gz  ./
time  Rscript aa.r  Khuman.vcf.gz   fastR.mat  2> /dev/null
time  ../VCF2Dis-1.53/bin/VCF2Dis_single  -InPut Khuman.vcf.gz -OutPut   vcf2dis.mat  -NoOUTtree  > /dev/null
cut -f 1-5 fastR.mat  |head -5
cut -f 1-5 vcf2dis.mat |head -6
