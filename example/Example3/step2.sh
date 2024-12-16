#!/bin/bash

NN=100
if [ "$#"  -eq  1 ]; then
   NN=$1
fi

cat  p_*.nwk  >    alltree_merge.tre   #  cat  tree*.tre  > alltree_merge.tre

../../../PHYLIPNEW-3.69.650/bin/fconsense   -intreefile   alltree_merge.tre  -outfile out  -treeprint Y
perl  ../../bin/percentageboostrapTree.pl alltree_merge.treefile  $NN    Final_boostrap.tre


