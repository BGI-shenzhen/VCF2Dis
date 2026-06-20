options(java.parameters="-Xmx6G")
unloadNamespace("fastreeR")
library(fastreeR)
library(utils)
#!/usr/bin/Rscript

args <- commandArgs(trailingOnly = TRUE)
tempVcf <- args[1]
outputFile <- args[2]
fastRDis <- fastreeR::vcf2dist(inputFile = tempVcf, threads = 1)
fastRDis <- as.matrix(fastRDis)
write.table(fastRDis, file = outputFile, sep = "\t", quote = FALSE, row.names = TRUE, col.names = TRUE)
