options(java.parameters="-Xmx6G")
unloadNamespace("fastreeR")
library(fastreeR)
library(utils)
library(ape)
library(stats)
library(grid)
library(BiocFileCache)
#!/usr/bin/Rscript

args <- commandArgs(trailingOnly = TRUE)
#InMatFile <-args[1]
tempVcf <- args[1]
	#"/home/heweiming/Test/VCF2Dis-1.52/example/Example1/Khuman.vcf"
myVcfDist <- fastreeR::vcf2dist(inputFile = tempVcf, threads = 1)
myVcfTree <- fastreeR::dist2tree(inputDist = myVcfDist)
tree<-ape::read.tree(text = myVcfTree)
write.tree(tree, file="output_treeFastTree.nwk")

