#!/usr/bin/Rscript

args <- commandArgs(trailingOnly = TRUE)
InMatFile <-args[1]


if (length(args) != 1) 
{
	cat("\t\tUsage: Rscript vistreecode.r  p_dis.mat \n")
	cat("\t\tPlease provide the input matrix file p_dis.mat \n")
	q("no")
}


if  (!file.exists(InMatFile))
{
	cat("\t\tInPut p_dis.mat is no exists,please check\n")
	q("no")
}



if(!require(ape)) install.packages("ape")
library(ape)

# read data from file 
matrix_data <- read.table(InMatFile, header=F, row.names=1,skip=1)
dist_matrix <- as.dist(as.matrix(matrix_data))

# Construct a tree using UPGMA/NJ method
# tree <-  as.phylo(hclust(dist_matrix, method = "average"))   # your can try UPGMA
tree <- nj(dist_matrix)
# print tree
#pdf("treeA.pdf") ;print(tree); dev.off();

# save the tree to Newick format file
write.tree(tree, file="output_tree.nwk") 

if (!require("BiocManager", quietly = TRUE))  install.packages("BiocManager")
if(!require(ggtree)) BiocManager.packages("ggtree")	
library(ggtree)
g <- ggtree(tree,layout="equal_angle")  # see more at ggtree


# save tree pdf
ggsave("tree_visualization.pdf", g)


