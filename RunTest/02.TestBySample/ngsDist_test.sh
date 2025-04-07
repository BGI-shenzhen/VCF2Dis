#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: $0  <vcf_file> <output_prefix> "
	exit 1
fi

VCF_FILE=$1
OUTPUT_PREFIX=$2
NGSDIST_PATH=/home/heweiming/VCF2Dis_RunTest/ngsDist-Test/ngsDist
PLINK_PATH=/home/heweiming/01.Soft/plink/plink1.9

$PLINK_PATH --double-id --vcf $VCF_FILE --recode A-transpose --out $OUTPUT_PREFIX > /dev/null

#rm INPUT.geno.gz 
tail -n +2 $OUTPUT_PREFIX.traw | cut -f 1,4- | perl -p -e 's/\bNA\b/-1/g' | gzip > $OUTPUT_PREFIX.INPUT.geno.gz

zcat  $VCF_FILE  |  head  -1000  | grep CHROM | cut -f 10- | tr "\t" "\n" > $OUTPUT_PREFIX.INPUT.labels

N_IND=$(wc -l < $OUTPUT_PREFIX.INPUT.labels)
N_SITES=$(zcat $OUTPUT_PREFIX.INPUT.geno.gz | wc -l)

# run  ngsDist
echo /bin/time -v  $NGSDIST_PATH --geno INPUT.geno.gz --n_ind $N_IND --n_sites $N_SITES --out OUT 2> /dev/null
/bin/time -v  $NGSDIST_PATH --geno $OUTPUT_PREFIX.INPUT.geno.gz --n_ind $N_IND --n_sites $N_SITES --out OUT 2>  OUT$OUTPUT_PREFIX.ere > OUT$OUTPUT_PREFIX.log


