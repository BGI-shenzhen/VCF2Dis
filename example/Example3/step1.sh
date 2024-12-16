#!/bin/bash

NN=100
if [ "$#"  -eq  1 ]; then
  NN=$1
fi

for X in $(seq 1 $NN)
do
   ../../bin/VCF2Dis -InPut ../Example1/Khuman.vcf.gz  -OutPut p_dis_${X}.mat -Rand 0.25
#   ../../../PHYLIPNEW-3.69.650/bin/fneighbor -datafile p_dis_${X}.mat  -outfile tree.out1_${X}.txt -matrixtype s -treetype n -outtreefile tree.out2_${X}.tre
done

