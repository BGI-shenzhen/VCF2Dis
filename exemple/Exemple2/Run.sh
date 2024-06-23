#!/bin/sh
echo Start Time : 
date
###  For InPut FA Format ###
../../bin/VCF2Dis	-InPut	single-copy.cds.fa.gz	-OutPut	p_dis.mat	-InFormat	FA	
###  For InPut PHY Format ###
#../../bin/VCF2Dis	-InPut	single-copy.cds.phy.gz	-OutPut	p_dis.mat	-InFormat	PHY
echo End Time : 
date
